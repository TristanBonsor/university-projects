/**
 * \file ShellEntity.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef SHELLENTITY_HPP_
#define SHELLENTITY_HPP_

#include "PhysicsComponent.hpp"
#include "ShellRenderComponent.hpp"

class ShellEntity: public Entity
{
public:

    ShellEntity(const unsigned int& id,
                const float& time_to_live,
                const unsigned int& source_tank_id,
               const Vector3d& position = Vector3d(),
               const float& angle = 0.0f,
               const Vector3d& axis = Vector3d())
        : Entity(id), m_source_tank_id(source_tank_id), m_time_to_live(time_to_live), m_time(0.0f)
    {
        attachComponent(std::shared_ptr<Entity::Component>(new PositionComponent(position)));
        attachComponent(std::shared_ptr<Entity::Component>(new OrientationComponent(Quaternion(angle,
                                                                                               axis))));
        attachComponent(std::shared_ptr<Entity::Component>(new VelocityComponent()));
        attachComponent(std::shared_ptr<Entity::Component>(new PhysicsComponent()));
        attachComponent(std::shared_ptr<Entity::Component>(new ShellRenderComponent()));
    }

    virtual ~ShellEntity()
    {
    }

    inline unsigned int sourceTankId() const;

    inline bool isDead() const;

    inline void incrementTime(const float& dt);

    virtual inline void update(const float& t, const float& dt);

    virtual inline void render(const float& alpha);

protected:

    inline PhysicsComponent::PhysicsState getPhysicsState(const float& alpha) const;

    unsigned int m_source_tank_id;
    float m_time_to_live;
    float m_time;
};

inline unsigned int ShellEntity::sourceTankId() const
{
    return m_source_tank_id;
}

inline bool ShellEntity::isDead() const
{
    return m_time >= m_time_to_live;
}

inline void ShellEntity::incrementTime(const float& dt)
{
    m_time += dt;
}

inline void ShellEntity::update(const float& t, const float& dt)
{
    getComponent<PhysicsComponent>()->update(t, dt);
}

inline void ShellEntity::render(const float& alpha)
{
    float angle;
    Vector3d axis;

    PhysicsComponent::PhysicsState state = getPhysicsState(alpha);

    state.orientation().angleAxis(angle, axis);

    std::shared_ptr<ShellRenderComponent> renderable = getComponent<
        ShellRenderComponent>();

    renderable->translateTo(state.position()).rotateTo(angle, axis);
    renderable->render();
}

inline PhysicsComponent::PhysicsState ShellEntity::getPhysicsState(const float& alpha) const
{
    return getComponent<PhysicsComponent>()->interpolate(alpha);
}




#endif /* SHELLENTITY_HPP_ */
