#include "gee_envelope2.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <gloostRenderGoodies.h>
#include <fstream>


namespace gee{


  /* static */ gloost::vec2 gee_envelope2::last_mouse_pos(0, 0);
  /* static */ gloost::TextureManager* gee_envelope2::texture_man(gloost::TextureManager::getInstance());

//------------------------------------------------------------------------------//default ctor		
	gee_envelope2::gee_envelope2(const char* bg_file, int width, int height)
    :	pos(0,0),
	size(width, height),
		keys(),
//		//tf_out(),
		texture_id(0),
		texture_keyring(0),
		name("noname"),
	flag_active(false),
	m_cb_generate_tf(0),
	m_cb_on_stop_edit_tf(0),
	m_index_generate_tf(0),
	m_tf_keys(),
	m_index_on_stop_edit_tf(0)






	{
	  texture_id = texture_man->createTexture(std::string(bg_file));
	  texture_keyring = texture_man->createTexture("../data/images/keyring.png");

		//tf_out = new gee::gee_textfield_button("save_tf");
		//tf_out->set_pos(100,280);
//		tf_out->set_cb_left_mouse_up(generate_tf,0);

		keys.push_back(new gee_key("starter_key", 0,0));

		keys.push_back(new gee_key("key_1", 30,0));
		keys.push_back(new gee_key("key_2", 80,0));
		keys.push_back(new gee_key("key_2", 130,0));

		keys.push_back(new gee_key("key_2", 190,0));
		keys.push_back(new gee_key("key_2", 220,0));
		keys.push_back(new gee_key("key_2", 270,0));

		keys.push_back(new gee_key("starter_key", (int) size.u, 0/*(int) size.v*/));

	}

//------------------------------------------------------------------------------//default dtor
  gee_envelope2::~gee_envelope2()
  {
    texture_man->dropReference(texture_id);
    texture_man->dropReference(texture_keyring);
  }

//------------------------------------------------------------------------------//is_active
  bool
  gee_envelope2::is_active() const
	{
		return flag_active;
	}

//------------------------------------------------------------------------------//set_active
  void
  gee_envelope2::set_active(bool active){
    flag_active = active;
  }

//------------------------------------------------------------------------------//mouse
	bool
  gee_envelope2::mouse(int button, int state, int x, int y){

    last_mouse_pos.u = x;
    last_mouse_pos.v = y;

#ifdef GEE_DEBUG
    std::cerr << "MESSAGE from virtual void gee_envelope2::mouse(int button, int state, int x, int y)"
	      << std::endl
	      << "button: " << button << std::endl
	      << "state: " << state << std::endl
	      << "x: " << x << std::endl
	      << "y: " << y << std::endl;
#endif

	//activate, if the left mouse button is pressed
    if(button == GLUT_LEFT_BUTTON && is_inside(x, y)){

		if(state == GLUT_DOWN)
		{

			flag_active = true;
			#if 0
			std::cerr << "MESSAGE from virtual void gee_envelope2::mouse(int button, int state, int x, int y)"
					  << std::endl
					  << "ACTIVATE!" << std::endl;
			#endif

		std::list<gee::gee_key*>::iterator index= keys.begin();
		std::list<gee::gee_key*>::iterator end= keys.end();

		for(; index != end ;index++)
		{
			(*index)->mouse(button, state, x, y);

		}
		}
		else
		{
			flag_active = false;
			if(m_cb_on_stop_edit_tf){
			  m_cb_on_stop_edit_tf(m_index_on_stop_edit_tf);
			}
		}

		return true;

    }

    return false;
  }

//------------------------------------------------------------------------------//draw
  void
  gee_envelope2::draw(int viewport_width, int viewport_height){

    std::cerr << "envelope2.draw() " << std::endl
				<< "size: " << size.u << ", " <<size.v << std::endl
				<< "pos: " << pos.u << ", " <<pos.v << std::endl
				<< "texture_id: " << texture_id << std::endl;

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    texture_man->getTextureWithoutRefcount(texture_id)->bind();
    glPushMatrix();
	  
	  //kehrt die x-Achse um beim zeichnen
    //glTranslatef((float) m_pos[0],(float) (viewport_height - m_pos[1]),0.0);
	  
	 //kehrt die x-Achse nicht um beim zeichnen
    glTranslatef(pos.u, pos.v, 0.0);
    glBegin(GL_QUADS);
    {
      glTexCoord2f(0.0, 0.0);
      glVertex3f  (0.0, size.u, 0);

      glTexCoord2f(0.0, 1.0);
      glVertex3f  (0.0, 0.0, 0);

      glTexCoord2f(1.0, 1.0);
      glVertex3f  (size.u, 0.0, 0);

      glTexCoord2f(1.0, 0.0);
      glVertex3f  (size.u, size.v, 0);
    }
    glEnd();
    texture_man->getTextureWithoutRefcount(texture_id)->unbind();
    glPopMatrix();
    glPopAttrib();
	  
	  //drawbounds ();
  }
//------------------------------------------------------------------------------//motion
  bool
  gee_envelope2::motion(int x, int y)
  {

    if(!is_inside(x,y)){
      return false;
    }



    if(flag_active){

		#if 0
		std::cerr << "MESSAGE from virtual void gee_envelope2::motion"
				<< std::endl
				<< "My name is: " << name
				<< std::endl
				<< "new pos: " << x << ", " << y
				<< std::endl;
		#endif

		//calculating the movement of the mouse
      //const int d_x(pos.u - last_mouse_pos.u);
      //const int d_y(last_mouse_pos.v - pos.v);

		//adjusting the position of the key
      //set_pos(pos.u + d_x, pos.v + d_y);
		//set_pos(x, y);
		//if(is_inside (x, y)){


			std::list<gee::gee_key*>::iterator index= keys.begin();
			std::list<gee::gee_key*>::iterator end= keys.end();
			//std::list<gee::gee_key*>::iterator previous= 0;
			//std::list<gee::gee_key*>::iterator next= 0;			

			//placing the first key on the left edge
			if(is_inside_y(y)){

				(*index)->set_y(y);
				generate_tf();
			}
			index++;

			//placing the last key on the left edge
			end--;
			if(is_inside_y(y)){

				(*end)->set_y(y);
				generate_tf();
			}

			//placing the rest inbetween
			for(; index != end ;index++)
			{
				if(is_inside_x(x)){

					//checking the prevoius key as boundary
					index--;
					if(x > ((*index)->get_x())){

						//checking the next key as boundary
						index++;
						index++;
						if(x < ((*index)->get_x())){
							index--;
							(*index)->set_x(x);
							generate_tf();
						}
						else
						{
							index--;
						}
					}
					else
					{
						index++;
					}
				}

				if(is_inside_y(y)){

					(*index)->set_y(y);
					generate_tf();
				}

				#if 0
				std::cerr << "MESSAGE from virtual void gee_envelope2::motion"
						<< std::endl
						<< "old pos for the ring: " << (*index)->get_x() << ", " << (*index)->get_y()
						<< std::endl;
				#endif				


			}


		
    }

    return true;
  }

//------------------------------------------------------------------------------//set_pos
    void gee_envelope2::set_pos(const int x, const int y)
	{
	  int oldpos_X = (int) pos.u;
	  int oldpos_Y = (int) pos.v;

		pos.u = x;
		pos.v = y;

		int diff_X = x - oldpos_X; 
		int diff_Y = y - oldpos_Y;
		//TODO: adjust the keyrings
		std::list<gee::gee_key*>::iterator index= keys.begin();
		std::list<gee::gee_key*>::iterator end= keys.end();

		for(; index != end ;index++)
		{
		  (*index)->set_pos( (int) ( (*index)->get_x() + diff_X),(int)( (*index)->get_y() + diff_Y));

		}


	}
//------------------------------------------------------------------------------//get_pos
  const gloost::vec2
  gee_envelope2::get_pos() const{
    return pos;
  }
//------------------------------------------------------------------------------//get_x
  float
  gee_envelope2::get_x(){
    return pos.u;
  }
//------------------------------------------------------------------------------//get_y
  float
  gee_envelope2::get_y(){
    return pos.v;
  }

//------------------------------------------------------------------------------//get_y
	bool
	gee_envelope2::is_inside(int x, int y) const
	{
	  if( (pos.u + size.u + 40) > x &&
	      (pos.u - 40) < x &&
	      (pos.v + size.v + 40) > y &&
	      (pos.v - 40) < y)
			return true;
		return false;
		
	}

//------------------------------------------------------------------------------//get_y
	bool
	gee_envelope2::is_inside_x(int x) const
	{
		if(pos.u + size.u > x &&
			pos.u < x)
			return true;
		return false;
		
	}

//------------------------------------------------------------------------------//get_y
	bool
	gee_envelope2::is_inside_y(int y) const
	{
		if(pos.v + size.v > y &&
			pos.v < y)
			return true;
		return false;
		
	}

	void 
	gee_envelope2::draw_keys(int viewport_width, int viewport_height)
	{
		#if 0
		std::cerr << "envelope2.draw_keys() " << std::endl
					<< "size: " << size.u << ", " <<size.v << std::endl
					<< "pos: " << pos.u << ", " <<pos.v << std::endl
					<< "texture_id: " << texture_id << std::endl;
		#endif

		glPushMatrix();
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			{

			  glViewport(0, 0, viewport_width, viewport_height);

			  glEnable(GL_BLEND);
			  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			  glMatrixMode(GL_PROJECTION);
			  glLoadIdentity();
			  glOrtho(0, (float) viewport_width, 0, (float) viewport_height, 0.1, 10.0);
			  glMatrixMode(GL_MODELVIEW);
			  glLoadIdentity();
			  gluLookAt(0.0, 0.0, 5.0,
						0.0, 0.0, 0.0,
						0.0, 1.0, 0.0);


			  glEnable(GL_TEXTURE);
			  glEnable(GL_TEXTURE_2D);
			  glDisable(GL_COLOR_MATERIAL);
			  glDisable(GL_DEPTH_TEST);
			  glDisable(GL_LIGHTING);

			  // color
			  glColor4d(1,1,1, 1.0);

		//#if 0
			  // background drawing
			  texture_man->getTextureWithoutRefcount(texture_id)->bind();
			  {
				glPushMatrix();
				glTranslatef(pos.u, pos.v, 0.0);

				glRotatef(0.0, 0.0, 0.0, 1.0);
				  /*
				  glScalef(g_texManager->getTextureWithoutRefcount(g_Texture_graphBackground)->getWidth(),
						   g_texManager->getTextureWithoutRefcount(g_Texture_graphBackground)->getHeight(),
						   1);*/
				  
				  //size of
				glScalef(size.u, size.v, 1);
				gloost::drawQuad();
				glPopMatrix();
			  }
			  texture_man->getTextureWithoutRefcount(texture_id)->unbind();
		//#endif

			  //drawing the keyrings
		//#if 0
			  texture_man->getTextureWithoutRefcount(texture_keyring)->bind();
			  {
				  std::list<gee::gee_key*>::iterator index= keys.begin();
				  std::list<gee::gee_key*>::iterator end= keys.end();

				  for(; index != end ;index++)
				  {
					glPushMatrix();
					  glTranslatef((*index)->get_x(), (*index)->get_y(), 0.0);

					  glRotatef(0.0, 0.0, 0.0, 1.0);
					  glScalef(texture_man->getTextureWithoutRefcount(texture_keyring)->getWidth(),
							   texture_man->getTextureWithoutRefcount(texture_keyring)->getHeight(),
							   1);
					  gloost::drawQuadCenter();
					glPopMatrix();
				  }
			  }
			  texture_man->getTextureWithoutRefcount(texture_keyring)->unbind();
		//#endif
		//----------------------lines
	  glEnable(GL_LINE_SMOOTH);

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		  glEnable(GL_COLOR_MATERIAL);
		  glEnable(GL_BLEND);
		  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		  glColor4f(1.0,1.0,1.0,1.0);

		  glBegin(GL_LINES);

			  std::list<gee::gee_key*>::iterator index= keys.begin();
			  std::list<gee::gee_key*>::iterator end= keys.end();

			  for(; index != end ;index++)
			  {

			  glVertex3d((*index)->get_x(), (*index)->get_y(), 0.0);

			  index++;
			  if(index!=end)
			  glVertex3d((*index)->get_x(), (*index)->get_y(), 0.0);
			  index--;
				}
				
			glEnd();

			glPopMatrix();
			}
		glPopAttrib();

		}

		//tf_out->draw(viewport_width, viewport_height);

	}

//------------------------------------------------------------------------------//get_y
	void 
	gee_envelope2::generate_tf()
	{


	if(m_cb_generate_tf){

	  m_tf_keys.clear();
	  std::list<gee::gee_key*>::iterator index= keys.begin();
	  std::list<gee::gee_key*>::iterator end= keys.end();
	  
	  
	  //iterate over all keys and write the values into a file
	  for(; index != end ; ++index){
	    
	    tf_key k;
	    k.x = ((*index)->get_x() - pos.u)/(size.u - 1);
	    k.y = ((*index)->get_y() - pos.v)/(size.v - 1);
	    m_tf_keys.push_back(k);
	  }

	  m_cb_generate_tf(m_index_generate_tf, m_tf_keys);
	}


	}

void
gee_envelope2::set_tf_keys(const std::vector<tf_key>& tf_keys){


   std::list<gee::gee_key*>::iterator index= keys.begin();
   std::list<gee::gee_key*>::iterator end= keys.end();
	  
   for(; index != end ; ++index){
     delete (*index);
   }



   keys.clear();
   //keys.push_back(new gee_key("starter_key", 0,0));
   for(unsigned int i = 0; i < tf_keys.size(); ++i){

	int x = (int) (tf_keys[i].x * (size.u - 1)  + pos.u);
	int y = (int) (tf_keys[i].y * (size.v - 1)  + pos.v);
	std::cerr << x << " " << y << std::endl;
	keys.push_back(new gee_key(std::string("key_") + gloost::toString(i), x,y));


   }

}
	
  void
  gee_envelope2::set_cb_generate_tf(void (* callback)(int index, const std::vector<tf_key>& tf_keys), int index){
    m_cb_generate_tf = callback;
    m_index_generate_tf = index;
  }

  void
  gee_envelope2::set_cb_on_stop_edit_tf(void (* callback)(int), int index){
    m_cb_on_stop_edit_tf = callback;
    m_index_on_stop_edit_tf = index;
  }

}
