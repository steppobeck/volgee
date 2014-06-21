
#if !defined(OGLRWPERF_CLOCK_H)

#define OGLRWPERF_CLOCK_H

// includes, system

#include <boost/noncopyable.hpp> //
#include <cstddef>               // std::size_t
#include <iosfwd>                // std::ostream
#include <string>                // std::string

// includes, project

#include <timevalue.h>

// exported namespace

namespace oglrwperf {
  
  // types, exported (class, enum, struct, union, typedef)
  
  class clock : private boost::noncopyable {
    
  public:

    static const timevalue   resolution;
    static const std::string name;
    
    static timevalue time ();        // wraps ::clock_gettime
    static timevalue time_of_day (); // wraps ::gettimeofday

  };
  
  
  // variables, exported (extern)
  
  // functions, inlined (inline)
  
  // functions, exported (extern)
  
  extern timevalue current_time ();
  extern timevalue current_time_of_day ();

} // namespace oglrwperf {

#endif // #if !defined(OGLRWPERF_SUPPORT_CLOCK_H)

// $Id: clock.h,v 1.1 2007/02/15 12:53:32 beck3 Exp $
