#ifdef LINUX
#include "Timer.h"

#include <clock.h>
#include <cmath>

namespace sys{

Timer::Timer()
  : m_ts(oglrwperf::clock::time()),
    m_msec()
{}


Timer::~Timer()
{}


void
Timer::step(){
  oglrwperf::timevalue ts_now(oglrwperf::clock::time());
  m_msec = (ts_now - m_ts).msec();
  m_ts = ts_now;
}



unsigned int
Timer::get_msec(){
  return m_msec;
}


float
Timer::get_fps(){
  return std::floor(1000.0f/m_msec);
}

}
#endif
