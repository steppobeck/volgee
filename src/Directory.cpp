#include "Directory.h"


#include <stddef.h>
#include <cstdio>
#include <sys/types.h>
#include <dirent.h>

#include <iostream>

#include <algorithm>

namespace sys{


  Directory::Directory(std::string path)
    : m_entries(),
      m_path(path)
  {}

  Directory::~Directory()
  {}

  size_t
  Directory::size() const{
    return m_entries.size();
  }

  bool
  Directory::open(const char* filter){
    DIR *dp;
    struct dirent *ep;
     
    dp = opendir (m_path.c_str());
    if (dp != NULL){
      ep = readdir (dp);
      while (ep){
	
	if(ep){
	  std::string entry(m_path + "/" + ep->d_name);
	  if(filter){
	    std::string fil(filter);
	    if(string_ends_with(entry,fil)){
	      m_entries.push_back(entry);
	    }
	  }
	  else{
	    m_entries.push_back(entry);
	  }
	}

	ep = readdir (dp);

      }
      closedir (dp);
    }
    else{
      perror (std::string("Couldn't open the directory" + m_path).c_str());
      return false;
    }

    std::sort(m_entries.begin(),m_entries.end());

    return true;

  }


  std::vector<std::string>&
  Directory::get_entries(){
    return m_entries;
  }


  bool
  Directory::string_ends_with(std::string s, std::string e){

    int index_s(s.size()-1);
    int index_e(e.size()-1);
    if(index_s < index_e){
      return false;
    }
    while(index_s >= 0 && index_e >= 0){

      if(s[index_s] != e[index_e]){
	return false;
      }

      --index_s;
      --index_e;
    }

    return true;
  }



}
