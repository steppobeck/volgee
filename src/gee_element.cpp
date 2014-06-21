#include "gee_element.h"

#include <GL/gl.h>
#include <iostream>

namespace gee{


  /* static */ int gee_element::s_last_mouse_x(0);
  /* static */ int gee_element::s_last_mouse_y(0);

  gee_element::gee_element()
    : m_bounds(),
      m_children(),
      m_pos(),
      m_active(true),
      m_working(false)
  {
    set_bounds(0,0,0,0);
    set_pos(0,0);
  }
  
  /*virtual*/ gee_element::~gee_element()
  {
    for(unsigned int i = 0; i < m_children.size(); ++i){
      delete m_children[i];
    }
  }

  bool
  gee_element::is_active() const{
    return m_active;
  }

  bool
  gee_element::is_working() const{
    return m_working;
  }

  /*virtual*/ void
  gee_element::toggle_active(){
    m_active = !m_active;
  }

  void
  gee_element::set_active(bool active){
    m_active = active;
  }


  /*virtual*/ bool
  gee_element::mouse(int button, int state, int x, int y){
    s_last_mouse_x = x;
    s_last_mouse_y = y;
#ifdef GEE_DEBUG
    std::cerr << "MESSAGE from virtual void gee_element::mouse(int button, int state, int x, int y)"
	      << std::endl
	      << "button: " << button << std::endl
	      << "state: " << state << std::endl
	      << "x: " << x << std::endl
	      << "y: " << y << std::endl;
#endif
    for(unsigned int i = 0; i < m_children.size(); ++i){
      if(m_children[i]->inside_bounds(x,y) && m_children[i]->is_active()){
	if(!m_children[i]->mouse(button,state,x,y))
	  i = m_children.size();
      }
    }

    return true;

  }
  
  /*virtual*/ bool
  gee_element::motion(int x, int y){
#ifdef GEE_DEBUG
    std::cerr << "MESSAGE from virtual void gee_element::motion(int x, int y)"
	      << std::endl
	      << "x: " << x << std::endl
	      << "y: " << y << std::endl;
#endif


    for(unsigned int i = 0; i < m_children.size(); ++i){
      if( (m_children[i]->inside_bounds(x,y) && m_children[i]->is_active()) || m_children[i]->is_working()){
	if(!m_children[i]->motion(x,y))
	  i = m_children.size();
      }
    }
    s_last_mouse_x = x;
    s_last_mouse_y = y;
    return true;

  }
  

  /*virtual*/ void
  gee_element::set_bounds(int min_x, int max_x, int min_y, int max_y){
    m_bounds[0] = min_x;
    m_bounds[1] = max_x;
    m_bounds[2] = min_y;
    m_bounds[3] = max_y;
  }


  void
  gee_element::add_child(gee_element* child){
    m_children.push_back(child);
  }

  /*virtual*/ void
  gee_element::set_pos(int x, int y){
    m_pos[0] = x;
    m_pos[1] = y;
  }

  const int*
  gee_element::get_pos() const{
    return m_pos;
  }

  /*virtual*/ void
  gee_element::draw(int viewport_width, int viewport_height){

#ifdef GEE_DEBUG
    std::cerr << "MESSAGE from virtual void gee_element::draw()"
	      << std::endl;
#endif


    
    glViewport(0,0,viewport_width,viewport_height);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    {
      glDisable(GL_DEPTH_TEST);
      
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0.0, (float) viewport_width, 0.0, (float) viewport_height, 0.1, 10.0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glTranslatef(0.0,0.0,-5.0);
      
    
    
      for(unsigned int i = 0; i < m_children.size(); ++i){
	if(m_children[i]->is_active())
	  m_children[i]->draw(viewport_width, viewport_height);
      }
    }
    glPopAttrib();

     
  }



  bool
  gee_element::inside_bounds(int x, int y) const{
    return (m_bounds[0] < x) &&
      (m_bounds[1] > x) &&
      (m_bounds[2] < y) &&
      (m_bounds[3] > y);
  }
  
}
