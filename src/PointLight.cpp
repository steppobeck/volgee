
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



/// gloost system includes
#include <PointLight.h>



namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

PointLight::PointLight():
    _position(),
    _ambient(),
    _diffuse(),
    _specular(),
    _isSpotLight(false),
    _spotDirection(),
    _spotExponent(),
    _spotCutoff(),
    _spotNear(0.0f),
    _spotFar(10000.0f),
    _glLightNum(GL_LIGHT0)
{
  _ambient[0] = 0.1;
  _ambient[1] = 0.1;
  _ambient[2] = 0.1;
  _ambient[3] = 1.0;

  _diffuse[0] = 0.5;
  _diffuse[1] = 0.5;
  _diffuse[2] = 0.5;
  _diffuse[3] = 1.0;

  _specular[0] = 1.0;
  _specular[1] = 1.0;
  _specular[2] = 1.0;
  _specular[3] = 1.0;

  _position[0] = 0.0;
  _position[1] = 0.0;
  _position[2] = 0.0;
  _position[3] = 1.0;
}


///////////////////////////////////////////////////////////////////////////////


  /// class destructor

PointLight::~PointLight()
{}


///////////////////////////////////////////////////////////////////////////////


  ///

Point3
PointLight::getPosition()
{
  return Point3(_position[0], _position[1], _position[2]);
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setPosition(const Point3& position)
{
  _position[0] = (float) position[0];
  _position[1] = (float) position[1];
  _position[2] = (float) position[2];
  _position[3] = (float) 1.0;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setPosition(float x, float y, float z)
{
  _position[0] = x;
  _position[1] = y;
  _position[2] = z;
  _position[3] = 1.0;
}


///////////////////////////////////////////////////////////////////////////////


  ///

gloost::vec4
PointLight::getAmbient() const
{
  return gloost::vec4(_ambient[0], _ambient[1], _ambient[2], _ambient[3]);
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setAmbient(const float& r, const float& g, const float& b, const float& a)
{
  _ambient[0] = r;
  _ambient[1] = g;
  _ambient[2] = b;
  _ambient[3] = a;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setAmbient(const gloost::vec4& color)
{
  _ambient[0] = color.r;
  _ambient[1] = color.g;
  _ambient[2] = color.b;
  _ambient[3] = color.a;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setAmbient(const Point3& ambient)
{
  _ambient[0] = (float) ambient[0];
  _ambient[1] = (float) ambient[1];
  _ambient[2] = (float) ambient[2];
  _ambient[3] = (float) ambient[3];
}


///////////////////////////////////////////////////////////////////////////////


  ///

gloost::vec4
PointLight::getDiffuse() const
{
  return gloost::vec4(_diffuse[0], _diffuse[1], _diffuse[2], _diffuse[3]);
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setDiffuse(const float& r, const float& g, const float& b, const float& a)
{
  _diffuse[0] = r;
  _diffuse[1] = g;
  _diffuse[2] = b;
  _diffuse[3] = a;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setDiffuse(const gloost::vec4& color)
{
  _diffuse[0] = color.r;
  _diffuse[1] = color.g;
  _diffuse[2] = color.b;
  _diffuse[3] = color.a;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setDiffuse(const Point3& diffuse)
{
  _diffuse[0] = (float) diffuse[0];
  _diffuse[1] = (float) diffuse[1];
  _diffuse[2] = (float) diffuse[2];
  _diffuse[3] = (float) diffuse[3];
}


///////////////////////////////////////////////////////////////////////////////


  ///

gloost::vec4
PointLight::getSpecular() const
{
  return gloost::vec4(_specular[0], _specular[1], _specular[2], _specular[3]);
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setSpecular(const float& r, const float& g, const float& b, const float& a)
{
  _specular[0] = r;
  _specular[1] = g;
  _specular[2] = b;
  _specular[3] = a;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setSpecular(const gloost::vec4& color)
{
  _specular[0] = color.r;
  _specular[1] = color.g;
  _specular[2] = color.b;
  _specular[3] = color.a;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::setSpecular(const Point3& specular)
{
  _specular[0] = (float) specular[0];
  _specular[1] = (float) specular[1];
  _specular[2] = (float) specular[2];
  _specular[3] = (float) specular[3];
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::set(GLenum glLightNum)
{
  glEnable(GL_LIGHTING);
  glEnable(glLightNum);


  glLightfv(glLightNum, GL_POSITION, _position);

  glLightfv(glLightNum, GL_AMBIENT,  _ambient);
  glLightfv(glLightNum, GL_DIFFUSE,  _diffuse);
  glLightfv(glLightNum, GL_SPECULAR, _specular);

  if (_isSpotLight)
  {
    glLightfv(glLightNum, GL_SPOT_DIRECTION, _spotDirection);
    glLightf(glLightNum, GL_SPOT_EXPONENT,   _spotExponent);
    glLightf(glLightNum, GL_SPOT_CUTOFF,     _spotCutoff);
  }
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::disable()
{
  glDisable(_glLightNum);
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::enableSpot()
{
  _isSpotLight = true;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
PointLight::disableSpot()
{
  _isSpotLight = false;
}


///////////////////////////////////////////////////////////////////////////////


  ///

bool
PointLight::isSpotLight()
{
  return _isSpotLight;
}


///////////////////////////////////////////////////////////////////////////////

void
PointLight::setSpotDirection(const Vector3& spotDir)
{
  Vector3 nSpotDir = spotDir.normalized();

  _spotDirection[0] = nSpotDir[0];
  _spotDirection[1] = nSpotDir[1];
  _spotDirection[2] = nSpotDir[2];
}

///////////////////////////////////////////////////////////////////////////////


void
PointLight::setSpotExponent(float exponent)
{
  _spotExponent = exponent;
}

///////////////////////////////////////////////////////////////////////////////


void
PointLight::setSpotCutoff(float cutoff)
{
  _spotCutoff = cutoff;
}

///////////////////////////////////////////////////////////////////////////////

void
PointLight::setSpotNear(float near)
{
  _spotNear = near;
}

///////////////////////////////////////////////////////////////////////////////

void
PointLight::setSpotFar(float far)
{
  _spotFar = far;
}

///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////

} // namespace gloost
