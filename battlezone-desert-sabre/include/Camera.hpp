/*
 * \file Camera.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <GL/gl.h>
#include <GL/glu.h>

#include "Entity.hpp"
#include "Quaternion.hpp"

class Camera
{
public:

    inline static void translateTo(const Vector3d& position);

    inline static void rotateTo(const float& angle, const Vector3d& axis);

    inline static void display();

protected:

    static const Vector3d M_DEFAULT_UP;
    static const Vector3d M_DEFAULT_AT;
    static const float M_FOCUS_RADIUS;

    static Vector3d m_position;
    static Quaternion m_orientation;
};

inline void Camera::translateTo(const Vector3d& position)
{
    m_position = position;
}

inline void Camera::rotateTo(const float& angle, const Vector3d& axis)
{
    m_orientation = Quaternion(angle, axis);
}

inline void Camera::display()
{
    m_orientation.normalize();

    Vector3d at = m_orientation * M_DEFAULT_AT;
    Vector3d up = m_orientation * M_DEFAULT_UP;

    at.normalize();
    up.normalize();

    Vector3d center = m_position + at * M_FOCUS_RADIUS;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(m_position.x,
              m_position.y,
              m_position.z,
              center.x,
              center.y,
              center.z,
              up.x,
              up.y,
              up.z);
}

#endif /* CAMERA_HPP_ */
