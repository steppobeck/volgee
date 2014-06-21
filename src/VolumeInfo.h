#ifndef VOLUMEINFO_H
#define VOLUMEINFO_H

#include <iosfwd>
#include <string>
// extracts info from such a filename foot_w256_h256_d256_c1_b8.raw


class VolumeInfo{
 public:

  VolumeInfo(const char* vol_file_name);
  ~VolumeInfo();


  unsigned int get_max_dim() const;
  std::string toString() const;

  std::string name;
  unsigned int dim[3];
  unsigned int channels;
  unsigned int bits_per_channel;


};


std::ostream& operator<< (std::ostream& o, const VolumeInfo& v);


#endif // #ifndef VOLUMEINFO_H
