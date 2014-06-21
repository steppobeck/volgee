#ifndef GLSLPREPROCESSOR_H
#define GLSLPREPROCESSOR_H


#include <string>


class GLSLPreprocessor{

 public:
  GLSLPreprocessor();
  ~GLSLPreprocessor();


  std::string process(const char* filename);

 private:
  std::string m_tmp_file;

};




#endif // #ifndef  GLSLPREPROCESSOR_H
