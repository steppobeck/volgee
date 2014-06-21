#include "gee_trackball.h"
#include <GL/glut.h>

#include <Matrix.h>
#include <Vector3.h>


#define NONE 0
#define ROTATE 1
#define ZOOM 2
#define PAN 3

namespace gee{

    
  gee_trackball::gee_trackball(floating_type zoom, float zoom_step)
    : m_arcball(new gl::ArcBall),
      m_zoom(-zoom),
      m_zoom_step(zoom_step),
      m_mat(),
      m_mode(NONE),
      m_center(),
      m_arcball_center(math::vec3(0.0,0.0,0.0)),
      m_cb_start(0),
      m_cb_stop(0),
      m_cb_drag(0),
      m_index_cb_start(0),
      m_index_cb_stop(0),
      m_index_cb_drag(0)
  {    
    m_arcball->place(m_arcball_center);
  }
  
  gee_trackball::~gee_trackball(){
    delete m_arcball;
  }

  bool
  gee_trackball::mouse(int button, int state, int x, int y){

    if (button == GLUT_LEFT_BUTTON) {
      m_mode = ROTATE;
      m_arcball->set_cur(x, y);
      
      if (state == GLUT_DOWN) {
	m_arcball->begin_drag();
      } else {
	m_arcball->end_drag();
      }
      
    }
    else if (button == GLUT_RIGHT_BUTTON) {
      m_mode = ZOOM;
    }
    else if(button == GLUT_MIDDLE_BUTTON){
      m_mode = PAN;
    }

    if(state == GLUT_DOWN){
      if(m_cb_start)
	m_cb_start(m_index_cb_start);
    }
    else{
      if(m_cb_stop)
	m_cb_stop(m_index_cb_stop);
    }

    return false;
  }

  bool
  gee_trackball::motion(int x, int y){
    switch (m_mode){
    case ROTATE:
      m_arcball->set_cur(x, y);
      if(m_cb_drag)
	m_cb_drag(m_index_cb_drag);
      break;
    case ZOOM:
      m_zoom += m_zoom_step * (x - s_last_mouse_x);
      break;
    case PAN:
      {
	
      }
      break;

    default:
      break;

    }
    return false;
  }
  
  void
  gee_trackball::set_bounds(int min_x, int max_x, int min_y, int max_y){
    gee_element::set_bounds(min_x,max_x,min_y,max_y);
    m_arcball->set_win_size(m_bounds[1],m_bounds[3]);
    
  }

  void
  gee_trackball::draw(int viewport_width, int viewport_height){

  }

  const gloost::Matrix& 
  gee_trackball::get_matrix(){

    
    m_mat.setIdentity();
    m_mat.setTranslate(0.0,0.0,m_zoom);

    gloost::Matrix tmp;
    tmp.setIdentity();
    const float* rotmat = m_arcball->get();
    for(unsigned int i = 0; i != 16; ++i){
      tmp[i] = rotmat[i];
    }

    gloost::Matrix tmp2;
    tmp2.setIdentity();
    tmp2.setTranslate(m_center[0],m_center[1],m_center[2]);
    m_mat = m_mat * tmp * tmp2;

    return m_mat;
  }


  const gloost::Matrix&
  gee_trackball::get_rot_matrix(){
    
    gloost::Matrix tmp;
    tmp.setIdentity();
    const float* rotmat = m_arcball->get();
    for(unsigned int i = 0; i != 16; ++i){
      tmp[i] = rotmat[i];
    }
    m_mat = tmp;
    return m_mat;
  }

  void
  gee_trackball::reset_rot_matrix(){
    delete m_arcball;
    m_arcball = new gl::ArcBall;
    m_arcball->place(m_arcball_center);
    m_arcball->set_win_size(m_bounds[1],m_bounds[3]);
  }


  float
  gee_trackball::get_zoom() const{
     return m_zoom;
  }

  void
  gee_trackball::setLookAt(gloost::Point3& center){
    m_center = center;
  }

  void
  gee_trackball::set_cb_start(void (* callback)( int ), int index){
    m_cb_start = callback;
    m_index_cb_start = index;
  }
  void gee_trackball::set_cb_stop(void (* callback)( int ), int index){
    m_cb_stop = callback;
    m_index_cb_stop = index;
  }
  void gee_trackball::set_cb_drag(void (* callback)( int ), int index){
    m_cb_drag = callback;
    m_index_cb_drag = index;
  }


}
