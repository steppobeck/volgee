#include "VolumeInfo.h"


#include <StringTokenizer.h>
#include <volgee_helper.h>

#include <gloostHelper.h>

#include <vector>
#include <iostream>
#include <stdlib.h>

VolumeInfo::VolumeInfo(const char* vol_file_name)
  : name(),
    dim(),
    channels(0),
    bits_per_channel(0)
{
  sys::StringTokenizer vol_info_string(gee::basename(vol_file_name).c_str());
  dim[0] = 256;
  dim[1] = 256;
  dim[2] = 256;
  channels = 1;

  // foot_w256_h256_d256_c1_b8.raw

  std::vector<std::string> tokens = vol_info_string.get_tokens("_");
  unsigned int state = 0;
  for(int i = (int) tokens.size() - 1; i > -1; --i, ++state){

    switch(state){
    case 0:
      { // extracing bits_per_channel
	sys::StringTokenizer tmp_str(tokens[i].c_str());
	sys::StringTokenizer tmp_str1((tmp_str.get_tokens(".")[0]).c_str());
	if(tmp_str1.get_tokens("b").size() != 1){
	  std::cerr << "ERROR when extracting VolumeInfo in VolumeInfo::VolumeInfo(const char* vol_file_name)" << std::endl;
	}
	bits_per_channel = atoi((tmp_str1.get_tokens("b")[0]).c_str());
      }
      break;

    case 1:
      { // extracting channels
	sys::StringTokenizer tmp_str(tokens[i].c_str());
	if(tmp_str.get_tokens("c").size() != 1){
	  std::cerr << "ERROR when extracting VolumeInfo in VolumeInfo::VolumeInfo(const char* vol_file_name)" << std::endl;
	}
	channels = atoi((tmp_str.get_tokens("c")[0]).c_str());
      }
      break;

    case 2:
      { // extracting dim[2]
	sys::StringTokenizer tmp_str(tokens[i].c_str());
	if(tmp_str.get_tokens("d").size() != 1){
	  std::cerr << "ERROR when extracting VolumeInfo in VolumeInfo::VolumeInfo(const char* vol_file_name)" << std::endl;
	}
	dim[2] = atoi((tmp_str.get_tokens("d")[0]).c_str());
      }
      break;

    case 3:
      { // extracting dim[1]
	sys::StringTokenizer tmp_str(tokens[i].c_str());
	if(tmp_str.get_tokens("h").size() != 1){
	  std::cerr << "ERROR when extracting VolumeInfo in VolumeInfo::VolumeInfo(const char* vol_file_name)" << std::endl;
	}
	dim[1] = atoi((tmp_str.get_tokens("h")[0]).c_str());
      }
      break;

    case 4:
      { // extracting dim[0]
	sys::StringTokenizer tmp_str(tokens[i].c_str());
	if(tmp_str.get_tokens("w").size() != 1){
	  std::cerr << "ERROR when extracting VolumeInfo in VolumeInfo::VolumeInfo(const char* vol_file_name)" << std::endl;
	}
	dim[0] = atoi((tmp_str.get_tokens("w")[0]).c_str());
      }
      break;

    case 5:
      { // extracting name
	name = tokens[i];
      }
      break;

    default:
      break;

    }


  }
  
}


VolumeInfo::~VolumeInfo()
{}



unsigned int
VolumeInfo::get_max_dim() const{


  return std::max(dim[0],std::max(dim[1],dim[2]));

}

std::string
VolumeInfo::toString() const{
  std::string s = name + " "
    + gloost::toString(dim[0]) + "x" + gloost::toString(dim[1]) + "x" + gloost::toString(dim[2])
    + " "
    + gloost::toString(bits_per_channel) + "bit";
#if 0
    + " "
    + gloost::toString(channels) + "c";
#endif
  return s;
}



std::ostream& operator<< (std::ostream& o, const VolumeInfo& v){
  o << v.toString();
  return o;
}
