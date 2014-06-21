#ifndef GEE_KEY_H
#define GEE_KEY_H

#include <UniformTypes.h>
#include <TextureManager.h>
#include <string>
#include <iostream>

namespace gee{

  //a draggable ring representing an influencable value
  class gee_key{

  public:

    gee_key();
    gee_key(int x, int y);
    gee_key(std::string);
    gee_key(std::string, int x, int y);
    ~gee_key();

    bool mouse(int button, int state, int x, int y);
    bool motion(int x, int y);
		  
    void set_pos(int x, int y);
    void set_x(int x);
    void set_y(int y);
    const gloost::vec2 get_pos() const;
	float get_x();
	float get_y();
		  
    void draw(int viewport_width, int viewport_height);
	void draw_bounds();
		  
	void set_active(bool active);
	bool is_active () const;
		  
	bool is_inside(int x, int y) const;
		  
  private:


	
	//position and size
	gloost::vec2 pos;
	gloost::vec2 size;
	static gloost::vec2 last_mouse_pos;

	//the texture manager and the texture id
	static gloost::TextureManager* texture_man;
    unsigned int texture_id;
		  
	std::string name;
		  
	bool flag_active;


		  

  };


}



#endif // #ifndef  GEE_KEY_H
