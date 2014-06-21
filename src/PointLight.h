
/*
                       ___                            __
                      /\_ \                          /\ \__
                   __ \//\ \     ___     ___     ____\ \  _\
                 /'_ `\ \ \ \   / __`\  / __`\  /  __\\ \ \/
                /\ \ \ \ \_\ \_/\ \ \ \/\ \ \ \/\__   \\ \ \_
                \ \____ \/\____\ \____/\ \____/\/\____/ \ \__\
                 \/___/\ \/____/\/___/  \/___/  \/___/   \/__/
                   /\____/
                   \_/__/

                   OpenGL framework for fast demo programming

                             http://www.gloost.org

    This file is part of the gloost framework. You can use it in parts or as
       whole under the terms of the GPL (http://www.gnu.org/licenses/#GPL).

            gloost is being created by Felix Weiﬂig and Stephan Beck

     Felix Weiﬂig (thesleeper@gmx.net), Stephan Beck (stephan@pixelstars.de)
*/



#ifndef GLOOST_POINTLIGHT_H
#define GLOOST_POINTLIGHT_H


/// gloost includes
#include <Point3.h>
#include <Vector3.h>
#include <UniformTypes.h>


// system includes
#include <GL/glew.h>
#include <GL/gl.h>


namespace gloost
{


/// wrapper for a openGL point light and base class for all kinds of point lights

class PointLight
{

 public:

    /// class constructor
    PointLight();
    virtual ~PointLight();

    Point3 getPosition();
    void setPosition(const Point3& position);
    void setPosition(float x, float y, float z);


    gloost::vec4 getAmbient() const;
    void setAmbient(const float& r, const float& g, const float& b, const float& a);
    void setAmbient(const gloost::vec4& color);
    void setAmbient(const Point3& ambient);

    gloost::vec4 getDiffuse() const;
    void setDiffuse(const float& r, const float& g, const float& b, const float& a);
    void setDiffuse(const gloost::vec4& color);
    void setDiffuse(const Point3& diffuse);

    gloost::vec4 getSpecular() const;
    void setSpecular(const float& r, const float& g, const float& b, const float& a);
    void setSpecular(const gloost::vec4& color);
    void setSpecular(const Point3& specular);




  /// set the light to gl state
  void set(GLenum glLightNum = GL_LIGHT0);
  void disable();




  void enableSpot();
  void disableSpot();
  bool isSpotLight();


  void setSpotDirection(const Vector3& spotDir);
  void setSpotExponent(float exponent);

  /// _spotCutoff is 1/2 of the spot lights coneangle (in degrees)
  void setSpotCutoff(float cutoff);

  /// area (in light direction) the light is working
  void setSpotNear(float near);
  void setSpotFar(float far);


 protected:

  GLfloat _position[4];

  GLfloat _ambient[4];
  GLfloat _diffuse[4];
  GLfloat _specular[4];

  bool _isSpotLight;

  float _spotDirection[3];
  float _spotExponent;
  float _spotCutoff;

  float _spotNear;
  float _spotFar;

  GLenum _glLightNum;

};


} // namespace gloost


#endif // GLOOST_POINTLIGHT_H
