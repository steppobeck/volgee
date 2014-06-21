#include "gee_dragger.h"

#include <GL/glut.h>

#include <iostream>

namespace gee{
   
  gee_dragger::gee_dragger(const char* filename)
    : gee_surface(filename),
      m_draggies(),
      m_dragging(false)
  {}

  gee_dragger::~gee_dragger()
  {}

  bool
  gee_dragger::mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON){
      if(state == GLUT_DOWN){
	m_dragging = true;
	m_working = true;
      }
      else{
	m_dragging = false;
	m_working = false;
      }
    }
    return false;
  }

  bool
  gee_dragger::motion(int x, int y){

    if(m_dragging){
      const int d_x(x - s_last_mouse_x);
      const int d_y(y - s_last_mouse_y);
      set_pos(m_pos[0] + d_x, m_pos[1] + d_y);
      for(unsigned int i = 0; i < m_draggies.size(); ++i){
	m_draggies[i]->set_pos(m_draggies[i]->get_pos()[0] + d_x, m_draggies[i]->get_pos()[1] + d_y);
      }

    }
    return false;
  }

  void
  gee_dragger::add_draggie(gee_element* draggie){
    m_draggies.push_back(draggie);
  }


  void
  gee_dragger::toggle_active(){
    m_active = !m_active;
    for(unsigned int i = 0; i < m_draggies.size(); ++i){
      m_draggies[i]->toggle_active();
    }
  }

}
