#include "gee_key.h"

#include <GL/glew.h>
#include <GL/glut.h>

namespace gee{


  /* static */ gloost::vec2 gee_key::last_mouse_pos(0, 0);
  /* static */ gloost::TextureManager* gee_key::texture_man(gloost::TextureManager::getInstance());

//------------------------------------------------------------------------------//default ctor		
	gee_key::gee_key()
    :	pos(0,0),
		size(10, 10),
		texture_id(0),
		name("noname"),

		flag_active(false)

  {
    texture_id = texture_man->createTexture("../data/images/keyring_15.png");

  }

//------------------------------------------------------------------------------//size ctor		
	gee_key::gee_key(int x, int y)
    :	pos(x,y),
		size(10, 10),
		texture_id(0),
		name("noname"),

		flag_active(false)

  {
    texture_id = texture_man->createTexture("../data/images/keyring_15.png");

  }

//------------------------------------------------------------------------------//name ctor		
	gee_key::gee_key(std::string nm)
    :	pos(0,0),
		size(10, 10),
		texture_id(0),
		name(nm),

		flag_active(false)

  {
    texture_id = texture_man->createTexture("../data/images/keyring_15.png");

  }

//------------------------------------------------------------------------------//name & size ctor		
	gee_key::gee_key(std::string nm, int x, int y)
    :	pos(x,y),
		size(10, 10),
		texture_id(0),
		name(nm),

		flag_active(false)

  {
    texture_id = texture_man->createTexture("../data/images/keyring_15.png");

  }
//------------------------------------------------------------------------------//default dtor
  gee_key::~gee_key()
  {
    texture_man->dropReference(texture_id);
  }

//------------------------------------------------------------------------------//is_active
  bool
  gee_key::is_active() const
	{
		return flag_active;
	}

//------------------------------------------------------------------------------//set_active
  void
  gee_key::set_active(bool active){
    flag_active = active;
  }

//------------------------------------------------------------------------------//mouse
	bool
  gee_key::mouse(int button, int state, int x, int y){

    last_mouse_pos.u = x;
    last_mouse_pos.v = y;

//#ifdef GEE_DEBUG
    std::cerr << "MESSAGE from virtual void gee_key::mouse(int button, int state, int x, int y)"
	      << std::endl
	      << "button: " << button << std::endl
	      << "state: " << state << std::endl
	      << "x: " << x << std::endl
	      << "y: " << y << std::endl;
//#endif

	//activate, if the left mouse button is pressed
    if(button == GLUT_LEFT_BUTTON && is_inside(x, y)){

		if(state == GLUT_DOWN)
		{

			flag_active = true;
			#if 0
			std::cerr << "MESSAGE from virtual void gee_key::mouse(int button, int state, int x, int y)"
					  << std::endl
					  << "ACTIVATE!" << std::endl;
			#endif
		}


    }
		else
		{
			flag_active = false;
		}

    return true;
  }

//------------------------------------------------------------------------------//motion
  bool
  gee_key::motion(int x, int y)
	{

    if(flag_active){

		//#if 0
		std::cerr << "MESSAGE from virtual void gee_key::mouse"
				<< std::endl
				<< "My name is: " << name
				<< std::endl
				<< "new pos: " << x << ", " << y
				<< std::endl;
		//#endif

		//calculating the movement of the mouse
      //const int d_x(pos.u - last_mouse_pos.u);
      //const int d_y(last_mouse_pos.v - pos.v);

		//adjusting the position of the key
      //set_pos(pos.u + d_x, pos.v + d_y);
		set_pos(x, y);
    }

    return false;
	}

//------------------------------------------------------------------------------//set_pos
    void gee_key::set_pos(const int x, const int y)
	{
		pos.u = x;
		pos.v = y;
	}
//------------------------------------------------------------------------------//set_pos
    void gee_key::set_x(const int x)
	{
		if(flag_active)
			pos.u = x;
	}
//------------------------------------------------------------------------------//set_pos
    void gee_key::set_y(const int y)
	{
		if(flag_active)
			pos.v = y;
	}
//------------------------------------------------------------------------------//get_pos
  const gloost::vec2
  gee_key::get_pos() const{
    return pos;
  }
//------------------------------------------------------------------------------//get_x
  float
  gee_key::get_x(){
    return pos.u;
  }
//------------------------------------------------------------------------------//get_y
  float
  gee_key::get_y(){
    return pos.v;
  }

//------------------------------------------------------------------------------//get_y
	bool
	gee_key::is_inside(int x, int y) const
	{
		//bool inside = false;
/*
		if(pos.u + size.u/2 > x &&
			pos.u - size.u/2 < x &&
			pos.v + size.v/2 > y &&
			pos.v - size.v/2 < y)
			return true;
*/
		if(pos.u + 20 > x &&
			pos.u - 20 < x &&
			pos.v + 20 > y &&
			pos.v - 20 < y)
			return true;
		return false;
		
	}

}
