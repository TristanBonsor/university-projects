/*
 * Camera.hpp
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include <cmath>
#include <iostream>
#include "Camera.hpp"
#include "Defines.hpp"

Vector3d Camera::m_position = Vector3d(DEFAULT_CAMERA_POSITION);
Vector3d Camera::m_up       = Vector3d(DEFAULT_CAMERA_UP);
Vector3d Camera::m_at       = Vector3d(DEFAULT_CAMERA_AT);
Vector3d Camera::m_right    = Vector3d(DEFAULT_CAMERA_RIGHT);

float Camera::m_image_distance = DEFAULT_IMAGE_DISTANCE;

void Camera::translate(const Vector3d& v)
{
    m_position = m_position + v;
}

void Camera::pitch(const float& angle)
{
    m_up = rotate(angle, m_right, m_up);
    m_at = rotate(angle, m_right, m_at);

    m_up.normalize();
    m_at.normalize();

}

void Camera::yaw(const float& angle)
{
    m_right = rotate(angle, m_up, m_right);
    m_at = rotate(angle, m_up, m_at);

    m_right.normalize();
    m_at.normalize();
}

Vector3d Camera::getPosition()
{
    return m_position;
}

Vector3d Camera::getUp()
{
    return m_up;
}

Vector3d Camera::getAt()
{
    return m_at;
}

Vector3d Camera::getRight()
{
    return m_right;
}

float Camera::getImageDistance()
{
    return m_image_distance;
}

void Camera::setPosition(const Vector3d& p)
{
    m_position = p;
}

void Camera::setUp(const Vector3d& v)
{
    m_up = v;
}

void Camera::setAt(const Vector3d& v)
{
    m_at = v;
}

void Camera::setRight(const Vector3d& v)
{
    m_right = v;
}

void Camera::setImageDistance(const float& d)
{
    m_image_distance = d;
}

Vector3d Camera::rotate(const float& angle,
                        const Vector3d& axis,
                        const Vector3d& position)
{
    float ax = axis.getX();
    float ay = axis.getY();
    float az = axis.getZ();

    // Calculate the rotation matrix.

    float x1 = (1.0f - cos(angle)) * pow(ax, 2) + cos(angle);
    float x2 = (1.0f - cos(angle)) * ax * ay + az * sin(angle);
    float x3 = (1.0f - cos(angle)) * ax * az - ay * sin(angle);

    Vector3d column_vector_x(x1, x2, x3);

    float y1 = (1.0f - cos(angle)) * ax * ay - az * sin(angle);
    float y2 = (1.0f - cos(angle)) * pow(ay, 2) + cos(angle);
    float y3 = (1.0f - cos(angle)) * ay * az + ax * sin(angle);

    Vector3d column_vector_y(y1, y2, y3);

    float z1 = (1.0f - cos(angle)) * ax * az + ay * sin(angle);
    float z2 = (1.0f - cos(angle)) * ay * az - ax * sin(angle);
    float z3 = (1.0f - cos(angle)) * pow(az, 2) + cos(angle);

    Vector3d column_vector_z(z1, z2, z3);

    // Multiply the position vector by the rotation matrix.

    float x = Vector3d::dotProduct(position, column_vector_x);
    float y = Vector3d::dotProduct(position, column_vector_y);
    float z = Vector3d::dotProduct(position, column_vector_z);

    return Vector3d(x, y, z);
}
