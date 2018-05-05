/*
 * \file TankEntity.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef TANKENTITY_HPP_
#define TANKENTITY_HPP_

#include "TankControlComponent.hpp"

class TankEntity: public Entity
{
public:

    TankEntity(const unsigned int id,
               const Vector3d& position = Vector3d(),
               const float& angle = 0.0f,
               const Vector3d& axis = Vector3d(),
               const float& turn_radius = 1.0f)
        : Entity(id)
    {
        attachComponent(std::shared_ptr<Entity::Component>(new PositionComponent(position)));
        attachComponent(std::shared_ptr<Entity::Component>(new OrientationComponent(Quaternion(angle,
                                                                                               axis))));
        attachComponent(std::shared_ptr<Entity::Component>(new VelocityComponent()));
        attachComponent(std::shared_ptr<Entity::Component>(new PhysicsComponent()));
        attachComponent(std::shared_ptr<Entity::Component>(new TankControlComponent(turn_radius)));
    }

    virtual ~TankEntity()
    {
    }

    virtual inline void update(const float& t, const float& dt);

protected:

    inline PhysicsComponent::PhysicsState getPhysicsState(const float& alpha) const;
};

inline void TankEntity::update(const float& t, const float& dt)
{
    getComponent<TankControlComponent>()->process();
    getComponent<PhysicsComponent>()->update(t, dt);
}

inline PhysicsComponent::PhysicsState TankEntity::getPhysicsState(const float& alpha) const
{
    return getComponent<PhysicsComponent>()->interpolate(alpha);
}

#endif /* TANKENTITY_HPP_ */
