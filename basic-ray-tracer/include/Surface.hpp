/**
 * \file Surface.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef SURFACE_HPP_
#define SURFACE_HPP_

#include <vector>

#include "color.h"
#include "Ray.hpp"
#include "Vector3d.hpp"

/**
 * \class Surface Surface.hpp "include/Surface.hpp"
 * \brief Abstract interface for surfaces in R3.
 *
 * This class provides a basic interface common to all surfaces in R3.
 */
class Surface
{
public:

    /**
     * Explicit constructor.
     * @param surface_color The surface color of the surface.
     * @param specular_color The specular color of the surface.
     * @param ambient_color The ambient color of the surface.
     */
    Surface(const CRGB& surface_color,
            const CRGB& specular_color,
            const CRGB& ambient_color)
        : m_surface_color(surface_color),
          m_specular_color(specular_color),
          m_ambient_color(ambient_color)
    {
    }

    virtual ~Surface()
    {
    }

    /**
     * Determines if surface has been hit by a ray on the interval [t0,t1].
     * @param ray The ray we wish to use for hit resolution.
     * @param t0 Beginning unit parameter of interval along the ray.
     * @param t1 Ending unit parameter of interval along the ray.
     * @return A ray-surface contact object.
     */
    virtual Ray::Contact hit(const Ray& ray,
                             const float& t0,
                             const float& t1) const = 0;

    /**
     * Determines the normal vector of the surface at a given point.
     * @param p A position vector in R3.
     * @return A vector, in R3, of the normal to the surface at a given point.
     */
    virtual Vector3d getNormal(const Vector3d& p) const = 0;

    /**
     * Get the surface color.
     * @return The surface color.
     */
    virtual CRGB getSurfaceColor() const;

    /**
     * Get the specular color.
     * @return The specular color.
     */
    virtual CRGB getSpecularColor() const;

    /**
     * Get the ambient color.
     * @return The ambient color.
     */
    virtual CRGB getAmbientColor() const;

protected:

    /**
     * The surface color.
     */
    CRGB m_surface_color;

    /**
     * The specular color.
     */
    CRGB m_specular_color;

    /**
     * The ambient color.
     */
    CRGB m_ambient_color;
};

#endif /* SURFACE_HPP_ */
