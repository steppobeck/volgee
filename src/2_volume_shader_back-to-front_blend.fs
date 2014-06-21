
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
  sample_pos = sample_pos + last_sample*ray_step;
  for(int i = int(last_sample) - 1; i >=0; --i){
  	vec4 data_i = look_up(sample_pos);
        float opacity_i = data_i.a * opacity_correction;
        // back to front compositing
	intensity = data_i.rgb * opacity_i + intensity*(1.0 - opacity_i);
	sample_pos -= ray_step;
  }
  
  gl_FragColor = vec4(intensity*color_bias,1.0);
}
