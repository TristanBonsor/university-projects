/**
 * \file LightSource.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef LIGHTSOURCE_HPP_
#define LIGHTSOURCE_HPP_

#include "Vector3d.hpp"

/**
 * \class LightSource LightSource.hpp "include/LightSource.hpp"
 * \brief A source of light within a scene.
 *
 * This class provides all behaviour and state of a given light source in a
 * scene.
 */
class LightSource
{
public:

    /**
     * Explicit constructor.
     * @param position The position of the light source.
     * @param red_intensity The red intensity of the light source.
     * @param green_intensity The green intensity of the light source.
     * @param blue_intensity The blue intensity of the light source.
     */
    LightSource(const Vector3d& position,
                const float& red_intensity,
                const float& green_intensity,
                const float& blue_intensity)
        : m_position(position),
          m_red_intensity(red_intensity),
          m_green_intensity(green_intensity),
          m_blue_intensity(blue_intensity)
    {
    }

    /**
     * Get the position of the light source.
     * @return A position vector in R3.
     */
    Vector3d getPosition() const;

    /**
     * Get the red intensity of the light source.
     * @return A value in the interval [0,1].
     */
    float getRedIntensity() const;

    /**
     * Get the green intensity of the light source.
     * @return A value in the interval [0,1].
     */
    float getGreenIntensity() const;

    /**
     * Get the blue intensity of the light source.
     * @return A value in the interval [0,1].
     */
    float getBlueIntensity() const;

protected:

    /**
     * The position of the light source.
     */
    Vector3d m_position;

    /**
     * The red intensity of the light source.
     */
    float m_red_intensity;

    /**
     * The green intensity of the light source.
     */
    float m_green_intensity;

    /**
     * The blue intensity of the light source.
     */
    float m_blue_intensity;
};

#endif /* LIGHTSOURCE_HPP_ */
