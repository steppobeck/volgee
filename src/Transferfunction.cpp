#include "Transferfunction.h"

#include <TextureManager.h>


#include <fstream>

#define TF_TEXTURE_WIDTH 256
#define TF_TEXTURE_WIDTH_NORM 255
#define TF_TEXTURE_HEIGHT 1



Transferfunction::Transferfunction()
  : m_tf_texture_buffer(0),
    m_alpha(),
    m_red(),
    m_green(),
    m_blue()
{
    m_tf_texture_buffer = new unsigned char [4*TF_TEXTURE_WIDTH];
}

Transferfunction::~Transferfunction(){
  delete [] m_tf_texture_buffer;
}


bool
Transferfunction::load(const char* filename){

  clean_up();

  std::ifstream tf_file;
  tf_file.open(filename);

  std::string token;

  while(tf_file >> token){

    if("a" == token){
      tf_key k;
      tf_file >> k.x >> k.y;
      m_alpha.push_back(k);
    }
    else if("r" == token){
      tf_key k;
      tf_file >> k.x >> k.y;
      m_red.push_back(k);
    }
    else if("g" == token){
      tf_key k;
      tf_file >> k.x >> k.y;
      m_green.push_back(k);
    }
    else if("b" == token){
      tf_key k;
      tf_file >> k.x >> k.y;
      m_blue.push_back(k);
    }
    
  }






  if(m_alpha.size() < 2){
    return false;
  }

  if(m_red.size() < 2){
    return false;
  }

  if(m_green.size() < 2){
    return false;
  }

  if(m_blue.size() < 2){
    return false;
  }
  

  return true;
}

unsigned char*
Transferfunction::get_texture_data(){

  

  unsigned int pos = 0;
  for(unsigned int i = 0; i < TF_TEXTURE_WIDTH * 4; i += 4, ++pos){
    m_tf_texture_buffer[i] = interpolate(m_red,pos);
    m_tf_texture_buffer[i+1] = interpolate(m_green,pos);
    m_tf_texture_buffer[i+2] = interpolate(m_blue,pos);
    m_tf_texture_buffer[i+3] = interpolate(m_alpha,pos);
    //std::cerr << (unsigned int) m_tf_texture_buffer[i+3] << std::endl;

  }
  
  return m_tf_texture_buffer;
}


unsigned char
Transferfunction::interpolate(const std::vector<tf_key>& tf_keys, unsigned int pos){

  
  const float p((1.0f * pos)/TF_TEXTURE_WIDTH_NORM);
  
  //std::cerr << "--------------------------------" << std::endl;
  //std::cerr << "tf_keys.size() " << tf_keys.size() << std::endl;

  for(unsigned int i = 0; i < tf_keys.size(); ++i){

    if(tf_keys[i].x >= p && i > 0){
      const unsigned int start = i - 1;
      const unsigned int stop = i;
      //std::cerr << "start " << start << " stop " << stop << std::endl;
      
      float value = tf_keys[start].y + ((p - tf_keys[start].x)/(tf_keys[stop].x - tf_keys[start].x)) * (tf_keys[stop].y - tf_keys[start].y);
      return (unsigned char) (TF_TEXTURE_WIDTH_NORM * value);

    }

  }

  std::cerr << "ERROR in Transferfunction::interpolate" << std::endl;
    
  return 255;
}

void
Transferfunction::clean_up(){
  m_alpha.clear();
  m_red.clear();
  m_green.clear();
  m_blue.clear();
}


bool
Transferfunction::set_keys_alpha(const std::vector<tf_key>& tf_keys){
  m_alpha = tf_keys;
  if(m_alpha.size() < 2){
    return false;
  }

  return true;
}

bool
Transferfunction::set_keys_red(const std::vector<tf_key>& tf_keys){
  m_red = tf_keys;
  if(m_red.size() < 2){
    return false;
  }
  return true;
}

bool
Transferfunction::set_keys_green(const std::vector<tf_key>& tf_keys){
  m_green = tf_keys;
  if(m_green.size() < 2){
    return false;
  }
  return true;
}

bool
Transferfunction::set_keys_blue(const std::vector<tf_key>& tf_keys){
  m_blue = tf_keys;
  if(m_blue.size() < 2){
    return false;
  }
  return true;
}



const std::vector<tf_key>&
Transferfunction::get_keys_alpha() const{
	return m_alpha;
}

const std::vector<tf_key>&
Transferfunction::get_keys_red() const{
	return m_red;
}

const std::vector<tf_key>&
Transferfunction::get_keys_green() const{
	return m_green;
}

const std::vector<tf_key>&
Transferfunction::get_keys_blue() const{
	return m_blue;
}


