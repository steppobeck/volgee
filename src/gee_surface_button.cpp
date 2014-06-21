#include "gee_surface_button.h"
#include <TextureManager.h>
#include <GL/glut.h>
#include <iostream>


#define NONE 0
#define LEFT 1
#define MIDDLE 2
#define RIGHT 3


namespace gee{
    
  gee_surface_button::gee_surface_button(const char* filename, const char* filename_active)
    : gee_surface(filename),
      m_texture_active_id(0),
      m_active_button(NONE),
      m_cb_left_mouse_down(NULL),
      m_cb_middle_mouse_down(NULL),
      m_cb_right_mouse_down(NULL),
      m_cb_left_mouse_pressed(NULL),
      m_cb_middle_mouse_pressed(NULL),
      m_cb_right_mouse_pressed(NULL),
      m_cb_left_mouse_up(NULL),
      m_cb_middle_mouse_up(NULL),
      m_cb_right_mouse_up(NULL),
      m_index_cb_left_mouse_down(0),
      m_index_cb_middle_mouse_down(0),
      m_index_cb_right_mouse_down(0),
      
      m_index_cb_left_mouse_pressed(0),
      m_index_cb_middle_mouse_pressed(0),
      m_index_cb_right_mouse_pressed(0),
      
      m_index_cb_left_mouse_up(0),
      m_index_cb_middle_mouse_up(0),
      m_index_cb_right_mouse_up(0)

  {
    if(filename_active)
      m_texture_active_id = m_texture_manager->createTexture(filename_active);
  }

  gee_surface_button::~gee_surface_button()
  {}

    bool
  gee_surface_button::mouse(int button, int state, int x, int y){

    switch(button){
    case GLUT_LEFT_BUTTON:
      if(state == GLUT_DOWN){
	m_active_button = LEFT;
	if(m_cb_left_mouse_down){
	  m_cb_left_mouse_down(m_index_cb_left_mouse_down);
	}
      }
      else if (state == GLUT_UP){
	m_active_button = NONE;
	if(m_cb_left_mouse_up){
	  m_cb_left_mouse_up(m_index_cb_left_mouse_up);
	}
      }
      break;
    case GLUT_MIDDLE_BUTTON:
      if(state == GLUT_DOWN){
	m_active_button = MIDDLE;
	if(m_cb_middle_mouse_down){
	  m_cb_middle_mouse_down(m_index_cb_middle_mouse_down);
	}
      }
      else if (state == GLUT_UP){
	m_active_button = NONE;
	if(m_cb_middle_mouse_up){
	  m_cb_middle_mouse_up(m_index_cb_middle_mouse_up);
	}
      }
      break;
    case GLUT_RIGHT_BUTTON:
      if(state == GLUT_DOWN){
	m_active_button = RIGHT;
	if(m_cb_right_mouse_down){
	  m_cb_right_mouse_down(m_index_cb_right_mouse_down);
	}
      }
      else if (state == GLUT_UP){
	m_active_button = NONE;
	if(m_cb_right_mouse_up){
	  m_cb_right_mouse_up(m_index_cb_right_mouse_up);
	}
      }
      break;
    default:
      break;
    }

    return false;
  }

  bool
  gee_surface_button::motion(int x, int y){
    switch(m_active_button){
    case LEFT:
      if(m_cb_left_mouse_pressed){
	m_cb_left_mouse_pressed(m_index_cb_left_mouse_pressed);
      }
      break;
    case MIDDLE:
      if(m_cb_middle_mouse_pressed){
	m_cb_middle_mouse_pressed(m_index_cb_middle_mouse_pressed);
      }
      break;
    case RIGHT:
      if(m_cb_right_mouse_pressed){
	m_cb_right_mouse_pressed(m_index_cb_right_mouse_pressed);
      }
      break;
    default:
      break;
    }
    return false;
  }

  void
  gee_surface_button::draw(int viewport_width, int viewport_height){


    if(m_active_button && m_texture_active_id){
      const unsigned int tmp_id = m_texture_id;
      m_texture_id = m_texture_active_id;
      gee_surface::draw(viewport_width, viewport_height);
      m_texture_id = tmp_id;
    }
    else{
      gee_surface::draw(viewport_width, viewport_height);
    }
  }


  void
  gee_surface_button::set_cb_left_mouse_down(void (* callback)(int), int index){
    m_cb_left_mouse_down = callback;
    m_index_cb_left_mouse_down = index;
  }

  void
  gee_surface_button::set_cb_middle_mouse_down(void (* callback)(int), int index){
    m_cb_middle_mouse_down = callback;
    m_index_cb_middle_mouse_down = index;
  }
  
  void
  gee_surface_button::set_cb_right_mouse_down(void (* callback)(int), int index){
    m_cb_right_mouse_down = callback;
    m_index_cb_right_mouse_down = index;
  }

  void
  gee_surface_button::set_cb_left_mouse_pressed(void (* callback)(int), int index){
    m_cb_left_mouse_pressed= callback;
    m_index_cb_left_mouse_pressed = index;
  }
  
  void
  gee_surface_button::set_cb_middle_mouse_pressed(void (* callback)(int), int index){
    m_cb_middle_mouse_pressed = callback;
    m_index_cb_middle_mouse_pressed = index;
  }
  
  void
  gee_surface_button::set_cb_right_mouse_pressed(void (* callback)(int), int index){
    m_cb_right_mouse_pressed = callback;
    m_index_cb_right_mouse_pressed = index;
  }

  void
  gee_surface_button::set_cb_left_mouse_up(void (* callback)(int), int index){
    m_cb_left_mouse_up = callback;
    m_index_cb_left_mouse_up = index;
  }

  void
  gee_surface_button::set_cb_middle_mouse_up(void (* callback)(int), int index){
    m_cb_middle_mouse_up = callback;
    m_index_cb_middle_mouse_up = index;
  }

  void
  gee_surface_button::set_cb_right_mouse_up(void (* callback)(int), int index){
    m_cb_right_mouse_up = callback;
    m_index_cb_right_mouse_up = index;
  }


}


