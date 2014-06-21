#ifndef DIRECTORY_H
#define DIRECTORY_H



#include <string>
#include <vector>


namespace sys{


  class Directory{


  public:

    Directory(std::string path);
    ~Directory();

    size_t size() const;
    bool open(const char* filter = 0);
    std::vector<std::string>& get_entries();

  private:

    bool string_ends_with(std::string s, std::string e);

    std::vector<std::string> m_entries;
    std::string m_path;


  };



}



#endif // #ifndef  DIRECTORY_H
