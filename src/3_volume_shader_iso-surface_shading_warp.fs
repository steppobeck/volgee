
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

  vec3 color = vec3(0.0,0.0,0.0);
  float iso = floating_param_strength;
  vec4 last_data = look_up(sample_pos);

  vec3 last_sample_pos = sample_pos;
  for(float i = 0; i < last_sample; ++i){
        vec4 data  = look_up(sample_pos);
	if(in_between(last_data.a, data.a, iso)){
	
            sample_pos = find_best(last_sample_pos, sample_pos, last_data.a, data.a, iso,0.05);

	    vec3 diffuse = look_up(sample_pos).rgb;
	    // do shading with gradients ...optimal would be to calculate gradient the other way round
            // so one doesnt need * -1.0
	    vec3 iso_surface_normal = normalize(get_gradient(sample_pos)) * -1.0;


            // calc shading with little camera light
	    vec3 vec_to_camera = normalize(ray_origin - sample_pos);
            
	    float ndotl = dot(iso_surface_normal,vec_to_camera);
            color = ndotl * diffuse * gl_LightSource[0].diffuse * camera_contribution;

            // calc ray to light
	    vec3 dir_to_light = normalize(light_pos - sample_pos);
            color += diffuse * gl_LightSource[0].diffuse * max(0.0,dot(iso_surface_normal,dir_to_light));
            vec3 half_vec = normalize(dir_to_light - ray_dir);
            color += specular * gl_LightSource[0].specular*pow(max(0.0,dot(iso_surface_normal, half_vec)), shininess);
     
            break;
        }
        last_data = data;
	last_sample_pos = sample_pos;
	sample_pos += ray_step;
  }

  gl_FragColor = vec4(color * color_bias,1.0);
}


