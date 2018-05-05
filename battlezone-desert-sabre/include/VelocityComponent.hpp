/**
 * \file VelocityComponent.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef VELOCITYCOMPONENT_HPP_
#define VELOCITYCOMPONENT_HPP_

#include "Entity.hpp"
#include "Quaternion.hpp"

/**
 * \class VelocityComponent VelocityComponent.hpp "include/VelocityComponent.hpp"
 * \brief The velocity component for an entity.
 *
 * This class provides a velocity component for an entity; provides linear and
 * angular velocity functionality as well as data access.
 */
class VelocityComponent: public Entity::Component
{
public:

    /**
     * Default constructor.
     * Initializes linear velocity vector to <0, 0, 0> and angular velocity
     * quaternion to <1, 0, 0, 0>.
     */
    VelocityComponent()
        : m_linear(), m_angular()
    {
    }

    /**
     * Explicit constructor.
     * Initializes linear velocity vector and angular velocity quaternion to
     * the specified values.
     * @param linear The linear velocity vector in R3.
     * @param angle The angular velocity in radians per second.
     * @param axis The axis about which the angular velocity occurs.
     */
    VelocityComponent(const Vector3d& linear,
                      const float& angle,
                      const Vector3d& axis)
        : m_linear(linear), m_angular(angle, axis)
    {
    }

    virtual ~VelocityComponent()
    {
    }

    /**
     * Get the current linear velocity.
     * @return The current linear velocity vector in R3.
     */
    inline Vector3d linear() const;

    /**
     * Set the current linear velocity.
     * @param v The linear velocity in R3.
     * @return A reference to the velocity component.
     */
    inline VelocityComponent& linear(const Vector3d& v);

    /**
     * Get the current angular velocity and axis of rotation.
     * @return A quaternion representing the current angular velocity and axis
     * of rotation.
     */
    inline Quaternion angular() const;

    /**
     * Set the current angular velocity and axis of rotation.
     * @param angle The angular velocity.
     * @param axis The axis about which the angular velocity occurs.
     * @return A reference to the velocity component.
     */
    inline VelocityComponent& angular(const float& angle, const Vector3d& axis);

protected:

    Vector3d m_linear;    //!< Current linear velocity.
    Quaternion m_angular; //!< Current angular velocity.
};

inline Vector3d VelocityComponent::linear() const
{
    return m_linear;
}

inline VelocityComponent& VelocityComponent::linear(const Vector3d& v)
{
    m_linear = v;
    return *this;
}

inline Quaternion VelocityComponent::angular() const
{
    return m_angular;
}

inline VelocityComponent& VelocityComponent::angular(const float& angle,
                                                     const Vector3d& axis)
{
    m_angular = Quaternion(angle, axis);
    return *this;
}

#endif /* VELOCITYCOMPONENT_HPP_ */
