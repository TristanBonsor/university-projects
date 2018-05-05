/*
 * Ray.cc
 * Ray implementation.
 *
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include "Ray.hpp"

bool Ray::Contact::isContact() const
{
    return m_is_contact;
}

float Ray::Contact::getParameter() const
{
    return m_parameter;
}

Vector3d Ray::interpolate(const float& t) const
{
    return Vector3d::interpolate(m_origin, m_direction, t);
}

Vector3d Ray::getOrigin() const
{
    return m_origin;
}

Vector3d Ray::getDirection() const
{
    return m_direction;
}

void Ray::setOrigin(const Vector3d& origin)
{
    m_origin = origin;
}

void Ray::setDirection(const Vector3d& direction)
{
    m_direction = direction;
}
