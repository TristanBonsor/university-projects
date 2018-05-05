/*
 * \file PlayerTankEntity.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef PLAYERTANKENTITY_HPP_
#define PLAYERTANKENTITY_HPP_

#include "TankEntity.hpp"
#include "TankRenderComponent.hpp"

class PlayerTankEntity: public TankEntity
{
public:

    PlayerTankEntity(const unsigned int& id,
                     const Vector3d& position = Vector3d(),
                     const float& angle = 0.0f,
                     const Vector3d& axis = Vector3d(),
                     const float& turn_radius = 1.0f)
         : TankEntity(id, position, angle, axis, turn_radius)
    {
        attachComponent(std::shared_ptr<Entity::Component>(new TankRenderComponent()));
    }

    virtual ~PlayerTankEntity()
    {
    }

    virtual inline void render(const float& alpha);

protected:

};

inline void PlayerTankEntity::render(const float& alpha)
{
    float angle;
    Vector3d axis;

    PhysicsComponent::PhysicsState state = getPhysicsState(alpha);

    state.orientation().angleAxis(angle, axis);

    std::shared_ptr<TankRenderComponent> renderable = getComponent<
        TankRenderComponent>();

    renderable->translateTo(state.position()).rotateTo(angle, axis);
    renderable->render();
}
#endif /* PLAYERTANKENTITY_HPP_ */
