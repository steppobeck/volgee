
/*

  author: 2008 by Stephan Beck
  mail:   stephan@pixelstars.de

*/


uniform sampler3D volume;
//uniform sampler2D transferfunction;
uniform sampler1D transferfunction;
uniform sampler2D jitterfunction;
uniform vec3 ray_origin;
uniform vec3 light_pos;
uniform float sample_distance;
uniform float inv_sample_distance;
uniform float grid_step_left;
uniform float grid_step_right;
uniform float floating_param_strength;

uniform float jitter_strength;

uniform vec3 clipping_plane_pos;
uniform vec3 clipping_plane_normal;
uniform float clipping_plane_distance_to_origin;
uniform float use_clipping_plane;


varying vec3 volume_entry_point;


vec3 color_bias = vec3(1.0,1.0,1.0);
vec3 vol_min = vec3(0.0,0.0,0.0);
vec3 vol_max = vec3(1.0,1.0,1.0);
int max_iterations = 8;

// after http://www.cs.utah.edu/~awilliam/box/

vec3
get_exit(vec3 r_origin, vec3 r_direction){
    float tmin;
    float tmax;
    float tymin;
    float tymax;
    float tzmin;
    float tzmax;

    vec3 rec_dir = vec3(vec3(1.0)/r_direction);

    if (r_direction.x >= 0.0) {
        tmin = (vol_min.x - r_origin.x) * rec_dir.x;
        tmax = (vol_max.x - r_origin.x) * rec_dir.x;
    }
    else {
        tmin = (vol_max.x - r_origin.x) * rec_dir.x;
        tmax = (vol_min.x - r_origin.x) * rec_dir.x;
    }

    if (r_direction.y >= 0.0) {
        tymin = (vol_min.y - r_origin.y) * rec_dir.y;
        tymax = (vol_max.y - r_origin.y) * rec_dir.y;
    }
    else {
        tymin = (vol_max.y - r_origin.y) * rec_dir.y;
        tymax = (vol_min.y - r_origin.y) * rec_dir.y;
    }



    if (tymin > tmin) {
        tmin = tymin;
    }

    if (tymax < tmax) {
        tmax = tymax;
    }

    if (r_direction.z >= 0.0) {
        tzmin = (vol_min.z - r_origin.z) * rec_dir.z;
        tzmax = (vol_max.z - r_origin.z) * rec_dir.z;
    }
    else {
        tzmin = (vol_max.z - r_origin.z) * rec_dir.z;
        tzmax = (vol_min.z - r_origin.z) * rec_dir.z;
    }

    if (tzmin > tmin) {
        tmin = tzmin;
    }

    if (tzmax < tmax) {
        tmax = tzmax;
    }

    return r_origin + (tmax * r_direction);
}

vec4 look_up(vec3 sample_pos){
  //return texture2D(transferfunction,vec2(texture3D( volume, sample_pos).a,0.5));
  return texture1D(transferfunction, texture3D( volume, sample_pos).a);
}




vec3 get_gradient(vec3 sample_pos){
  float x_l  = look_up(sample_pos + vec3(grid_step_left,0.0,0.0)).a;
  float x_r  = look_up(sample_pos + vec3(grid_step_right,0.0,0.0)).a;
 
  float y_l  = look_up(sample_pos + vec3(0.0,grid_step_left,0.0)).a;
  float y_r  = look_up(sample_pos + vec3(0.0,grid_step_right,0.0)).a;
  
  float z_l  = look_up(sample_pos + vec3(0.0,0.0,grid_step_left)).a;
  float z_r  = look_up(sample_pos + vec3(0.0,0.0,grid_step_right)).a;
  
  vec3 gradient = vec3((x_r - x_l)/2.0,(y_r - y_l)/2.0,(z_r - z_l)/2.0);
  return gradient;
}

bool in_between(float a, float b, float v){
	return ( a < v && b >= v ) || ( b < v && a >= v );
}


// find the best sample_pos ...the one that is nearest to the iso
// using just linear interpolation
vec3 find_best2(vec3 last_sample_pos,vec3 sample_pos,float last_d,float d,float iso,float threshold){

        float u = (iso - last_d)/(d - last_d);
	return vec3(last_sample_pos + u * (sample_pos - last_sample_pos));

}

// using binary search
vec3 find_best(vec3 last_sample_pos,vec3 sample_pos,float last_d,float d,float iso,float threshold){

        // check if we are within the threshold (e.g. 5%)
	float inv_iso = 1.0/iso;

	if(abs(d-iso)*inv_iso < threshold)
           return vec3(sample_pos);
	if(abs(last_d-iso)*inv_iso < threshold)
           return vec3(last_sample_pos);


	vec3 start_pos = last_sample_pos;
	vec3 end_pos = sample_pos;

	float d_min = min(last_d,d);
	float d_max = max(last_d,d);

	if(d_min != last_d){
		vec3 tmp = start_pos;
		start_pos = end_pos;
		end_pos = tmp;
	}

	// from now we have 


	float best_d = d;
        vec3 mid = vec3(0.0,0.0,0.0);
        for(int i = 0; i < max_iterations; ++i){
		mid = 0.5*(start_pos + end_pos);
		best_d = look_up(mid).a;

		if(abs(best_d - iso)*inv_iso < threshold){
			return vec3(mid);
		}

		if(best_d >= iso){
			end_pos = mid;
		}
		else{
			start_pos = mid;
		}
		
	}

        return vec3(mid);
}

/*
uniform vec3 clipping_plane_pos;
uniform vec3 clipping_plane_normal;
uniform float clipping_plane_distance_to_origin;
*/
float intersect_ray_plane(vec3 o, vec3 d){
  return -1.0 * (dot(clipping_plane_normal, o) - clipping_plane_distance_to_origin)/dot(clipping_plane_normal, d);
}


/* with jitter...depricated
vec3 jitter =  (jitter_strength * 0.0005)*(texture2D(jitterfunction,gl_TexCoord[0].st).rgb - vec3(0.5,0.5,0.5));
vec3 ray_dir_tmp = vec3(volume_entry_point + jitter  - ray_origin);
*/




// without
vec3 ray_dir_tmp = vec3(volume_entry_point - ray_origin);
float dist_to_volume_entry = length(ray_dir_tmp);


vec3 ray_dir = normalize(ray_dir_tmp);

float steps = dist_to_volume_entry * inv_sample_distance;


// These three variables are needed by every shader
// a short ray segment in order to travers the volume
vec3 ray_step = sample_distance * ray_dir;
// the starting sample position
vec3 sample_pos = ray_origin + (steps + 4.0) * ray_step;
// the amount of samples
//vec3 volume_exit_point = get_exit(ray_origin,ray_dir);
//float last_sample = length(vec3((volume_exit_point - 4.0 * ray_step) - sample_pos)) * inv_sample_distance;
vec3 volume_exit_point = vec3(1.0);
float last_sample = 0.0;


void check_valid(){

  volume_exit_point =  get_exit(ray_origin,ray_dir);
  last_sample = length(vec3((volume_exit_point - 4.0 * ray_step) - sample_pos)) * inv_sample_distance;

  if(use_clipping_plane != 0.0){

    float entry_behind = 0.0;
    if(dot(clipping_plane_normal,vec3(volume_entry_point - clipping_plane_pos)) < 0.0){
      entry_behind = 1.0;
    }

    float exit_behind = 0.0;
    if(dot(clipping_plane_normal,vec3(volume_exit_point - clipping_plane_pos)) < 0.0){
      exit_behind = 1.0;
    }

    // test A
    if((entry_behind + exit_behind) == 2.0){
      discard;
    }
    // test B
    else if((entry_behind + exit_behind) == 0.0){
      ;
    }
    // test C
    else if(entry_behind == 0.0 && exit_behind == 1.0){

      // our new exit_point is intersection point ray with plane
      volume_exit_point = ray_origin + intersect_ray_plane(ray_origin, ray_dir) * ray_dir;
      last_sample = length(vec3((volume_exit_point - 4.0 * ray_step) - sample_pos)) * inv_sample_distance;
    }
    // test D
    else {
	
	     // our new entry_point is intersection point ray with plane
	     vec3 volume_entry_point_tmp = ray_origin + intersect_ray_plane(ray_origin, ray_dir) * ray_dir;
	     float dist_to_volume_entry = length(vec3(volume_entry_point_tmp - ray_origin));
	     float steps = dist_to_volume_entry * inv_sample_distance;
	     sample_pos = ray_origin + (steps + 4.0) * ray_step;
    }

  }
}							   




// Check wether ray starts inside a volume could be implemented

/* cannot happen within this context because bbx is not clipped from volgee
  if (inside(ray_origin)) {
	sample_pos = ray_origin;
  }
*/


/*
bool inside(vec3 p){
    if (vol_min.x > p.x || p.x > vol_max.x) {
        return false;
    }
    if (vol_min.y > p.y || p.y > vol_max.y) {
        return false;
    }
    if (vol_min.z > p.z || p.z > vol_max.z) {
        return false;
    }
    return true;
}



bool outside(vec3 sample_pos){
     return vol_min.x > sample_pos.x ||
     	    vol_min.y > sample_pos.y ||
	    vol_min.z > sample_pos.z ||
	    vol_max.x < sample_pos.x ||
	    vol_max.y < sample_pos.y ||
	    vol_max.z < sample_pos.z;
}
*/
