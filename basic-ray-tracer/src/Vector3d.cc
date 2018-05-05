/*
 * Vector3d.cc
 * Vector3d implementation.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include <cmath>

#include "Vector3d.hpp"

float Vector3d::dotProduct(const Vector3d& u, const Vector3d& v)
{
    return u.m_x * v.m_x + u.m_y * v.m_y + u.m_z * v.m_z;
}

Vector3d Vector3d::crossProduct(const Vector3d& u, const Vector3d& v)
{
    float x = u.m_y * v.m_z - u.m_z * v.m_y;
    float y = u.m_z * v.m_x - u.m_x * v.m_z;
    float z = u.m_x * v.m_y - u.m_y * v.m_x;

    return Vector3d(x, y, z);
}

Vector3d Vector3d::interpolate(const Vector3d& u,
                               const Vector3d& v,
                               const float& t)
{
    float x = u.m_x + t * (v.m_x - u.m_x);
    float y = u.m_y + t * (v.m_y - u.m_y);
    float z = u.m_z + t * (v.m_z - u.m_z);

    return Vector3d(x, y, z);
}

Vector3d& Vector3d::operator=(const Vector3d& rhs)
{
    if(this != &rhs)
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
        m_z = rhs.m_z;
    }

    return *this;
}

Vector3d Vector3d::operator+(const Vector3d& rhs) const
{
    float x = m_x + rhs.m_x;
    float y = m_y + rhs.m_y;
    float z = m_z + rhs.m_z;

    return Vector3d(x, y, z);
}

Vector3d Vector3d::operator-(const Vector3d& rhs) const
{
    float x = m_x - rhs.m_x;
    float y = m_y - rhs.m_y;
    float z = m_z - rhs.m_z;

    return Vector3d(x, y, z);
}

bool Vector3d::operator==(const Vector3d& rhs) const
{
    return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z;
}

bool Vector3d::operator!=(const Vector3d& rhs) const
{
    return !(this->operator==(rhs));
}

float Vector3d::getMagnitude() const
{
    return sqrt(dotProduct(*this, *this));
}

Vector3d Vector3d::getNormalized() const
{
    Vector3d u(m_x, m_y, m_z);
    u.normalize();
    return u;
}

void Vector3d::normalize()
{
    float norm_inverse = 1.0f / getMagnitude();

    m_x = norm_inverse * m_x;
    m_y = norm_inverse * m_y;
    m_z = norm_inverse * m_z;
}

Vector3d Vector3d::getScaled(const float& c) const
{
    float x = c * m_x;
    float y = c * m_y;
    float z = c * m_z;

    return Vector3d(x, y, z);
}

void Vector3d::scale(const float& c)
{
    m_x = c * m_x;
    m_y = c * m_y;
    m_z = c * m_z;
}

float Vector3d::getX() const
{
    return m_x;
}

float Vector3d::getY() const
{
    return m_y;
}

float Vector3d::getZ() const
{
    return m_z;
}

void Vector3d::setX(const float& x)
{
    m_x = x;
}

void Vector3d::setY(const float& y)
{
    m_y = y;
}

void Vector3d::setZ(const float& z)
{
    m_z = z;
}
