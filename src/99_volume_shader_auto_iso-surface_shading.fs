
/*

  author: 2008 by Stephan Beck
  mail:   stephan@pixelstars.de

*/

// #include "../src/shader_codebase.h"



void main( void )
{



  check_valid();

  vec3 color = vec3(0.0,0.0,0.0);


  float min_gradient = floating_param_strength * 0.2;

  float sampling_correction = 2000.0 * sample_distance * min_gradient;
  
  vec3 specular = vec3(1.0,1.0,1.0);
  float shininess = 128.0;
  float camera_contribution = 0.5;

  
  float rest_transparency = 1.0;
  
  for(int i = 0; i < int(last_sample); ++i){
	vec3 gradient = get_gradient(sample_pos);
	if(length(gradient) > min_gradient){
            
	    vec4 data  = look_up(sample_pos);
            float opacity_i = data.a * sampling_correction;
	    float transparency_i = 1.0 - opacity_i;

	    // do shading with gradients
	    vec3 iso_surface_normal = normalize(gradient) * -1.0;
            // calc shading with little camera light
            vec3 color_i = vec3(0.0,0.0,0.0);
	    vec3 vec_to_camera = normalize(ray_origin - sample_pos);
	    float weight = max(0.0,dot(iso_surface_normal,vec_to_camera));
	    if(weight != 0.0){            
		color_i = weight * data.rgb * gl_LightSource[0].diffuse.rgb * camera_contribution;
            }
            	// calc ray to light
            vec3 dir_to_light = normalize(light_pos - sample_pos);
            weight = max(0.0,dot(iso_surface_normal,dir_to_light));
            if(weight != 0.0){            
            	color_i += weight * data.rgb * gl_LightSource[0].diffuse.rgb;
            	vec3 half_vec = normalize(dir_to_light - ray_dir);
            	color_i += specular * gl_LightSource[0].specular.rgb*pow(max(0.0,dot(iso_surface_normal, half_vec)), shininess);
     	        
	    }

            color = color + color_i * rest_transparency * opacity_i;
	    rest_transparency = rest_transparency * transparency_i;
            // early ray termination
	    if(rest_transparency < 0.10)
            	break;
        }
	sample_pos += ray_step;
  }

  gl_FragColor = vec4(color * 1.1 * color_bias,1.0);
}



