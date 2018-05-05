/**
 * Sphere.cc
 * Copyright (C) 2014 Tristan W. Bonsor
 */

#include <cmath>

#include "Sphere.hpp"

Ray::Contact Sphere::hit(const Ray& ray, const float& t0, const float& t1) const
{
    Vector3d e = ray.getOrigin();
    Vector3d d = ray.getDirection();
    Vector3d c = m_center;
    float r = m_radius;

    float A = Vector3d::dotProduct(d, d);
    float B = Vector3d::dotProduct(d.getScaled(2), e - c);
    float C = Vector3d::dotProduct(e - c, e - c) - (r * r);

    float discriminant = (B * B) - (4 * A * C);

    if(discriminant < 0)
    {
        return Ray::Contact();
    }

    float t = (-B - sqrt(discriminant)) / (2.0f * A);

    if(t >= t0 && t <= t1)
    {
        return Ray::Contact(t);
    }

    return Ray::Contact();
}

Vector3d Sphere::getNormal(const Vector3d& p) const
{
    Vector3d n = (p - m_center).getScaled(2.0f);
    return n;
}
