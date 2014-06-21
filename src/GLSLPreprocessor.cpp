#include "GLSLPreprocessor.h"

#include <fstream>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


GLSLPreprocessor::GLSLPreprocessor()
  : m_tmp_file()
{}

GLSLPreprocessor::~GLSLPreprocessor()
{
  // delete m_tmp_file from disk
#if 1
  if(m_tmp_file != "")
    unlink(m_tmp_file.c_str());
#endif
}


std::string
GLSLPreprocessor::process(const char* filename){

  m_tmp_file = "";
  std::string token;
  std::string i_filename;
  bool found = false;

  {
    // open file
    std::ifstream in;
    in.open(filename);
    

    while(in >> token){
      
      if("#include" == token){
	
	found = true;
	
	in >> i_filename;
	i_filename = i_filename.substr(1,i_filename.size()-2);
	
	std::cerr << "Message from GLSLPreprocessor: found " << i_filename << std::endl;
	
	m_tmp_file = "out.fs";
	
	std::cerr << "Message from GLSLPreprocessor:  tempfilename is " << m_tmp_file << std::endl;
	break;



	
      }

    }

    in.close();
  }

  if(found){

    fflush(NULL);
    sync();


    struct stat file_info;

    stat(i_filename.c_str(),&file_info);
    unsigned long i_fileSize = file_info.st_size;
    FILE* i_file = fopen(i_filename.c_str(),"r");
    if(i_file == NULL){
      return std::string(filename);
    }
    unsigned char* i_file_buff = new unsigned char [i_fileSize];
    if(i_file_buff == 0){
      return std::string(filename);
    }
    if(i_fileSize != fread (i_file_buff,1,i_fileSize,i_file)){
      fclose(i_file);
      return std::string(filename);
    }
    fclose(i_file);
    


    stat(filename,&file_info);
    FILE* fs_file = fopen(filename,"r");
    if(fs_file == NULL){
      return std::string(filename);
    }
    unsigned long fs_fileSize = file_info.st_size;
    unsigned char* fs_file_buff = new unsigned char [fs_fileSize];
    if(fs_file_buff == 0){
      return std::string(filename);
    }
    if(fs_fileSize != fread (fs_file_buff,1,fs_fileSize,fs_file)){
      fclose(fs_file);
      return std::string(filename);
    }
    fclose(fs_file);
    

    FILE* tmp_file = fopen(m_tmp_file.c_str(),"w+");
    if(tmp_file == NULL){
      return std::string(filename);
    }
    if(i_fileSize != fwrite (i_file_buff,1,i_fileSize,tmp_file)){
      fclose(tmp_file);
      return std::string(filename);
    }
    if(fs_fileSize != fwrite (fs_file_buff,1,fs_fileSize,tmp_file)){
      fclose(tmp_file);
      return std::string(filename);
    }
    fclose(tmp_file);
    

    fflush(NULL);
    sync();


    delete [] i_file_buff;
    delete [] fs_file_buff;
    

    return m_tmp_file;
  }


  return std::string(filename);
}
