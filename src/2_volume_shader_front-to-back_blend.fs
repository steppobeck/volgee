
/*

  author: 2008 by Stephan Beck
  mail:   stephan@pixelstars.de

*/

// #include "../src/shader_codebase.h"



void main( void )
{

  check_valid();

  float opacity_correction = floating_param_strength * 50.0 * sample_distance;

  // front to back compositing with early ray termination
  vec3 intensity = vec3(0.0,0.0,0.0);
  float rest_transparency = 1.0;
  for(int i = 0; i < int(last_sample); ++i){
  	vec4 data_i = look_up(sample_pos);
        float opaticity_i = data_i.a * opacity_correction;
        float transparency_i = 1.0 - opaticity_i;

	vec3 intensity_i = data_i.rgb * opaticity_i;
	
	// blending
	intensity = intensity + intensity_i * rest_transparency;
	rest_transparency = rest_transparency * transparency_i;

	// early ray termination
	if(rest_transparency < 0.05)
	     break;

	sample_pos += ray_step;
  }
  
  gl_FragColor = vec4(intensity*color_bias,1.0);
}
