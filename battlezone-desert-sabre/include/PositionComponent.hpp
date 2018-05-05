/**
 * \file PositionComponent.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef POSITIONCOMPONENT_HPP_
#define POSITIONCOMPONENT_HPP_

#include "Entity.hpp"
#include "Vector3d.hpp"

/**
 * \class PositionComponent PositionComponent.hpp "include/PositionComponent.hpp"
 * \brief The position component for an entity.
 *
 * This class provides a position component for an entity; provides position
 * translation functionality as well as position data access.
 */
class PositionComponent: public Entity::Component
{
public:

    /**
     * Default constructor.
     * Initializes component with position vector <0, 0, 0>.
     */
    PositionComponent()
        : m_position()
    {
    }

    /**
     * Explicit constructor.
     * Initializes component with position vector.
     * @param p The position vector in R3.
     */
    PositionComponent(const Vector3d& p)
        : m_position(p)
    {
    }

    virtual ~PositionComponent()
    {
    }

    /**
     * Get the current position.
     * @return A position vector in R3.
     */
    inline Vector3d position() const;

    /**
     * Translate the current position by the given displacement vector.
     * @param d A displacement vector in R3.
     * @return A reference to the position component.
     */
    inline PositionComponent& translateBy(const Vector3d& d);

    /**
     * Translate the current position to the given position vector.
     * @param p A position vector in R3.
     * @return A reference to the position component.
     */
    inline PositionComponent& translateTo(const Vector3d& p);

protected:

    Vector3d m_position; //!< Current position.
};

inline Vector3d PositionComponent::position() const
{
    return m_position;
}

inline PositionComponent& PositionComponent::translateBy(const Vector3d& d)
{
    m_position = m_position + d;
    return *this;
}

inline PositionComponent& PositionComponent::translateTo(const Vector3d& p)
{
    m_position = p;
    return *this;
}

#endif /* POSITIONCOMPONENT_HPP_ */
