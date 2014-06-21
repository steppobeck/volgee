#ifndef GEE_TRACKBALL_H
#define GEE_TRACKBALL_H


#include <gee_element.h>

#include <Matrix.h>
#include <Point3.h>

#include <arcball/arcball.hpp>


namespace gee{

  typedef double floating_type;

  class gee_trackball : public gee_element{

  public:
    
    gee_trackball(floating_type zoom, float zoom_step);
    ~gee_trackball();

    bool mouse(int button, int state, int x, int y);
    bool motion(int x, int y);
    void set_bounds(int min_x, int max_x, int min_y, int max_y);

    void draw(int viewport_width, int viewport_height);

    const gloost::Matrix& get_matrix();
    const gloost::Matrix& get_rot_matrix();
    void reset_rot_matrix();

    float get_zoom() const;
    

    void setLookAt(gloost::Point3& center);

    void set_cb_start(void (* callback)( int ), int index);
    void set_cb_stop(void (* callback)( int ), int index);
    void set_cb_drag(void (* callback)( int ), int index);

  private:
    gl::ArcBall* m_arcball;
    floating_type m_zoom;
    float m_zoom_step;
    gloost::Matrix m_mat;
    unsigned int m_mode;
    gloost::Point3 m_center;
    math::vec3 m_arcball_center;

    void (*m_cb_start)(int);
    void (*m_cb_stop)(int);
    void (*m_cb_drag)(int);

    int m_index_cb_start;
    int m_index_cb_stop;
    int m_index_cb_drag;

  };


}



#endif // #ifndef GEE_TRACKBALL_H
