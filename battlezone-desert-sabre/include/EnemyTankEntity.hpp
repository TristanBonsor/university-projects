/*
 * \file EnemyTankEntity.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef ENEMYTANKENTITY_HPP_
#define ENEMYTANKENTITY_HPP_

#include "TankEntity.hpp"
#include "TankRenderComponent.hpp"

class EnemyTankEntity: public TankEntity
{
public:

    EnemyTankEntity(const unsigned int& id,
                    const Vector3d& position = Vector3d(),
                    const float& angle = 0.0f,
                    const Vector3d& axis = Vector3d(),
                    const float& turn_radius = 1.0f)
        : TankEntity(id, position, angle, axis, turn_radius), m_is_shot_desired(false)
    {
        attachComponent(std::shared_ptr<Entity::Component>(new TankRenderComponent()));
    }

    virtual ~EnemyTankEntity()
    {
    }

    virtual inline void update(const float& t, const float& dt);

    virtual inline void render(const float& alpha);

protected:

    bool m_is_shot_desired;
};

inline void EnemyTankEntity::update(const float& t, const float& dt)
{
    TankEntity::update(t, dt);
}

inline void EnemyTankEntity::render(const float& alpha)
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

#endif /* ENEMYTANKENTITY_HPP_ */
