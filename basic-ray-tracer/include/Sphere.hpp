/**
 * \file Sphere.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef SPHERE_HPP_
#define SPHERE_HPP_

#include "Surface.hpp"

/**
 * \class Sphere Sphere.hpp "include/Sphere.hpp"
 * \brief Represents a sphere in R3.
 *
 * This class provides basic properties, behaviour, and operations for a sphere
 * in R3.
 */
class Sphere: public Surface
{
public:

    /**
     * Explicit constructor.
     * Initializes a sphere at a given position vector with a given radius.
     * @param surface_color The surface color of the surface.
     * @param specular_color The specular color of the surface.
     * @param ambient_color The ambient color of the surface.
     * @param center A position vector in R3.
     * @param radius The radius of the sphere.
     */
    Sphere(const CRGB& surface_color,
           const CRGB& specular_color,
           const CRGB& ambient_color,
           const Vector3d& center,
           const float& radius)
        : Surface(surface_color, specular_color, ambient_color),
          m_center(center),
          m_radius(radius)
    {
    }

    virtual ~Sphere()
    {
    }

    /**
     * Determines if sphere has been hit by a ray on the interval [t0,t1].
     * @param ray The ray we wish to use for hit resolution.
     * @param t0 Beginning unit parameter of interval along the ray.
     * @param t1 Ending unit parameter of interval along the ray.
     * @return A ray-surface contact object.
     */
    virtual Ray::Contact hit(const Ray& ray,
                             const float& t0,
                             const float& t1) const;

    /**
     * Determines the normal vector of the surface of a sphere at a given point.
     * @param p A position vector in R3.
     * @return A vector, in R3, of the normal to the surface of the sphere
     * at a given point.
     */
    virtual Vector3d getNormal(const Vector3d& p) const;

protected:

    /**
     * The center of the sphere.
     */
    Vector3d m_center;

    /**
     * The radius of the sphere.
     */
    float m_radius;
};

#endif /* SPHERE_HPP_ */
