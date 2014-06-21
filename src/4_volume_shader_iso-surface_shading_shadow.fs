
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
  for(int i = 0; i < int(last_sample); ++i){
  	vec4 data  = look_up(sample_pos);
	if(in_between(last_data.a, data.a, iso)){

            sample_pos = find_best(last_sample_pos, sample_pos, last_data.a, data.a, iso,0.05);


	    vec3 diffuse = look_up(sample_pos).rgb;
	
	    // do shading with gradients
	    vec3 iso_surface_normal = normalize(get_gradient(sample_pos)) * -1.0;


            // calc shading with little camera light
	    vec3 vec_to_camera = normalize(ray_origin - sample_pos);
	    float ndotl = dot(iso_surface_normal,vec_to_camera);
            color = ndotl * diffuse * gl_LightSource[0].diffuse.rgb * camera_contribution;

            // calc ray to light
	    vec3 dir_to_light = normalize(light_pos - sample_pos);
            vec3 light_volume_exit_point = get_exit(sample_pos,dir_to_light);

	    float light_last_sample = length(vec3(light_volume_exit_point - sample_pos)) * inv_sample_distance -4.0;
            float lit = 1.0;
            vec3 light_ray_step = sample_distance * dir_to_light;
            // advance shadowray a bit in direction to light
            sample_pos += 4.0 * light_ray_step;
            for(int s = 4; s < int(light_last_sample); ++s){
                float alpha = look_up(sample_pos).a;
		if(alpha >= iso){
                    lit = 0.0;
                    break;
                }
                sample_pos += light_ray_step;
            }
            if(lit != 0.0){
               color += diffuse * gl_LightSource[0].diffuse.rgb * max(0.0,dot(iso_surface_normal,dir_to_light));
	       vec3 half_vec = normalize(dir_to_light - ray_dir);
               color += specular * gl_LightSource[0].specular.rgb*pow(max(0.0,dot(iso_surface_normal, half_vec)), shininess);
            }

            break;
        }
        last_data = data;
	last_sample_pos = sample_pos;
        sample_pos += ray_step;
  }

  //gl_FragColor = vec4(1.0,0.0,0.0,1.0);
  gl_FragColor = vec4(color*color_bias,1.0);
}


