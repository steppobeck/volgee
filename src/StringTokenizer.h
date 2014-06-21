#ifndef STRINGTOKENIZER_H
#define STRINGTOKENIZER_H

#include <string>
#include <vector>

namespace sys{


  class StringTokenizer{


  public:
    StringTokenizer(const char* s);
    ~StringTokenizer();

    const std::vector<std::string>& get_tokens(const char* delimiter = " ");
    std::string get_token_string(const char* delimiter, const char* space, unsigned int begin, unsigned int end);


  private:
    std::string m_string;
    std::vector<std::string> m_tokens;

  };


}




#endif // #ifndef STRINGTOKENIZER_H
