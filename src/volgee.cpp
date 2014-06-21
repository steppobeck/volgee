#include <CMDParser.h>
#include <Directory.h>
#include <Timer.h>
#include <StringTokenizer.h>
#include <GLSLPreprocessor.h>
#include <volgee_helper.h>
#include <VolumeInfo.h>
#include <Transferfunction.h>
// gee includes

#include <gee_textfield_button.h>
#include <gee_trackball.h>
#include <gee_surface_button.h>
#include <gee_dragger.h>
#include <gee_envelope2.h>

/// gloost includes
#include <gloostHelper.h>
#include <gloostRenderGoodies.h>
#include<Shader.h>
#include<UniformSet.h>
#include<TextureManager.h>
#include<Texture.h>
#include <Point3.h>
#include <PointLight.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cstdlib>
#include <iostream>

#include <algorithm>
#include <functional>



///////////////////////////////////////////////////////////////////////////////

// Screen attributes
unsigned int g_screenWidth(1200);
unsigned int g_screenHeight(800);
unsigned int g_frameCounter(0);
unsigned int g_hSpace(400);
unsigned int g_renderWidth(g_screenWidth - g_hSpace);
unsigned int g_renderHeight(g_screenHeight);

float g_renderAspect(((float) g_renderWidth)/g_renderHeight);
float g_fov(45.0);
float g_near(0.1);
float g_far(30.0);
bool g_raycast_direct = true;
const float g_default_floating_param_strength = 0.5;
float g_floating_param_strength = g_default_floating_param_strength;
gloost::PointLight* light;
gloost::Point3 g_light_start_pos(0.0,1.5,0.0);
gloost::Point3 g_light_pos(g_light_start_pos);
gloost::TextureManager* g_texManager = 0;
gloost::Shader*  g_shader     = 0;
gloost::UniformSet*  g_uniforms   = 0;
unsigned int g_volumeTextureId = 0;
unsigned int g_transferFunctionTextureId = 0;
unsigned int g_jitterFunctionTextureId = 0;
unsigned char* g_jitterTextureBuffer = 0;

const unsigned int g_interactive_samples = 100;
unsigned int g_num_samples = g_interactive_samples;

VolumeInfo* g_volume_info = 0;
std::string g_current_volume_file = "";
#ifdef LINUX
sys::Timer g_timer;
#endif



unsigned int   g_volumeSize;
sys::Directory* g_volume_files;
sys::Directory* g_shader_vs_files;
sys::Directory* g_shader_fs_files;
sys::Directory* g_transferFunction_files;
sys::Directory* g_volume_recording_files;
bool g_animate(false);

std::vector<std::string> g_vec_shader_info;
unsigned int g_shader_info = 0;

gee::gee_element*   g_gui;
gee::gee_trackball* g_trackball;
gee::gee_trackball* g_trackball_light;
gee::gee_trackball* g_trackball_clipping_plane;
gee::gee_surface_button* g_flip_button;
gee::gee_surface_button* g_clipping_button;
gee::gee_surface_button* g_camera_button;
gee::gee_surface_button* g_light_button;
bool g_draw_light = false;


gee::gee_surface_button* g_floating_param_button_plus;
gee::gee_surface_button* g_floating_param_button_minus;
gee::gee_textfield_button* g_floating_param_info = 0;

float g_default_oversampling = 1.0;
float g_oversampling = g_default_oversampling;
gee::gee_surface_button* g_oversampling_button_plus;
gee::gee_surface_button* g_oversampling_button_minus;
gee::gee_textfield_button* g_oversampling_info = 0;




gee::gee_surface* top_bg = 0;
gee::gee_surface* bottom_bg = 0;
gee::gee_textfield* bottom_info = 0;

gee::gee_textfield_button* browse_volumes_button;
std::vector<gee::gee_textfield_button*> g_volume_files_buttons;

gee::gee_textfield_button* browse_shaders_button;
std::vector<gee::gee_textfield_button*> g_shader_files_buttons;

gee::gee_textfield_button* browse_transferFunctions_button;
std::vector<gee::gee_textfield_button*> g_transferFunction_files_buttons;


gee::gee_envelope2* g_envelope_alpha;
gee::gee_envelope2* g_envelope_red;
gee::gee_envelope2* g_envelope_green;
gee::gee_envelope2* g_envelope_blue;

Transferfunction* g_transferfunction;

gloost::Point3 g_clipping_plane_pos(0.5,0.5,0.5);
gloost::Vector3 g_clipping_plane_start_normal(1.0,0.0,0.0);
gloost::Vector3 g_clipping_plane_normal(g_clipping_plane_start_normal);
bool g_draw_clipping_plane = false;
bool g_use_clipping_plane = false;

///////////////////////////////////////////////////////////////////////////////


void genJitterTexture(){

   const unsigned int jitterTextureBufferSize = 256*256*3;
   g_jitterTextureBuffer = new unsigned char [jitterTextureBufferSize];
   for(unsigned int i=0; i < jitterTextureBufferSize; ++i){
       const float random_value = gloost::frand();
       g_jitterTextureBuffer[i] = (unsigned char)(255.0 * random_value);
   }
}



bool load_shader(std::string vs, std::string fs){

  if(g_shader){
    delete g_shader;
  }
  GLSLPreprocessor pp;
  g_shader = new gloost::Shader(vs,pp.process(fs.c_str()));

  return true;
}


bool load_tf(std::string tf_file){
  
  g_transferfunction->load(tf_file.c_str());

  if(g_transferFunctionTextureId){
    g_texManager->dropReference(g_transferFunctionTextureId);
    g_texManager->cleanUp();
  }



  gloost::Texture* tfTexture = new gloost::Texture(TF_TEXTURE_WIDTH,
						   1,
						   1,
						   g_transferfunction->get_texture_data(),
						   GL_TEXTURE_1D,
						   GL_RGBA,
						   GL_RGBA,
						   GL_UNSIGNED_BYTE);

  tfTexture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP);

  g_transferFunctionTextureId = g_texManager->addTexture(tfTexture);

  // deprecated
  //tfTexture->generate();
  tfTexture->bind();
  

  g_uniforms->set_sampler1D("transferfunction", g_transferFunctionTextureId);



  g_envelope_alpha->set_tf_keys(g_transferfunction->get_keys_alpha());
  g_envelope_red->set_tf_keys(g_transferfunction->get_keys_red());
  g_envelope_green->set_tf_keys(g_transferfunction->get_keys_green());
  g_envelope_blue->set_tf_keys(g_transferfunction->get_keys_blue());


  return true;
}

void on_update_tf(int index, const std::vector<tf_key>& tf_keys){


  switch (index){

  case 0:
    g_transferfunction->set_keys_alpha(tf_keys);
    break;
  case 1:
    g_transferfunction->set_keys_red(tf_keys);
    break;
  case 2:
    g_transferfunction->set_keys_green(tf_keys);
    break;
  case 3:
    g_transferfunction->set_keys_blue(tf_keys);
    break;

  default:
    break;
  }


  if(g_transferFunctionTextureId){
    g_texManager->dropReference(g_transferFunctionTextureId);
    g_texManager->cleanUp();
  }



  gloost::Texture* tfTexture = new gloost::Texture(TF_TEXTURE_WIDTH,
						   1,
						   1,
						   g_transferfunction->get_texture_data(),
						   GL_TEXTURE_1D,
						   GL_RGBA,
						   GL_RGBA,
						   GL_UNSIGNED_BYTE);

  tfTexture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP);

  g_transferFunctionTextureId = g_texManager->addTexture(tfTexture);

  // depreacted
  //tfTexture->generate();
  tfTexture->bind();


  g_uniforms->set_sampler1D("transferfunction", g_transferFunctionTextureId);


}


bool load_volume(std::string volume_file){
  
  g_current_volume_file = volume_file;

  // create volume data
  if(g_volume_info)
    delete g_volume_info;
  // Analyse Volume Dimension etc.
  g_volume_info = new VolumeInfo(volume_file.c_str());
  


  { // setting g_volumeSize to best power of two
    g_volumeSize = g_volume_info->get_max_dim() - 1;
    //std::cerr << g_volumeSize << std::endl;
    int power = 0;
    while(g_volumeSize){
      g_volumeSize >>= 1;
      ++power;
    }
    
    g_volumeSize = (1 << power);

  }


  if(g_volumeSize > 512){
    std::cerr << g_volumeSize << std::endl;
    exit(0);
  }
  const unsigned int volsize = g_volumeSize*g_volumeSize*g_volumeSize;
  unsigned char* volumeData = new unsigned char [volsize] ;

  unsigned int x_start = (g_volumeSize - g_volume_info->dim[0])/2;
  unsigned int x_end = x_start+g_volume_info->dim[0];

  unsigned int y_start = (g_volumeSize - g_volume_info->dim[1])/2;
  unsigned int y_end = y_start+g_volume_info->dim[1];

  unsigned int z_start = (g_volumeSize - g_volume_info->dim[2])/2;
  unsigned int z_end = z_start+g_volume_info->dim[2];




  // load a raw file Hack
  if(g_volume_info->bits_per_channel == 8){
    
    FILE* volfile = fopen(volume_file.c_str(),"rb");
  
    if(!volfile) std::cerr << "error opening volume file" << std::endl;
    
    fseek (volfile , 0 , SEEK_END);
    long volFileSize = ftell (volfile);
    unsigned char* volumeData_raw = new unsigned char [volFileSize] ;
    
    rewind (volfile);
    long nbr = fread (volumeData_raw,g_volume_info->bits_per_channel/8,volFileSize,volfile);
    if(nbr != volFileSize) std::cerr << "error opening volume file" << std::endl;
    fclose(volfile);
    const float weight(1.0f/g_volume_info->channels);
    unsigned int pos_raw = 0;
    unsigned int pos = 0;
    for(unsigned int z = 0; z < g_volumeSize; ++z){
      for(unsigned int y = 0; y < g_volumeSize; ++y){
	for(unsigned int x = 0; x < g_volumeSize; ++x){
	  if( x >= x_start && x < x_end &&
	      y >= y_start && y < y_end &&
	      z >= z_start && z < z_end){
	    if(g_volume_info->channels > 1){
	      float d = 0;
	      for(unsigned int i = 0; i < g_volume_info->channels; ++i){
		d += (float) volumeData_raw[pos_raw];
	      }
	      volumeData[pos] = (unsigned char) (d*weight);
	      pos_raw += g_volume_info->channels; 
	    }
	    else{
	      volumeData[pos] = volumeData_raw[pos_raw];
	      ++pos_raw;
	    }
	  }
	  else{
	    volumeData[pos] = 0;
	  }
	  ++pos;
	}
      }
    }
    
    delete [] volumeData_raw;
  }
  else if(g_volume_info->bits_per_channel == 16){
    //std::cerr << "using unsigned short" << std::endl;
    FILE* volfile = fopen(volume_file.c_str(),"rb");
  
    if(!volfile) std::cerr << "error opening volume file" << std::endl;
    
    fseek (volfile , 0 , SEEK_END);
    long volFileSize = ftell (volfile);
    unsigned short* volumeData_raw = new unsigned short [volFileSize] ;
    
    rewind (volfile);
    long nbr = fread (volumeData_raw,g_volume_info->bits_per_channel/8,volFileSize,volfile);
    if(nbr != volFileSize) std::cerr << "error opening volume file" << std::endl;
    fclose(volfile);
    
    unsigned int pos_raw = 0;
    unsigned int pos = 0;
    for(unsigned int z = 0; z < g_volumeSize; ++z){
      for(unsigned int y = 0; y < g_volumeSize; ++y){
	for(unsigned int x = 0; x < g_volumeSize; ++x){
	  if( x >= x_start && x < x_end &&
	      y >= y_start && y < y_end &&
	      z >= z_start && z < z_end){
	    
	    volumeData[pos] = (unsigned char)  (255 *  (((float) volumeData_raw[pos_raw]) / (1 << g_volume_info->bits_per_channel)));
	    ++pos_raw;
	  }
	  else{
	    volumeData[pos] = 0;
	  }
	  ++pos;
	}
      }
    }
    
    delete [] volumeData_raw;
  }
  else if(g_volume_info->bits_per_channel == 32){
    std::cerr << "using unsigned int" << std::endl;
    FILE* volfile = fopen(volume_file.c_str(),"rb");
  
    if(!volfile) std::cerr << "error opening volume file" << std::endl;
    
    fseek (volfile , 0 , SEEK_END);
    long volFileSize = ftell (volfile);
    unsigned int* volumeData_raw = new unsigned int [volFileSize] ;
    
    rewind (volfile);
    long nbr = fread (volumeData_raw,g_volume_info->bits_per_channel/8,volFileSize,volfile);
    if(nbr != volFileSize) std::cerr << "error opening volume file" << std::endl;
    fclose(volfile);
    
    unsigned int pos_raw = 0;
    unsigned int pos = 0;
    for(unsigned int z = 0; z < g_volumeSize; ++z){
      for(unsigned int y = 0; y < g_volumeSize; ++y){
	for(unsigned int x = 0; x < g_volumeSize; ++x){
	  if( x >= x_start && x < x_end &&
	      y >= y_start && y < y_end &&
	      z >= z_start && z < z_end){
	    volumeData[pos] = (unsigned char)  (255 *  (((float) volumeData_raw[pos_raw]) / (1 << g_volume_info->bits_per_channel)));
	    ++pos_raw;
	  }
	  else{
	    volumeData[pos] = 0;
	  }
	  ++pos;
	}
      }
    }
    
    delete [] volumeData_raw;
  }
















  gloost::Texture* volumeTexture = new gloost::Texture(g_volumeSize,
                                                       g_volumeSize,
                                                       g_volumeSize,
                                                       volumeData,
                                                       GL_TEXTURE_3D,
                                                       GL_ALPHA,
                                                       GL_ALPHA,
                                                       GL_UNSIGNED_BYTE);

  volumeTexture->setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP);
  volumeTexture->setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP);
  volumeTexture->setTexParameter(GL_TEXTURE_WRAP_R, GL_CLAMP);
  volumeTexture->enableTextureMatrix();

  g_texManager->dropReference(g_volumeTextureId);
  g_texManager->cleanUp();
  g_volumeTextureId = g_texManager->addTexture(volumeTexture);
  // depreacted
  //volumeTexture->generate();
  volumeTexture->bind();

  delete [] volumeData;
  g_uniforms->set_sampler3D("volume", g_volumeTextureId);
  return true;
}

///////////////////////////////////////////////////////////////////////////////
std::string get_floating_param_string(){
  std::string floating_param(gloost::toString(g_floating_param_strength));
  unsigned int end(4 < floating_param.size() ? 4 : floating_param.size());
  floating_param = "parameter:      " + floating_param.substr(0,end);
  return floating_param;
}

std::string get_oversampling_string(){
  std::string oversampling(gloost::toString(g_oversampling));
  unsigned int end(4 < oversampling.size() ? 4 : oversampling.size());
  oversampling = "osa:      " + oversampling.substr(0,end);
  return oversampling;
}


void set_osa(){
  g_num_samples = (unsigned int) (std::sqrt(3)  * g_oversampling * g_volumeSize);
}
void on_stop_navigate(int index){
  set_osa();
}


void on_left_button_up_browse_volumes(int index){
  for(unsigned int i = 0; i < g_volume_files_buttons.size(); ++i){
    g_volume_files_buttons[i]->toggle_active();
  }
}

void on_left_button_up_volume(int index){
  for(unsigned int i = 0; i < g_volume_files_buttons.size(); ++i){
    g_volume_files_buttons[i]->set_active(false);
  }
  load_volume(g_volume_files->get_entries()[index]);
  
  set_osa();
}


void on_left_button_up_browse_shaders(int index){
  for(unsigned int i = 0; i < g_shader_files_buttons.size(); ++i){
    g_shader_files_buttons[i]->toggle_active();
  }
}

void on_left_button_up_shader(int index){
  for(unsigned int i = 0; i < g_shader_files_buttons.size(); ++i){
    g_shader_files_buttons[i]->set_active(false);
  }

  g_raycast_direct = (g_shader_fs_files->get_entries()[index] != "../src//3_volume_shader_iso-surface_shading_warp.fs");
  if(g_raycast_direct)
    load_shader(g_shader_vs_files->get_entries()[index],g_shader_fs_files->get_entries()[index]);
  g_shader_info = index;
  set_osa();
}


void on_left_button_up_browse_transferFunctions(int index){
  for(unsigned int i = 0; i < g_transferFunction_files_buttons.size(); ++i){
    g_transferFunction_files_buttons[i]->toggle_active();
  }
}

void on_left_button_up_transferFunction(int index){
  for(unsigned int i = 0; i < g_transferFunction_files_buttons.size(); ++i){
    g_transferFunction_files_buttons[i]->set_active(false);
  }
  load_tf(g_transferFunction_files->get_entries()[index]);
  set_osa();
}

void on_floating_param(int index){
  if(index){
    g_floating_param_strength *= 1.05;
  }
  else{
    g_floating_param_strength *= 0.95;
  }
  set_osa();
  g_floating_param_info->set_text(get_floating_param_string().c_str());
}

void on_oversampling(int index){
  if(index){
    g_oversampling *= 1.05;
  }
  else{
    g_oversampling *= 0.95;
  }
  set_osa();
  g_oversampling_info->set_text(get_oversampling_string().c_str());
}

void on_exit(int exit_num){
  delete g_gui;
  delete g_envelope_alpha;
  delete g_envelope_red;
  delete g_envelope_green;
  delete g_envelope_blue;
  delete g_transferfunction;
  delete g_volume_files;
  delete g_shader_vs_files;
  delete g_shader_fs_files; 

  exit(exit_num);
}

void on_light_change(int index){

  gloost::Matrix light_rot(g_trackball_light->get_rot_matrix());
  gloost::Matrix cam_rot(g_trackball->get_rot_matrix());
  gloost::Matrix cam_rot_inv(cam_rot);
  cam_rot_inv.invert();
  
  g_light_pos = cam_rot_inv * light_rot * cam_rot * g_light_start_pos;
  
}


void on_clipping_plane_change(int index){
  
  gloost::Matrix clipping_rot(g_trackball_clipping_plane->get_rot_matrix());
  gloost::Matrix cam_rot(g_trackball->get_rot_matrix());
  gloost::Matrix cam_rot_inv(cam_rot);
  cam_rot_inv.invert();
  
  g_clipping_plane_normal = cam_rot_inv * clipping_rot * cam_rot * g_clipping_plane_start_normal;
  
}

void on_reset_floating_param_info(int index){
  g_floating_param_strength = g_default_floating_param_strength;
  g_floating_param_info->set_text(get_floating_param_string().c_str());
  set_osa();
}

void on_reset_oversampling_info(int index){
  g_oversampling = g_default_oversampling;
  g_oversampling_info->set_text(get_oversampling_string().c_str());
  set_osa();
}

void on_toggle_interaction(int index){
  if(index == 0){
    g_trackball->toggle_active();
    g_trackball_light->toggle_active();

    g_camera_button->toggle_active();
    g_light_button->toggle_active();
    
    g_draw_light = !g_draw_light;
    if(g_draw_light){
      g_trackball_light->reset_rot_matrix();
      g_light_start_pos = g_light_pos;
    }
  }

  if(index == 1){
    g_trackball->toggle_active();
    g_trackball_clipping_plane->toggle_active();
    /*
    g_camera_button->toggle_active();
    g_light_button->toggle_active();
    */

    g_draw_clipping_plane = !g_draw_clipping_plane;
    if(g_draw_clipping_plane){
      g_trackball_clipping_plane->reset_rot_matrix();
      g_clipping_plane_start_normal = g_clipping_plane_normal;
    }
  }


    set_osa();
}


///////////////////////////////////////////////////////////////////////////////
void reshape(int w, int h){
  g_screenWidth = w;
  g_screenHeight = h;
  g_renderWidth = g_screenWidth - g_hSpace;
  g_renderHeight = g_screenHeight;
  g_renderAspect = ((float) g_renderWidth)/g_renderHeight;  


  g_trackball->set_bounds(0,g_renderWidth,0,g_renderHeight);
  g_trackball_light->set_bounds(0,g_renderWidth,0,g_renderHeight);
  g_trackball_clipping_plane->set_bounds(0,g_renderWidth,0,g_renderHeight);

  g_gui->set_bounds(0,g_screenWidth,0,g_screenHeight);

  g_flip_button->set_pos(g_screenWidth - 36 - 50, 36);
  g_clipping_button->set_pos(g_screenWidth - 36, 36);
  g_camera_button->set_pos(g_screenWidth - 36, g_screenHeight);
  g_light_button->set_pos(g_screenWidth - 36, g_screenHeight);

  g_floating_param_info->set_pos(g_screenWidth - 302 - 60, 26);
  g_floating_param_button_plus->set_pos(g_screenWidth - 4*36 - 60, 36);
  g_floating_param_button_minus->set_pos(g_screenWidth - 5*36 - 60, 36);

  g_oversampling_info->set_pos(g_screenWidth - 215, g_screenHeight - 9);
  g_oversampling_button_plus->set_pos(g_screenWidth - 2*36 - 60, g_screenHeight);
  g_oversampling_button_minus->set_pos(g_screenWidth - 3*36 - 60, g_screenHeight);
  
  bottom_bg->set_pos(0, g_screenHeight);
  bottom_info->set_pos(10, g_screenHeight - 9);

  int distance = 160;
  g_envelope_blue->set_pos(g_screenWidth - g_hSpace + 40, 60 + 0*distance);
  g_envelope_green->set_pos(g_screenWidth - g_hSpace + 40, 60 + 1*distance);
  g_envelope_red->set_pos(g_screenWidth - g_hSpace + 40, 60 + 2*distance);
  g_envelope_alpha->set_pos(g_screenWidth - g_hSpace + 40, 60 + 3*distance);

  
}


void mouse(int button, int state, int x, int y){

  bool envelopes_active = true;
  envelopes_active = g_envelope_alpha->mouse(button, state, x, g_screenHeight - y);
  envelopes_active = g_envelope_red->mouse(button, state, x, g_screenHeight - y);
  envelopes_active = g_envelope_green->mouse(button, state, x, g_screenHeight - y);
  envelopes_active = g_envelope_blue->mouse(button, state, x, g_screenHeight - y);


  if(!envelopes_active)
    g_gui->mouse(button,state,x,y);

  glutPostRedisplay();
}

void motion(int x, int y){
  bool envelopes_active = true;
  envelopes_active = g_envelope_alpha->motion(x,  g_screenHeight - y);
  envelopes_active = g_envelope_red->motion(x,  g_screenHeight - y);
  envelopes_active = g_envelope_green->motion(x,  g_screenHeight - y);
  envelopes_active = g_envelope_blue->motion(x,  g_screenHeight - y);

  if(!envelopes_active)
    g_gui->motion(x,y);

  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
  switch (key) {
  case 'q':
  case 27:
    on_exit(EXIT_SUCCESS);
  case ' ':
    on_toggle_interaction(0);
    break;
  case 'c':
    on_toggle_interaction(1);
    break;
  case 'r':
    load_volume(g_current_volume_file);
    break;
  case 'a':
    g_animate = !g_animate;
    break;
  default:
    break;
  }
 glutPostRedisplay();
}

void idle(){
  glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////////


class line{
public:
  line(const gloost::Point3& O,const gloost::Vector3& D)
    : o(O), d(D){}
  gloost::Point3 o;
  gloost::Vector3 d;
};


float intersect_ray_plane(gloost::Point3 ray_o, gloost::Vector3 ray_d,
			  gloost::Point3 clip_o, gloost::Vector3 clip_n){
  const float clipping_plane_distance_to_origin = clip_n*gloost::Vector3(clip_o);
  return -1.0 * ( clip_n *  gloost::Vector3(ray_o) - clipping_plane_distance_to_origin)/(clip_n *  ray_d);
}


class mysorter {

public:
  mysorter(const gloost::Vector3& X,const gloost::Vector3& Y,const gloost::Vector3& Z, const gloost::Point3& O)
    : x(X), y(Y), z(Z), o(O){}
  
  bool operator()(const gloost::Point3& a, const gloost::Point3& b) {
    gloost::Vector3 va(a-o);
    va.normalize();
    gloost::Vector3 vb(b-o);
    vb.normalize();



    return (va * x) < (vb * x);
  }

  gloost::Vector3 x;
  gloost::Vector3 y;
  gloost::Vector3 z;
  gloost::Point3 o;

};



std::vector<gloost::Point3> calc_clipping_plane_volume_intersection(const gloost::Point3& clipping_pos,
								    const gloost::Vector3& clipping_normal){


  std::vector<line> lines;
  // bottom
  lines.push_back(line(gloost::Point3(0.0,0.0,0.0),gloost::Vector3(0.0,0.0,1.0)));
  lines.push_back(line(gloost::Point3(0.0,0.0,1.0),gloost::Vector3(1.0,0.0,0.0)));
  lines.push_back(line(gloost::Point3(1.0,0.0,1.0),gloost::Vector3(0.0,0.0,-1.0)));
  lines.push_back(line(gloost::Point3(1.0,0.0,0.0),gloost::Vector3(-1.0,0.0,0.0)));

  // vertical
  lines.push_back(line(gloost::Point3(0.0,0.0,0.0),gloost::Vector3(0.0,1.0,0.0)));
  lines.push_back(line(gloost::Point3(1.0,0.0,0.0),gloost::Vector3(0.0,1.0,0.0)));
  lines.push_back(line(gloost::Point3(1.0,0.0,1.0),gloost::Vector3(0.0,1.0,0.0)));
  lines.push_back(line(gloost::Point3(0.0,0.0,1.0),gloost::Vector3(0.0,1.0,0.0)));

  // top
  lines.push_back(line(gloost::Point3(0.0,1.0,0.0),gloost::Vector3(0.0,0.0,1.0)));
  lines.push_back(line(gloost::Point3(0.0,1.0,1.0),gloost::Vector3(1.0,0.0,0.0)));
  lines.push_back(line(gloost::Point3(1.0,1.0,1.0),gloost::Vector3(0.0,0.0,-1.0)));
  lines.push_back(line(gloost::Point3(1.0,1.0,0.0),gloost::Vector3(-1.0,0.0,0.0)));


  std::vector<gloost::Point3> i_points;

  for(unsigned int i = 0; i < lines.size(); ++i){
    float lambda = intersect_ray_plane(lines[i].o, lines[i].d,
				       clipping_pos, clipping_normal);
    if(lambda > 0.0 && lambda < 1.0){
      i_points.push_back(lines[i].o + lines[i].d.scalar(lambda));
    }
    

  }

  gloost::Vector3 plane_z = clipping_normal;
  plane_z.normalize();

  gloost::Vector3 plane_x = i_points[1] - i_points[0];
  
  plane_x.normalize();

  gloost::Vector3 plane_y = cross(plane_x,plane_z);
  plane_y.normalize();

  mysorter ms(plane_x, plane_y, plane_z, i_points[0]);
  std::sort(i_points.begin(), i_points.end(),ms);
  


  return i_points;
}



///////////////////////////////////////////////////////////////////////////////

void raycast_direct(){

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(g_fov, g_renderAspect, g_near, g_far);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gloost::Matrix model_view(g_trackball->get_matrix());
  gloostMultMatrix(model_view.data());

  model_view.invert();

  gloost::Point3 eye_pos(0.0,0.0,0.0);
  eye_pos = model_view * eye_pos;
  gloost::vec3 ray_origin(eye_pos[0],eye_pos[1],eye_pos[2]);
  g_uniforms->set_vec3("ray_origin",ray_origin);
    

  g_uniforms->set_float("sample_distance",1.0f/g_num_samples);
  g_uniforms->set_float("inv_sample_distance",(float) g_num_samples);
  g_uniforms->set_float("floating_param_strength",g_floating_param_strength);

  // this should be handled by a seperate variable (instead of g_oversampling)
  g_uniforms->set_float("jitter_strength",g_oversampling);
  g_uniforms->set_float("grid_step_right",1.0f / g_volumeSize);
  g_uniforms->set_float("grid_step_left",-1.0f / g_volumeSize);




  // here we pass the clipping plane to the shader
  gloost::vec3 clipping_plane_pos(g_clipping_plane_pos[0],
				  g_clipping_plane_pos[1],
				  g_clipping_plane_pos[2]);
  g_uniforms->set_vec3("clipping_plane_pos",clipping_plane_pos);


  gloost::vec3 clipping_plane_normal(g_clipping_plane_normal[0],
				     g_clipping_plane_normal[1],
				     g_clipping_plane_normal[2]);
  g_uniforms->set_vec3("clipping_plane_normal",clipping_plane_normal);

  const gloost::Vector3 tmp_pos(g_clipping_plane_pos);
  const float clipping_plane_distance_to_origin(tmp_pos * g_clipping_plane_normal);
  g_uniforms->set_float("clipping_plane_distance_to_origin",clipping_plane_distance_to_origin);
  g_uniforms->set_float("use_clipping_plane",(float) (g_use_clipping_plane == true? 1.0 : 0.0));
  


  gloost::Point3 l_pos_global(g_light_pos);
  gloost::vec3 l_pos(l_pos_global[0],l_pos_global[1],l_pos_global[2]);



  g_uniforms->set_vec3("light_pos",l_pos);
  light->setPosition(l_pos_global);
  light->set(GL_LIGHT0);



  glPushAttrib(GL_ALL_ATTRIB_BITS);
  {
    
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    //glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    g_shader->set();
    g_uniforms->applyToShader(g_shader->getShaderHandle());
    gee::drawProxy(0.0,0.0,0.0,
		   1.0,1.0,1.0,true);
    
    g_shader->disable();
  }
  glPopAttrib();

}

void raycast_warp(){

}

///////////////////////////////////////////////////////////////////////////////

void display(){
#ifdef LINUX
  g_timer.step();
  bottom_info->set_text((gloost::toString(g_timer.get_fps()) + " fps | " +  g_volume_info->toString() + " | " + g_vec_shader_info[g_shader_info]).c_str());
#else
  bottom_info->set_text((gloost::toString(0) + " fps | " +  g_volume_info->toString() + " | " + g_vec_shader_info[g_shader_info]).c_str());
#endif
  glViewport(0, 0, g_screenWidth, g_screenHeight);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, g_renderWidth, g_renderHeight);



  if(g_raycast_direct){
    raycast_direct();
  }
  else{
    raycast_warp();
  }



  if(g_draw_clipping_plane || g_use_clipping_plane ){
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    {
      //glEnable(GL_DEPTH_TEST);

      glDisable(GL_LIGHTING);

#if 0
      glColor4f(0.0f,1.0f,0.0f,0.15f);
      glDisable(GL_CULL_FACE);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      std::vector<gloost::Point3> ipoints = calc_clipping_plane_volume_intersection(g_clipping_plane_pos,
										    g_clipping_plane_normal);
      glBegin(GL_POLYGON);
      for(unsigned int i = 0; i < ipoints.size(); ++i){
	glVertex3f(ipoints[i][0],ipoints[i][1],ipoints[i][2]);
      }
      glEnd();
#endif

      glColor4f(1.0f,0.0f,0.0f,1.0f);
      glBegin(GL_LINES);
      glVertex3f(g_clipping_plane_pos[0],g_clipping_plane_pos[1],g_clipping_plane_pos[2]);
      glVertex3f(g_clipping_plane_pos[0] + g_clipping_plane_normal[0],
		 g_clipping_plane_pos[1] + g_clipping_plane_normal[1],
		 g_clipping_plane_pos[2] + g_clipping_plane_normal[2]);
      glEnd();
      
      
    }
    glPopAttrib();
  }





  if(g_draw_light){
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    {
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_LIGHTING);
      glColor3f(1.0f,1.0f,0.0f);
      glBegin(GL_LINES);
      glVertex3f(0.5,0.5,0.5);
      glVertex3f(g_light_pos[0],g_light_pos[1],g_light_pos[2]);
      glEnd();
      glTranslated(g_light_pos[0],g_light_pos[1],g_light_pos[2]);
      glutSolidSphere(0.03,16,16);
    }
    glPopAttrib();
  }

  g_envelope_alpha->draw_keys(g_screenWidth, g_screenHeight);
  g_envelope_red->draw_keys(g_screenWidth, g_screenHeight);
  g_envelope_green->draw_keys(g_screenWidth, g_screenHeight);
  g_envelope_blue->draw_keys(g_screenWidth, g_screenHeight);

  g_gui->draw(g_screenWidth, g_screenHeight);

  

  //g_num_samples = g_interactive_samples;
  
  
  glutSwapBuffers();
  
  if(g_volume_recording_files){
    if(g_animate){
      ++g_frameCounter;
      const unsigned rec_index = g_frameCounter % g_volume_recording_files->get_entries().size();
      load_volume(g_volume_recording_files->get_entries()[rec_index]);
    }
  }
}

void flip(int index){
  g_clipping_plane_normal = g_clipping_plane_normal.scalar(-1.0);
}

void on_clipping_toggle(int index){
  g_use_clipping_plane = ! g_use_clipping_plane;
  g_flip_button->toggle_active();
}




int main(int argc, char *argv[])
{
  sys::CMDParser p("<directory_to_volume_files> <directory_to_shader_files> <directory_to_transferfunction_files>");
  p.addOpt("a", 1,"antialiasing","specify the __GL_FSAA_MODE parameter");
  p.addOpt("r", 1,"recording","specify directory to recording");
  p.init(argc,argv);

  if(p.isOptSet("a")){
    setenv("__GL_FSAA_MODE",gloost::toString(p.getOptsInt("a")[0]).c_str(),true);
  }


  if(p.isOptSet("r")){
    g_animate = true;
    g_volume_recording_files = new sys::Directory(p.getOptsString("r")[0]);
    g_volume_recording_files->open(".raw");
  }


  setenv("__GL_SYNC_TO_VBLANK","1",true);  

  
  

  
  g_volume_files = new sys::Directory(p.getArgs()[0]);
  g_volume_files->open(".raw");

  g_shader_vs_files = new sys::Directory(p.getArgs()[1]);
  g_shader_vs_files->open(".vs");

  g_shader_fs_files = new sys::Directory(p.getArgs()[1]);
  g_shader_fs_files->open(".fs");

  g_transferFunction_files = new sys::Directory(p.getArgs()[2]);
  g_transferFunction_files->open(".tf");

  if(!g_volume_files->size()){
    std::cerr << "No Volume Files found...exiting" << std::endl;
    on_exit(EXIT_SUCCESS);
  }
  std::string volume_file = g_volume_files->get_entries()[0];
  

  if(g_shader_vs_files->size() != g_shader_vs_files->size() || !g_shader_vs_files->size()){
    std::cerr << "No Shader Files found...exiting" << std::endl;
    on_exit(EXIT_SUCCESS);
  }
  std::string vs_file = g_shader_vs_files->get_entries()[0];
  std::string fs_file = g_shader_fs_files->get_entries()[0];
 

  if(!g_transferFunction_files->size()){
    std::cerr << "No Transferfunctions found...exiting" << std::endl;
    on_exit(EXIT_SUCCESS);
  }
  std::string transferFunction_file = g_transferFunction_files->get_entries()[0];



  setenv("__GL_SYNC_TO_VBLANK","1",true);


  glutInit(&argc, argv);
  glutInitWindowPosition(50,50);
  glutInitWindowSize(g_screenWidth, g_screenHeight);
  glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

  glutCreateWindow(argv[0]);

  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  
  glutIdleFunc(idle);
  

  // Init glew
  if (GLEW_OK != glewInit())
  {
    std::cerr << "'glewInit()' failed." << std::endl;
    exit(0);
  }





  g_texManager = gloost::TextureManager::getInstance();

  load_shader(vs_file,fs_file);
  
  g_uniforms   = new gloost::UniformSet;


  genJitterTexture();
  gloost::Texture* jtTexture = new gloost::Texture(256,
						   256,
						   1,
						   g_jitterTextureBuffer,
						   GL_TEXTURE_2D,
						   GL_RGB,
						   GL_RGB,
						   GL_UNSIGNED_BYTE);
  g_jitterFunctionTextureId = g_texManager->addTexture(jtTexture);
  g_uniforms->set_sampler2D("jitterfunction", g_jitterFunctionTextureId);


  light = new gloost::PointLight;
  light->setDiffuse(gloost::Point3(0.7, 0.7, 0.7));
  light->setSpecular(gloost::Point3(1.0, 1.0, 1.0));



  load_volume(volume_file);

  

  g_transferfunction = new Transferfunction;
  g_transferfunction->load(transferFunction_file.c_str());

  g_envelope_alpha = new gee::gee_envelope2("../data/images/grid_alpha.png",300,130);
  g_envelope_alpha->set_cb_generate_tf(on_update_tf, 0);
  g_envelope_alpha->set_cb_on_stop_edit_tf(on_stop_navigate,0);

  g_envelope_red = new gee::gee_envelope2("../data/images/grid_red.png",300,130);
  g_envelope_red->set_cb_generate_tf(on_update_tf, 1);
  g_envelope_red->set_cb_on_stop_edit_tf(on_stop_navigate,0);

  g_envelope_green = new gee::gee_envelope2("../data/images/grid_green.png",300,130);
  g_envelope_green->set_cb_generate_tf(on_update_tf, 2);
  g_envelope_green->set_cb_on_stop_edit_tf(on_stop_navigate,0);

  g_envelope_blue = new gee::gee_envelope2("../data/images/grid_blue.png",300,130);
  g_envelope_blue->set_cb_generate_tf(on_update_tf, 3);
  g_envelope_blue->set_cb_on_stop_edit_tf(on_stop_navigate,0);


  load_tf(transferFunction_file);





  // create the g_gui  
  g_gui = new gee::gee_element();


  top_bg = new gee::gee_surface("../data/images/top.png");
  top_bg->set_pos(0,36);
  g_gui->add_child(top_bg);
  bottom_bg = new gee::gee_surface("../data/images/bottom.png");
  g_gui->add_child(bottom_bg);







  bottom_info = new gee::gee_textfield(g_volume_info->toString().c_str());
  g_gui->add_child(bottom_info);







  g_flip_button = new gee::gee_surface_button("../data/images/flip_button.png",
					    "../data/images/flip_button_active.png");
  g_flip_button->set_cb_left_mouse_down(flip,0);
  g_flip_button->set_active(false);
  g_gui->add_child(g_flip_button);

  g_clipping_button = new gee::gee_surface_button("../data/images/clipping_button.png",
						  "../data/images/clipping_button_active.png");
  g_clipping_button->set_cb_left_mouse_down(on_clipping_toggle,0);
  g_gui->add_child(g_clipping_button);






  g_camera_button = new gee::gee_surface_button("../data/images/camera.png",
					    "../data/images/camera_active.png");
  g_camera_button->set_cb_left_mouse_up(on_toggle_interaction,0);
  g_camera_button->set_active(false);
  g_gui->add_child(g_camera_button);

  g_light_button = new gee::gee_surface_button("../data/images/bulb.png",
					    "../data/images/bulb_active.png");
  g_light_button->set_cb_left_mouse_up(on_toggle_interaction,0);
  g_gui->add_child(g_light_button);












  g_floating_param_button_plus = new gee::gee_surface_button("../data/images/plus.png",
							"../data/images/plus_active.png");
  g_floating_param_button_plus->set_cb_left_mouse_up(on_floating_param,1);
  g_gui->add_child(g_floating_param_button_plus);
  g_floating_param_button_minus = new gee::gee_surface_button("../data/images/minus.png",
							      "../data/images/minus_active.png");
  g_floating_param_button_minus->set_cb_left_mouse_up(on_floating_param,0);
  g_gui->add_child(g_floating_param_button_minus);
  g_floating_param_info = new gee::gee_textfield_button(get_floating_param_string().c_str());
  g_floating_param_info->set_cb_left_mouse_up(on_reset_floating_param_info,0);
  g_gui->add_child(g_floating_param_info);


  g_oversampling_button_plus = new gee::gee_surface_button("../data/images/plus.png",
							"../data/images/plus_active.png");
  g_oversampling_button_plus->set_cb_left_mouse_up(on_oversampling,1);
  g_gui->add_child(g_oversampling_button_plus);
  g_oversampling_button_minus = new gee::gee_surface_button("../data/images/minus.png",
							      "../data/images/minus_active.png");
  g_oversampling_button_minus->set_cb_left_mouse_up(on_oversampling,0);
  g_gui->add_child(g_oversampling_button_minus);
  g_oversampling_info = new gee::gee_textfield_button(get_oversampling_string().c_str());
  g_oversampling_info->set_cb_left_mouse_up(on_reset_oversampling_info,0);
  g_gui->add_child(g_oversampling_info);




  browse_volumes_button = new gee::gee_textfield_button("Volume");
  browse_volumes_button->set_pos(10,26);
  browse_volumes_button->set_cb_left_mouse_up(on_left_button_up_browse_volumes,0);
  g_gui->add_child(browse_volumes_button);



  const std::vector<std::string> volume_files = g_volume_files->get_entries();
  for(unsigned int i = 0; i < volume_files.size(); ++i){
    std::string tmp_name(gee::basename(volume_files[i].c_str()).c_str());
    sys::StringTokenizer tmp_name_tk(tmp_name.c_str());
    tmp_name = tmp_name_tk.get_token_string("_", " ", 1, 0);
    tmp_name = tmp_name.substr(0,tmp_name.size() - 4);
    gee::gee_textfield_button* tb = new gee::gee_textfield_button(tmp_name.c_str());
    tb->set_pos(10,(int) (60 + 20*i));
    tb->set_active(false);
    tb->set_cb_left_mouse_up(on_left_button_up_volume,i);
    g_volume_files_buttons.push_back(tb);
    g_gui->add_child(tb);
  }


  browse_shaders_button = new gee::gee_textfield_button("Shader");
  browse_shaders_button->set_pos(100,26);
  browse_shaders_button->set_cb_left_mouse_up(on_left_button_up_browse_shaders,0);
  g_gui->add_child(browse_shaders_button);

  const std::vector<std::string> shader_files = g_shader_fs_files->get_entries();
  for(unsigned int i = 0; i < shader_files.size(); ++i){
    std::string tmp_name(gee::basename(shader_files[i].c_str()).c_str());
    sys::StringTokenizer tmp_name_tk(tmp_name.c_str());
    tmp_name = tmp_name_tk.get_token_string("_", " ", 3, 0);
    tmp_name = tmp_name.substr(0,tmp_name.size() - 3);
    g_vec_shader_info.push_back(tmp_name);
    gee::gee_textfield_button* tb = new gee::gee_textfield_button(tmp_name.c_str());
    tb->set_pos(100,(int) (60 + 20*i));
    tb->set_active(false);
    tb->set_cb_left_mouse_up(on_left_button_up_shader,i);
    g_shader_files_buttons.push_back(tb);
    g_gui->add_child(tb);
  }

  browse_transferFunctions_button = new gee::gee_textfield_button("TransferFunction");
  browse_transferFunctions_button->set_pos(190,26);
  browse_transferFunctions_button->set_cb_left_mouse_up(on_left_button_up_browse_transferFunctions,0);
  g_gui->add_child(browse_transferFunctions_button);

  const std::vector<std::string> transferFunction_files = g_transferFunction_files->get_entries();
  for(unsigned int i = 0; i < transferFunction_files.size(); ++i){
    std::string tmp_name(gee::basename(transferFunction_files[i].c_str()).c_str());
    sys::StringTokenizer tmp_name_tk(tmp_name.c_str());
    tmp_name = tmp_name_tk.get_token_string("_", " ", 1, 0);
    tmp_name = tmp_name.substr(0,tmp_name.size() - 3);
    g_vec_shader_info.push_back(tmp_name);
    gee::gee_textfield_button* tb = new gee::gee_textfield_button(tmp_name.c_str());
    tb->set_pos(190,(int) (60 + 20*i));
    tb->set_active(false);
    tb->set_cb_left_mouse_up(on_left_button_up_transferFunction,i);
    g_transferFunction_files_buttons.push_back(tb);
    g_gui->add_child(tb);
  }

  gloost::Point3 center(-0.5,-0.5,-0.5);
  

  g_trackball = new gee::gee_trackball(5.0, 0.05);
  g_trackball->setLookAt(center);
  g_trackball->set_cb_stop(on_stop_navigate, 0);
  g_gui->add_child(g_trackball);

  g_trackball_light = new gee::gee_trackball(3.0, 0.05);
  g_trackball_light->setLookAt(center);
  g_trackball_light->set_cb_stop(on_stop_navigate, 1);
  g_trackball_light->set_cb_drag(on_light_change, 1);
  g_trackball_light->set_active(false);
  g_gui->add_child(g_trackball_light);

  g_trackball_clipping_plane = new gee::gee_trackball(3.0, 0.05);
  g_trackball_clipping_plane->setLookAt(center);
  g_trackball_clipping_plane->set_cb_stop(on_stop_navigate, 1);
  g_trackball_clipping_plane->set_cb_drag(on_clipping_plane_change, 1);
  g_trackball_clipping_plane->set_active(false);
  g_gui->add_child(g_trackball_clipping_plane);


  glutMainLoop();

  return EXIT_SUCCESS;
}
