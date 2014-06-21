#ifdef LINUX
#ifndef TIMER_H
#define TIMER_H

#include <timevalue.h>

namespace sys{

  class Timer{

  public:
    Timer();
    ~Timer();



    void step();

    unsigned int get_msec();
    float get_fps();


  private:

    oglrwperf::timevalue m_ts;
    unsigned int m_msec;

  };

}




#endif // #ifndef TIMER_H
#endif
