
/*

  author: 2008 by Stephan Beck
  mail:   stephan@pixelstars.de

*/

varying vec3 volume_entry_point;
void main( void )
{
   gl_Position = ftransform();
   volume_entry_point = gl_Vertex.xyz;
   gl_TexCoord[0].st = gl_MultiTexCoord0.xy;
}
