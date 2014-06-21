#include "gee_textfield.h"
#include <TextureText.h>
#include <iostream>

namespace gee{


  /*static*/ gloost::TextureText* gee_textfield::s_textMaker = 0;

  gee_textfield::gee_textfield(const char* text)
    : m_text(text),
      m_alpha()
  {
    if(!s_textMaker){
      init();
    }
    m_alpha = s_textMaker->getCharAlpha();
  }
  
  gee_textfield::~gee_textfield()
  {
    if(s_textMaker)
      delete s_textMaker;
    s_textMaker = 0;
  }


  void
  gee_textfield::set_pos(int x, int y){
    gee_element::set_pos(x,y);
    calc_bounds();
  }

  void
  gee_textfield::set_text(const char* text){
    m_text = text;
    calc_bounds();
  }

  void
  gee_textfield::calc_bounds(){
    // calc new bounds
    const unsigned int size(m_text.size());
    const float cs(s_textMaker->getCharScale());
    const unsigned int width((unsigned int) (size * (s_textMaker->getCharWidth() + s_textMaker->getCharSpace()) * cs));
    const unsigned int height((unsigned int) (s_textMaker->getCharHeight() * cs));
    
    m_bounds[0] = m_pos[0];
    m_bounds[1] = m_pos[0] + width;
    m_bounds[2] = m_pos[1] - height;
    m_bounds[3] = m_pos[1];
  }

  void
  gee_textfield::draw(int viewport_width, int viewport_height){

    //std::cerr << m_bounds[0] << "," << m_bounds[2] << "  " << m_bounds[1] << "," << m_bounds[3] << std::endl;

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor4f(1.0, 1.0, 1.0, m_alpha);
    s_textMaker->bindFontTexture();
    s_textMaker->renderTextLine(m_pos[0], viewport_height - m_pos[1], m_text);
    s_textMaker->unbindFontTexture();
    glPopAttrib();

  }

  void
  gee_textfield::init(){

    s_textMaker = new gloost::TextureText("../data/fonts/gloost_Monaco_16.png"); // height is 28px
    s_textMaker->setCharSpace(-4.0);
    s_textMaker->setCharScale(1.0);
    s_textMaker->setCharAlpha(0.9);
    
  }


}
