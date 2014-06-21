#ifndef GEE_DRAGGER_H
#define GEE_DRAGGER_H


#include <gee_surface.h>

#include <vector>

namespace gee{


  class gee_dragger : public gee_surface{

  public:
    
    gee_dragger(const char* filename);
    ~gee_dragger();

    bool mouse(int button, int state, int x, int y);
    bool motion(int x, int y);

    void add_draggie(gee_element* draggie);
    void toggle_active();
  private:
    std::vector<gee_element*> m_draggies;
    bool m_dragging;
  };


}



#endif // #ifndef  GEE_DRAGGER_H
