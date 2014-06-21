#ifndef GEE_SURFACE_BUTTON_H
#define GEE_SURFACE_BUTTON_H



#include <gee_surface.h>


namespace gee{

  class gee_surface_button : public gee_surface{

  public:
    
    gee_surface_button(const char* filename, const char* filename_active = 0);
    ~gee_surface_button();

    bool mouse(int button, int state, int x, int y);
    bool motion(int x, int y);


    void draw(int viewport_width, int viewport_height);

    void set_cb_left_mouse_down(void (* callback)(int) , int index);
    void set_cb_middle_mouse_down(void (* callback)(int), int index);
    void set_cb_right_mouse_down(void (* callback)(int), int index);
    void set_cb_left_mouse_pressed(void (* callback)(int), int index);
    void set_cb_middle_mouse_pressed(void (* callback)(int), int index);
    void set_cb_right_mouse_pressed(void (* callback)(int), int index);
    void set_cb_left_mouse_up(void (* callback)(int), int index);
    void set_cb_middle_mouse_up(void (* callback)(int), int index);
    void set_cb_right_mouse_up(void (* callback)(int), int index);




  private:
    unsigned int m_texture_active_id;
    unsigned int m_active_button;
    void (*m_cb_left_mouse_down)(int);
    void (*m_cb_middle_mouse_down)(int);
    void (*m_cb_right_mouse_down)(int);

    void (*m_cb_left_mouse_pressed)(int);
    void (*m_cb_middle_mouse_pressed)(int);
    void (*m_cb_right_mouse_pressed)(int);

    void (*m_cb_left_mouse_up)(int);
    void (*m_cb_middle_mouse_up)(int);
    void (*m_cb_right_mouse_up)(int);

    int m_index_cb_left_mouse_down;
    int m_index_cb_middle_mouse_down;
    int m_index_cb_right_mouse_down;

    int m_index_cb_left_mouse_pressed;
    int m_index_cb_middle_mouse_pressed;
    int m_index_cb_right_mouse_pressed;

    int m_index_cb_left_mouse_up;
    int m_index_cb_middle_mouse_up;
    int m_index_cb_right_mouse_up;
  };


}


#endif // #ifndef  GEE_SURFACE_BUTTON_H
