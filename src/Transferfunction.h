#ifndef TRANSFERFUNCTION_H
#define TRANSFERFUNCTION_H

#define TF_TEXTURE_WIDTH 256
#define TF_TEXTURE_WIDTH_NORM 255
#define TF_TEXTURE_HEIGHT 1

#include <vector>

struct tf_key{
  float x;
  float y;
};


class Transferfunction{


 public:
  Transferfunction();
  ~Transferfunction();

  bool load(const char* filename);
  unsigned char* get_texture_data();

  bool set_keys_alpha(const std::vector<tf_key>& tf_keys);
  bool set_keys_red(const std::vector<tf_key>& tf_keys);
  bool set_keys_green(const std::vector<tf_key>& tf_keys);
  bool set_keys_blue(const std::vector<tf_key>& tf_keys);

  const std::vector<tf_key>& get_keys_alpha() const;
  const std::vector<tf_key>& get_keys_red() const;
  const std::vector<tf_key>& get_keys_green() const;
  const std::vector<tf_key>& get_keys_blue() const;

 private:
  void clean_up();  
  unsigned char interpolate(const std::vector<tf_key>& tf_keys, unsigned int pos);
  unsigned char* m_tf_texture_buffer;
  std::vector<tf_key> m_alpha;
  std::vector<tf_key> m_red;
  std::vector<tf_key> m_green;
  std::vector<tf_key> m_blue;

};




#endif // #ifndef TRANSFERFUNCTION_H
