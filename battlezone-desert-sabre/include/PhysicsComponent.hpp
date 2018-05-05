/**
 * \file PhysicsComponent.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef PHYSICSCOMPONENT_HPP_
#define PHYSICSCOMPONENT_HPP_

#include "Entity.hpp"
#include "OrientationComponent.hpp"
#include "PositionComponent.hpp"
#include "VelocityComponent.hpp"

class PhysicsComponent: public Entity::Component
{
public:

    class PhysicsState
    {
    public:

        PhysicsState()
            : m_position(),
              m_orientation(),
              m_linear_velocity(),
              m_angular_velocity()
        {
        }

        PhysicsState(const Vector3d& position,
                     const Quaternion& orientation,
                     const Vector3d& linear_velocity,
                     const Quaternion& angular_velocity)
            : m_position(position),
              m_orientation(orientation),
              m_linear_velocity(linear_velocity),
              m_angular_velocity(angular_velocity)
        {
        }

        inline Vector3d position() const;

        inline PhysicsState& position(const Vector3d& p);

        inline Quaternion orientation() const;

        inline PhysicsState& orientation(const Quaternion& o);

        inline Vector3d linearVelocity() const;

        inline PhysicsState& linearVelocity(const Vector3d& v);

        inline Quaternion angularVelocity() const;

        inline PhysicsState& angularVelocity(const Quaternion& w);

        static inline PhysicsState interpolate(const PhysicsState& begin_state,
                                               const PhysicsState& end_state,
                                               const float& t);

    protected:

        Vector3d m_position;
        Quaternion m_orientation;
        Vector3d m_linear_velocity;
        Quaternion m_angular_velocity;
    };

    virtual ~PhysicsComponent()
    {
    }

    virtual inline void onAttach(const Entity& entity);

    virtual inline void onDetach(const Entity& entity);

    inline Vector3d linearVelocity() const;

    inline PhysicsComponent& linearVelocity(const Vector3d& v);

    inline Quaternion angularVelocity() const;

    inline PhysicsComponent& angularVelocity(const float& angle,
                                             const Vector3d& axis);

    inline PhysicsComponent& update(const float& t, const float& dt);

    inline PhysicsState interpolate(const float& t);

protected:

    PhysicsState m_current_state;
    PhysicsState m_previous_state;

    std::shared_ptr<PositionComponent> m_position_component;
    std::shared_ptr<OrientationComponent> m_orientation_component;
    std::shared_ptr<VelocityComponent> m_velocity_component;
};

inline Vector3d PhysicsComponent::PhysicsState::position() const
{
    return m_position;
}

inline PhysicsComponent::PhysicsState& PhysicsComponent::PhysicsState::position(const Vector3d& p)
{
    m_position = p;
    return *this;
}

inline Quaternion PhysicsComponent::PhysicsState::orientation() const
{
    return m_orientation;
}

inline PhysicsComponent::PhysicsState& PhysicsComponent::PhysicsState::orientation(const Quaternion& o)
{
    m_orientation = o;
    return *this;
}

inline Vector3d PhysicsComponent::PhysicsState::linearVelocity() const
{
    return m_linear_velocity;
}

inline PhysicsComponent::PhysicsState& PhysicsComponent::PhysicsState::linearVelocity(const Vector3d& v)
{
    m_linear_velocity = v;
    return *this;
}

inline Quaternion PhysicsComponent::PhysicsState::angularVelocity() const
{
    return m_angular_velocity;
}

inline PhysicsComponent::PhysicsState& PhysicsComponent::PhysicsState::angularVelocity(const Quaternion& w)
{
    m_angular_velocity = w;
    return *this;
}

inline PhysicsComponent::PhysicsState PhysicsComponent::PhysicsState::interpolate(const PhysicsComponent::PhysicsState& begin_state,
                                                                                  const PhysicsComponent::PhysicsState& end_state,
                                                                                  const float& t)
{
    return PhysicsState(Vector3d::lerp(begin_state.position(),
                                       end_state.position(),
                                       t),
                        Quaternion::slerp(begin_state.orientation(),
                                          end_state.orientation(),
                                          t).normalize(),
                        Vector3d::lerp(begin_state.linearVelocity(),
                                       end_state.linearVelocity(),
                                       t),
                        Quaternion::slerp(begin_state.angularVelocity(),
                                          end_state.angularVelocity(),
                                          t).normalize());
}

inline void PhysicsComponent::onAttach(const Entity& entity)
{
    m_position_component = entity.getComponent<PositionComponent>();
    m_orientation_component = entity.getComponent<OrientationComponent>();
    m_velocity_component = entity.getComponent<VelocityComponent>();

    m_current_state.position(m_position_component->position());
    m_current_state.orientation(m_orientation_component->orientation());
    m_current_state.linearVelocity(m_velocity_component->linear());
    m_current_state.angularVelocity(m_velocity_component->angular());

    m_previous_state = m_current_state;
}

inline void PhysicsComponent::onDetach(const Entity& entity)
{
    m_position_component = nullptr;
    m_orientation_component = nullptr;
    m_velocity_component = nullptr;
}

inline Vector3d PhysicsComponent::linearVelocity() const
{
    return m_velocity_component->linear();
}

inline PhysicsComponent& PhysicsComponent::linearVelocity(const Vector3d& v)
{
    m_velocity_component->linear(v);
    return *this;
}

inline Quaternion PhysicsComponent::angularVelocity() const
{
    return m_velocity_component->angular();
}

inline PhysicsComponent& PhysicsComponent::angularVelocity(const float& angle,
                                                           const Vector3d& axis)
{
    m_velocity_component->angular(angle, axis);
    return *this;
}

inline PhysicsComponent& PhysicsComponent::update(const float& t,
                                                  const float& dt)
{
    m_previous_state = m_current_state;

    Vector3d new_position = m_position_component->position()
                            + m_velocity_component->linear() * dt;

    Quaternion displacement_orientation = m_velocity_component->angular() * dt;

    displacement_orientation.normalize();

    float angle = 0.0f;
    Vector3d axis;

    displacement_orientation.angleAxis(angle, axis);

    axis.normalize();

    m_position_component->translateTo(new_position);
    m_orientation_component->rotateBy(angle, axis).orientation().normalize();

    m_current_state.position(m_position_component->position());
    m_current_state.orientation(m_orientation_component->orientation());

    return *this;
}

inline PhysicsComponent::PhysicsState PhysicsComponent::interpolate(const float& t)
{
    return PhysicsState::interpolate(m_previous_state, m_current_state, t);
}

#endif /* PHYSICSCOMPONENT_HPP_ */
