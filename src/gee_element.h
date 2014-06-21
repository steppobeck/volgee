#ifndef GEE_ELEMENT_H
#define GEE_ELEMENT_H


#include <vector>

namespace gee{


  class gee_element{

  public:
    
    gee_element();
    virtual ~gee_element();

    virtual bool mouse(int button, int state, int x, int y);
    virtual bool motion(int x, int y);
    virtual void set_bounds(int min_x, int max_x, int min_y, int max_y);

    void add_child(gee_element* child);
    void set_pos(int x, int y);
    const int* get_pos() const;
    virtual void draw(int viewport_width, int viewport_height);



    bool is_active() const;
    bool is_working() const;
    virtual void toggle_active();
    void set_active(bool active);

  protected:

    bool inside_bounds(int x, int y) const;

    int m_bounds[4];
    std::vector<gee_element*> m_children;
    int m_pos[2];
    bool m_active;
    bool m_working;
    static int s_last_mouse_x;
    static int s_last_mouse_y;

  };


}



#endif // #ifndef  GEE_ELEMENT_H
