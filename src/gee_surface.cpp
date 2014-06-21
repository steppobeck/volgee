#include "gee_surface.h"

#include <TextureManager.h>

namespace gee{
    
  gee_surface::gee_surface(const char* filename)
    : m_texture_id(0),
      m_texture_manager(gloost::TextureManager::getInstance())
  {

    m_texture_id = m_texture_manager->createTexture(filename);
    set_bounds(0, m_texture_manager->getTextureWithoutRefcount(m_texture_id)->getWidth(), 0, m_texture_manager->getTextureWithoutRefcount(m_texture_id)->getHeight());
  }

  gee_surface::~gee_surface()
  {
    m_texture_manager->dropReference(m_texture_id);
  }


  void
  gee_surface::set_pos(int x, int y){
    gee_element::set_pos(x,y);

    // calc new bounds
    const int width(m_bounds[1] - m_bounds[0]);
    const int height(m_bounds[3] - m_bounds[2]);
    m_bounds[0] = m_pos[0];
    m_bounds[1] = m_pos[0] + width;
    m_bounds[2] = m_pos[1] - height;
    m_bounds[3] = m_pos[1];

  }

  void
  gee_surface::draw(int viewport_width, int viewport_height){

    //std::cerr << "bounds: " << m_bounds[0] << "," << m_bounds[2] << "   " << m_bounds[1] << "," << m_bounds[3] << std::endl;

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    
    m_texture_manager->getTextureWithoutRefcount(m_texture_id)->bind();
    glPushMatrix();
    glTranslatef((float) m_pos[0],(float) (viewport_height - m_pos[1]),0.0);
    glBegin(GL_QUADS);
    {
      const int width(m_bounds[1] - m_bounds[0]);
      const int height(m_bounds[3] - m_bounds[2]);
      glTexCoord2f(0.0, 0.0);
      glVertex3f  (0.0, height, 0);

      glTexCoord2f(0.0, 1.0);
      glVertex3f  (0.0, 0.0, 0);

      glTexCoord2f(1.0, 1.0);
      glVertex3f  (width, 0.0, 0);
      
      glTexCoord2f(1.0, 0.0);
      glVertex3f  (width, height, 0);
    }
    glEnd();
    m_texture_manager->getTextureWithoutRefcount(m_texture_id)->unbind();
    glPopMatrix();
    glPopAttrib();
  }

}
