#ifndef VOLGEE_HELPER_H
#define VOLGEE_HELPER_H

#include <StringTokenizer.h>

#include <gloostHelper.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>


#include <time.h>



namespace gee
{


/// returns the basename of the path of a file: "../../blaa.jpg" -> "bla.jpg"


inline std::string
basename(const char* word)
{
  sys::StringTokenizer st(word);
  return st.get_tokens("/")[st.get_tokens("/").size()-1];
}



 inline std::string
   makeDateString(){

   time_t t;
   time(&t);
   tm ts;
   localtime_r(&t,&ts);

   /*
     int tm_sec;
     int tm_min;
     int tm_hour;
     int tm_mday;
     int tm_mon;
     int tm_year;
   */
   ts.tm_mon += 1;
   std::string mon(gloost::toString(ts.tm_mon));
   if(ts.tm_mon < 10){
     mon = gloost::toString(0) + mon;
   }
   std::string day(gloost::toString(ts.tm_mday));
   if(ts.tm_mday < 10){
     day = gloost::toString(0) + day;
   }
   std::string hour(gloost::toString(ts.tm_hour));
   if(ts.tm_hour < 10){
     hour = gloost::toString(0) + hour;
   }
   std::string min(gloost::toString(ts.tm_min));
   if(ts.tm_min < 10){
     min = gloost::toString(0) + min;
   }
   std::string sec(gloost::toString(ts.tm_sec));
   if(ts.tm_sec < 10){
     sec = gloost::toString(0) + sec;
   }
   return gloost::toString(1900 + ts.tm_year)
     + mon
     + day
     + "_"
     + hour
     + min 
     + sec;
   
 }


  /// Draws a simple box using the given corners

inline void drawProxyOrig(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
{

  // front
  glBegin(GL_QUADS);
  glVertex3f(xmin, ymin, zmax);
  glVertex3f(xmax, ymin, zmax);
  glVertex3f(xmax, ymax, zmax);
  glVertex3f(xmin, ymax, zmax);
  glEnd();


  // right
  glBegin(GL_QUADS);
  glVertex3f(xmax, ymin, zmax);
  glVertex3f(xmax, ymin, zmin);
  glVertex3f(xmax, ymax, zmin);
  glVertex3f(xmax, ymax, zmax);
  glEnd();


  // back
  glBegin(GL_QUADS);
  glVertex3f(xmax, ymin, zmin);
  glVertex3f(xmin, ymin, zmin);
  glVertex3f(xmin, ymax, zmin);
  glVertex3f(xmax, ymax, zmin);
  glEnd();

  // left
  glBegin(GL_QUADS);
  glVertex3f(xmin, ymin, zmin);
  glVertex3f(xmin, ymin, zmax);
  glVertex3f(xmin, ymax, zmax);
  glVertex3f(xmin, ymax, zmin);
  glEnd();

  // top
  glBegin(GL_QUADS);
  glVertex3f(xmin, ymax, zmax);
  glVertex3f(xmax, ymax, zmax);
  glVertex3f(xmax, ymax, zmin);
  glVertex3f(xmin, ymax, zmin);
  glEnd();

  // bottom
  glBegin(GL_QUADS);
  glVertex3f(xmin, ymin, zmin);
  glVertex3f(xmax, ymin, zmin);
  glVertex3f(xmax, ymin, zmax);
  glVertex3f(xmin, ymin, zmax);
  glEnd();

}


  /// Draws a simple box using the given corners

 inline void drawProxy(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax, bool slow = false)
{


  float pos;
  const unsigned int num_steps = (slow == false) ? 2 : 10;
  const float step = 1.0/num_steps;
  

  // front
  glBegin(GL_QUADS);
  pos = xmin;
  for(unsigned int i = 0; i < num_steps; ++i){
    glTexCoord2f(0.0,0.0);
    glVertex3f(pos, ymin, zmax);
    glTexCoord2f(1.0,0.0);
    glVertex3f(pos + step, ymin, zmax);
    glTexCoord2f(1.0,1.0);
    glVertex3f(pos + step, ymax, zmax);
    glTexCoord2f(0.0,1.0);
    glVertex3f(pos, ymax, zmax);
    pos += step;
    
  }
  glEnd();


  // right
  glBegin(GL_QUADS);
  pos = zmax;
  for(unsigned int i = 0; i < num_steps; ++i){
    glTexCoord2f(0.0,0.0);
    glVertex3f(xmax, ymin, pos);
    glTexCoord2f(1.0,0.0);
    glVertex3f(xmax, ymin, pos - step);
    glTexCoord2f(1.0,1.0);
    glVertex3f(xmax, ymax, pos - step);
    glTexCoord2f(0.0,1.0);
    glVertex3f(xmax, ymax, pos);
    pos -= step;
    
  }
  glEnd();


  // back
  glBegin(GL_QUADS);
  pos = xmax;
  for(unsigned int i = 0; i < num_steps; ++i){
    glTexCoord2f(0.0,0.0);
    glVertex3f(pos, ymin, zmin);
    glTexCoord2f(1.0,0.0);
    glVertex3f(pos - step, ymin, zmin);
    glTexCoord2f(1.0,1.0);
    glVertex3f(pos - step, ymax, zmin);
    glTexCoord2f(0.0,1.0);
    glVertex3f(pos, ymax, zmin);
    pos -= step;
    
  }
  glEnd();

  // left
  glBegin(GL_QUADS);
  pos = zmin;
  for(unsigned int i = 0; i < num_steps; ++i){
    glTexCoord2f(0.0,0.0);
    glVertex3f(xmin, ymin, pos);
    glTexCoord2f(1.0,0.0);
    glVertex3f(xmin, ymin, pos + step);
    glTexCoord2f(1.0,1.0);
    glVertex3f(xmin, ymax, pos + step);
    glTexCoord2f(0.0,1.0);
    glVertex3f(xmin, ymax, pos);
    pos += step;
    
  }
  glEnd();

  // top
  glBegin(GL_QUADS);

  pos = xmin;
  for(unsigned int i = 0; i < num_steps; ++i){
    glTexCoord2f(0.0,0.0);
    glVertex3f(pos, ymax, zmax);
    glTexCoord2f(1.0,0.0);
    glVertex3f(pos + step, ymax, zmax);
    glTexCoord2f(1.0,1.0);
    glVertex3f(pos + step, ymax, zmin);
    glTexCoord2f(0.0,1.0);
    glVertex3f(pos, ymax, zmin);
    pos += step;
    
  }
  glEnd();

  // bottom
  glBegin(GL_QUADS);

  pos = xmin;
  for(unsigned int i = 0; i < num_steps; ++i){
    glTexCoord2f(0.0,0.0);
    glVertex3f(pos, ymin, zmin);
    glTexCoord2f(1.0,0.0);
    glVertex3f(pos + step, ymin, zmin);
    glTexCoord2f(1.0,1.0);
    glVertex3f(pos + step, ymin, zmax);
    glTexCoord2f(0.0,1.0);
    glVertex3f(pos, ymin, zmax);
    pos += step;
    
  }
  glEnd();

}



}

#endif // #VOLGEE_HELPER_H
