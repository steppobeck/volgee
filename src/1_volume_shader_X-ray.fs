
/*

  author: 2008 by Stephan Beck
  mail:   stephan@pixelstars.de

*/


// #include "../src/shader_codebase.h"


/*

  AVG Intenstity Traversal Scheme

  works best with ramp

*/

void main( void )
{

  check_valid();

  float intensity = 0.0;
  
  for(int i = 0; i < int(last_sample); ++i){
  	intensity  += look_up(sample_pos).a;
	sample_pos += ray_step;
  }
  // normalize global
  intensity = intensity * sample_distance * floating_param_strength; // floating_param_strength - default is 1.0

  gl_FragColor = vec4(intensity * color_bias.r,intensity * color_bias.g,intensity * color_bias.b,1.0);
}
