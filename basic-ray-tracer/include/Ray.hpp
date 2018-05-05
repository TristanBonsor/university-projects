/**
 * \file Ray.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef RAY_HPP_
#define RAY_HPP_

#include "Vector3d.hpp"

/**
 * \class Ray Ray.hpp "include/Ray.hpp"
 * \brief ADT for a ray in R3.
 *
 * This class provides basic properties, behaviour, and operations for a ray in
 * R3.
 */
class Ray
{
public:

    /**
     * \class Ray::Contact Ray.hpp "include/Ray.hpp"
     * \brief ADT for contact between a ray and another object.
     *
     * This class provides a basic object for accessing contact data generated
     * between a ray and an object in R3.
     */
    class Contact
    {
    public:

        /**
         * Default constructor.
         * Initializes a contact object with no parametric data and contact
         * state set to false.
         */
        Contact()
            : m_is_contact(false), m_parameter(0.0f)
        {
        }

        /**
         * Explicit constructor.
         * Initializes a contact object with parametric data and contact state
         * set to true.
         * @param param The parametric data that describes the point of contact
         * along a ray.
         */
        Contact(const float& param)
            : m_is_contact(true), m_parameter(param)
        {
        }

        /**
         * Query if there is contact.
         * @return True if there is contact, false otherwise.
         */
        bool isContact() const;

        /**
         * Get the parametric data that describes the point of contact along a
         * ray.
         * @return The parametric value that describes the point of contact along
         * a ray.
         */
        float getParameter() const;

    protected:

        /**
         * Is there contact between a ray and a surface?
         */
        bool m_is_contact;

        /**
         * The parametric value indicating the position along the ray where the
         * surface is hit.
         */
        float m_parameter;
    };

    /**
     * Explicit constructor.
     * Initialize the ray with an origin position vector and a propagation
     * direction vector.
     * @param origin The origin position vector in R3.
     * @param direction The propagation direction vector in R3.
     */
    Ray(const Vector3d& origin, const Vector3d& direction)
        : m_origin(origin), m_direction(direction)
    {
    }

    /**
     * Calculate the position vector of a given point on the ray.
     * @param t The unit parameter of the ray.
     * @return A position vector in R3.
     */
    Vector3d interpolate(const float& t) const;

    /**
     * Get the origin position vector of the ray.
     * @return A position vector in R3.
     */
    Vector3d getOrigin() const;

    /**
     * Get the propagation direction vector of the ray.
     * @return A direction vector in R3.
     */
    Vector3d getDirection() const;

    /**
     * Set the origin position vector of the ray.
     * @param origin A position vector in R3.
     */
    void setOrigin(const Vector3d& origin);

    /**
     * Set the propagation direction vector of the ray.
     * @param direction A direction vector in R3.
     */
    void setDirection(const Vector3d& direction);

protected:

    /**
     * The ray origin.
     */
    Vector3d m_origin;

    /**
     * The ray propagation direction.
     */
    Vector3d m_direction;
};

#endif /* RAY_HPP_ */
