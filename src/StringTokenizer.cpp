#include "StringTokenizer.h"

#include <iostream>



namespace sys{

  StringTokenizer::StringTokenizer(const char* s)
    : m_string(s),
      m_tokens()
  {}


  StringTokenizer::~StringTokenizer()
  {}

  // from http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html
  const std::vector<std::string>&
  StringTokenizer::get_tokens(const char* d){
    m_tokens.clear();

    const std::string delimiters(d);
    std::string::size_type lastPos = m_string.find_first_not_of(delimiters, 0);
    std::string::size_type pos     = m_string.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        m_tokens.push_back(m_string.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = m_string.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = m_string.find_first_of(delimiters, lastPos);
    }

    return m_tokens;
  }


  std::string
  StringTokenizer::get_token_string(const char* delimiter, const char* space, unsigned int begin, unsigned int end){
    
    std::string result;
    const std::string the_space(space);
    const std::vector<std::string>& tokens = get_tokens(delimiter);

    const unsigned int the_end((end == 0) ? tokens.size() : end);
    
    std::string last_result;
    unsigned int pos = begin;

    for(; pos < the_end && pos < tokens.size(); ++pos){
      result += tokens[pos];
      last_result = result;
      result += the_space;
    }

    result = last_result;

    return result;
  }


}

