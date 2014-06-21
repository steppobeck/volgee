#ifndef GEE_TEXTFIELD_H
#define GEE_TEXTFIELD_H



#include <gee_element.h>
#include <string>


namespace gloost{
  class TextureText;
}


namespace gee{


  class gee_textfield : public gee_element{

  public:
    
    gee_textfield(const char* text);
    ~gee_textfield();

    void set_pos(int x, int y);
    void set_text(const char* text);
    
    void draw(int viewport_width, int viewport_height);
    

  protected:

    void init();
    static gloost::TextureText* s_textMaker;

    

  private:
    void calc_bounds();
    std::string m_text;
    
  protected:
    float m_alpha;

  };


}


#endif // #ifndef  GEE_TEXTFIELD_H
