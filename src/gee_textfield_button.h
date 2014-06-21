#ifndef GEE_TEXTFIELD_BUTTON_H
#define GEE_TEXTFIELD_BUTTON_H



#include <gee_textfield.h>



namespace gloost{
  class TextureText;
}


namespace gee{

  class gee_textfield_button : public gee_textfield{

  public:
    
    gee_textfield_button(const char* text);
    ~gee_textfield_button();

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


#endif // #ifndef  GEE_TEXTFIELD_BUTTON_H
