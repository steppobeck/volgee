
/*

  author: 2008 by Stephan Beck
  mail:   stephan@pixelstars.de

*/

// #include "../src/shader_codebase.h"


/*

  MAX Intenstity Traversal Scheme

  works best with ramp

*/


void main( void )
{

  check_valid();

  float intensity = 0.0;
  
  for(int i = 0; i < int(last_sample); ++i){
  	intensity  = max(look_up(sample_pos).a, intensity);
	sample_pos += ray_step;
  }
  intensity = intensity * floating_param_strength; // floating_param_strength - default is 1.0
  gl_FragColor = vec4(intensity * color_bias.r,intensity * color_bias.g,intensity * color_bias.b,1.0);
}






