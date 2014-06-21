#ifndef GEE_ENVELOPE2_H
#define GEE_ENVELOPE2_H

#include <UniformTypes.h>
#include <TextureManager.h>
#include <string>
#include <iostream>
#include <list>

#include <gee_key.h>
#include <gee_textfield_button.h>


#include <Transferfunction.h>


namespace gee{

	class gee_envelope2
	{

		public:

	  gee_envelope2(const char* bg_file, int width, int height);
			~gee_envelope2();


			bool mouse(int button, int state, int x, int y);//--
			bool motion(int x, int y);//--
				  
			void set_pos(int x, int y);//--
			const gloost::vec2 get_pos() const;//
			float get_x();//
			float get_y();//
				  
			void draw(int viewport_width, int viewport_height);//--
			void draw_keys(int viewport_width, int viewport_height);//--
			void draw_bounds();//**
				  
			void set_active(bool active);//
			bool is_active () const;//
				  
			bool is_inside(int x, int y) const;//--
			bool is_inside_x(int x) const;//--
			bool is_inside_y(int y) const;//--

			//inserts a key in the list
			void insert_key();//**
			void remove_key();//**

			//generates a transfer function from the positions of the keys
			void generate_tf();

                        void set_tf_keys(const std::vector<tf_key>& tf_keys);


			void set_cb_generate_tf(void (* callback)(int index, const std::vector<tf_key>& tf_keys), int index);
			void set_cb_on_stop_edit_tf(void (* callback)(int), int index);

		private:

			//position and size
			gloost::vec2 pos;
			gloost::vec2 size;
			static gloost::vec2 last_mouse_pos;

			//the list of keys for constructing an envelope
			std::list<gee::gee_key*> keys;

			//gee::gee_textfield_button* tf_out; 

			//the texture manager and the texture id
			static gloost::TextureManager* texture_man;
			unsigned int texture_id;
			unsigned int texture_keyring;
				  
			std::string name;
				  
			bool flag_active;

			void (*m_cb_generate_tf)(int index, const std::vector<tf_key>& tf_keys);
			void (*m_cb_on_stop_edit_tf)(int index);
			int m_index_generate_tf;
			std::vector<tf_key> m_tf_keys;
			int m_index_on_stop_edit_tf;




	};

}

#endif // #ifndef  GEE_ENVELOPE2_H
