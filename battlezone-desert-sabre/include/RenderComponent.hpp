/*
 * \file RenderComponent.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef RENDERCOMPONENT_HPP_
#define RENDERCOMPONENT_HPP_

#include "Entity.hpp"
#include "Vector3d.hpp"

class RenderComponent: public Entity::Component
{
public:

    virtual ~RenderComponent()
    {
    }

    virtual inline void render() = 0;

    inline RenderComponent& translateTo(const Vector3d& position);

    inline RenderComponent& rotateTo(const float& angle, const Vector3d& axis);

protected:

    Vector3d m_position;
    float m_angle;
    Vector3d m_axis;
};

inline RenderComponent& RenderComponent::translateTo(const Vector3d& position)
{
    m_position = position;
    return *this;
}

inline RenderComponent& RenderComponent::rotateTo(const float& angle,
                                                  const Vector3d& axis)
{
    m_angle = angle;
    m_axis = axis;
    return *this;
}

#endif /* RENDERCOMPONENT_HPP_ */
