#ifndef GEE_SURFACE_H
#define GEE_SURFACE_H



#include <gee_element.h>

namespace gloost{
  class TextureManager;
}


namespace gee{

  class gee_surface : public gee_element{

  public:
    
    gee_surface(const char* filename);
    ~gee_surface();

    //bool mouse(int button, int state, int x, int y);
    //bool motion(int x, int y);
    //void set_bounds(unsigned int min_x, unsigned int max_x, unsigned int min_y, unsigned int max_y);
    void set_pos(int x, int y);
    void draw(int viewport_width, int viewport_height);

  protected:
    unsigned int m_texture_id;
    gloost::TextureManager* m_texture_manager;
    
  };


}


#endif // #ifndef  GEE_SURFACE_H
