
/*

  author: 2008 by Stephan Beck
  mail:   stephan@pixelstars.de

*/

// #include "../src/shader_codebase.h"


void main( void )
{

  check_valid();

  vec3 specular = vec3(1.0,1.0,1.0);
  float shininess = 128.0;
  float camera_contribution = 0.5;



  float iso = min(1.0,floating_param_strength);
  float sampling_correction = 50.0 * sample_distance;

  vec3 intensity_i = vec3(0.0,0.0,0.0);
  vec3 intensity = vec3(0.0,0.0,0.0);
  float rest_transparency = 1.0;

  vec4 last_data = look_up(sample_pos);
  vec3 last_sample_pos = sample_pos;
  for(int i = 0; i < int(last_sample); ++i){

    	vec4 data =  look_up(sample_pos);
        float opaticty_i = data.a * sampling_correction;
	float transparency_i = 1.0 - opaticty_i;
	
	if(in_between(last_data.a, data.a, iso)){

            sample_pos = find_best(last_sample_pos, sample_pos, last_data.a, data.a, iso,0.05);

	    vec3 gradient = get_gradient(sample_pos);
	    float shading_opacity = data.a * length(gradient);
            // only do it if the contribution is worth it
	    if(shading_opacity > 0.05){

	    	// do shading with gradient
	    	vec3 iso_surface_normal = normalize(gradient) * -1.0;

		vec4 data = look_up(sample_pos);

                // calc shading with little camera light
	        vec3 vec_to_camera = normalize(ray_origin - sample_pos);
	        float weight = dot(iso_surface_normal,vec_to_camera);
                intensity_i = weight * data.rgb * gl_LightSource[0].diffuse.rgb * camera_contribution;

                 // calc ray to light
	         vec3 dir_to_light = normalize(light_pos - sample_pos);
                 intensity_i += data.rgb * gl_LightSource[0].diffuse.rgb * max(0.0,dot(iso_surface_normal,dir_to_light));
                 vec3 half_vec = normalize(dir_to_light - ray_dir);
                 intensity_i += specular * gl_LightSource[0].specular.rgb*pow(max(0.0,dot(iso_surface_normal, half_vec)), shininess);
  
	         // intensity_i *= shading_opacity;
                 // transparency_i = 1.0 - shading_opacity;
    

             }
	     else{
	    	 intensity_i = data.rgb * opaticty_i;
	     }

        }
	else{
	    intensity_i = data.rgb * opaticty_i;
	}

	intensity = intensity + intensity_i * rest_transparency;
	rest_transparency = rest_transparency * transparency_i;



        if(rest_transparency < 0.05)
	     	break;

        last_sample_pos = sample_pos;
	sample_pos += ray_step;
        last_data = data;
	

  }
  
  gl_FragColor = vec4(intensity*color_bias,1.0);
}
