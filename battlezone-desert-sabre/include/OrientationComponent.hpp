/**
 * \file OrientationComponent.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef ORIENTATIONCOMPONENT_HPP_
#define ORIENTATIONCOMPONENT_HPP_

#include "Entity.hpp"
#include "Quaternion.hpp"

/**
 * \class OrientationComponent OrientationComponent.hpp "include/OrientationComponent.hpp"
 * \brief The orientation component for an entity.
 *
 * This class provides an orientation component for an entity; provides
 * orientation rotation functionality as well as orientation data access.
 */
class OrientationComponent: public Entity::Component
{
public:

    /**
     * Default constructor.
     * Initializes component with orientation quaternion <1, 0, 0, 0>.
     */
    OrientationComponent()
        : m_orientation()
    {
    }

    /**
     * Explicit constructor.
     * Initializes component with orientation quaternion.
     * @param o The orientation quaternion.
     */
    OrientationComponent(const Quaternion& o)
        : m_orientation(o)
    {
    }

    virtual ~OrientationComponent()
    {
    }

    /**
     * Get the current orientation.
     * @return A quaternion representing the current orientation.
     */
    inline Quaternion orientation() const;

    /**
     * Rotate the current orientation by an angle about an axis.
     * @param angle The angle of rotation.
     * @param axis The axis of rotation.
     * @return A reference to the orientation component.
     */
    inline OrientationComponent& rotateBy(const float& angle,
                                          const Vector3d& axis);

    /**
     * Rotate the current orientation to an angle about an axis.
     * @param angle The angle of rotation.
     * @param axis The axis of rotation.
     * @return A reference to the orientation component.
     */
    inline OrientationComponent& rotateTo(const float& angle,
                                          const Vector3d& axis);

protected:

    Quaternion m_orientation; //!< Current orientation.
};

inline Quaternion OrientationComponent::orientation() const
{
    return m_orientation;
}

inline OrientationComponent& OrientationComponent::rotateBy(const float& angle,
                                                            const Vector3d& axis)
{
    Quaternion rotation(angle, axis);
    m_orientation = rotation * m_orientation; // Accumulate rotations: second * first.
    m_orientation.normalize();
    return *this;
}

inline OrientationComponent& OrientationComponent::rotateTo(const float& angle,
                                                            const Vector3d& axis)
{
    m_orientation = Quaternion(angle, axis);
    m_orientation.normalize();
    return *this;
}

#endif /* ORIENTATIONCOMPONENT_HPP_ */
