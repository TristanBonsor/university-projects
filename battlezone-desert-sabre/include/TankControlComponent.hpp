/**
 * \file TankControlComponent.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef TANKCONTROLCOMPONENT_HPP_
#define TANKCONTROLCOMPONENT_HPP_

#include "PhysicsComponent.hpp"

class TankControlComponent: public Entity::Component
{
protected:

    enum class TreadState
    {
        STOP = 0, FORWARD, BACKWARD
    };

    static const float M_TREAD_VELOCITY_MAGNITUDE;

    static const Vector3d M_TREAD_VELOCITY_STOP;

    static const Vector3d M_TREAD_VELOCITY_FORWARD;

    static const Vector3d M_TREAD_VELOCITY_BACKWARD;

    static const float M_ANGULAR_VELOCITY_STOP;

    std::shared_ptr<PositionComponent> m_position_component;
    std::shared_ptr<OrientationComponent> m_orientation_component;
    std::shared_ptr<PhysicsComponent> m_physics_component;

    float m_turn_radius;

    float m_cw_turn_angular_velocity;
    float m_ccw_turn_angular_velocity;

    float m_cw_pivot_angular_velocity;
    float m_ccw_pivot_angular_velocity;

    TreadState m_left_tread;
    TreadState m_right_tread;

    bool m_is_shot_desired;
    bool m_is_shell_active;

public:

    static const Vector3d M_DEFAULT_UP;

    static const Vector3d M_DEFAULT_AT;

    TankControlComponent(const float& turn_radius = 1.0f)
        : m_position_component(nullptr),
          m_orientation_component(nullptr),
          m_physics_component(nullptr),
          m_turn_radius(turn_radius),
          m_left_tread(TreadState::STOP),
          m_right_tread(TreadState::STOP),
          m_is_shot_desired(false),
          m_is_shell_active(false)
    {
        m_cw_turn_angular_velocity = -M_TREAD_VELOCITY_MAGNITUDE
                                     / m_turn_radius;

        m_ccw_turn_angular_velocity = M_TREAD_VELOCITY_MAGNITUDE
                                      / m_turn_radius;

        m_cw_pivot_angular_velocity = -M_TREAD_VELOCITY_MAGNITUDE
                                      / (m_turn_radius * 0.5f);

        m_ccw_pivot_angular_velocity = M_TREAD_VELOCITY_MAGNITUDE
                                       / (m_turn_radius * 0.5f);
    }

    virtual ~TankControlComponent()
    {
    }

    inline TankControlComponent& process();

    inline TankControlComponent& leftTreadForward();

    inline TankControlComponent& rightTreadForward();

    inline TankControlComponent& leftTreadBackward();

    inline TankControlComponent& rightTreadBackward();

    inline TankControlComponent& leftTreadStop();

    inline TankControlComponent& rightTreadStop();

    inline bool isShotDesired() const;

    inline void isShotDesired(const bool& is_desired);

    inline bool isShellActive() const;

    inline TankControlComponent& isShellActive(const bool& is_active);

    inline Vector3d position() const;

    inline Quaternion orientation() const;

    inline Vector3d linearVelocity() const;

    inline Quaternion angularVelocity() const;

    virtual inline void onAttach(const Entity& entity);

    virtual inline void onDetach(const Entity& entity);
};

inline TankControlComponent& TankControlComponent::leftTreadForward()
{
    m_left_tread = TreadState::FORWARD;
    return *this;
}

inline TankControlComponent& TankControlComponent::rightTreadForward()
{
    m_right_tread = TreadState::FORWARD;
    return *this;
}

inline TankControlComponent& TankControlComponent::leftTreadBackward()
{
    m_left_tread = TreadState::BACKWARD;
    return *this;
}

inline TankControlComponent& TankControlComponent::rightTreadBackward()
{
    m_right_tread = TreadState::BACKWARD;
    return *this;
}

inline TankControlComponent& TankControlComponent::leftTreadStop()
{
    m_left_tread = TreadState::STOP;
    return *this;
}

inline TankControlComponent& TankControlComponent::rightTreadStop()
{
    m_right_tread = TreadState::STOP;
    return *this;
}

inline bool TankControlComponent::isShotDesired() const
{
    return m_is_shot_desired;
}

inline void TankControlComponent::isShotDesired(const bool& is_desired)
{
    m_is_shot_desired = is_desired;
}

inline bool TankControlComponent::isShellActive() const
{
    return m_is_shell_active;
}

inline TankControlComponent& TankControlComponent::isShellActive(const bool& is_active)
{
    m_is_shell_active = is_active;
    return *this;
}

inline TankControlComponent& TankControlComponent::process()
{
    if(m_left_tread == TreadState::FORWARD
       && m_right_tread == TreadState::FORWARD)
    {
        // Move forward.
        m_physics_component->angularVelocity(M_ANGULAR_VELOCITY_STOP,
                                             M_DEFAULT_UP);

        m_physics_component->linearVelocity(M_TREAD_VELOCITY_FORWARD
                                            * m_orientation_component
                                                ->orientation().normalize());
    }
    else if(m_left_tread == TreadState::FORWARD
            && m_right_tread == TreadState::BACKWARD)
    {
        // Pivot right (cw).
        m_physics_component->linearVelocity(M_TREAD_VELOCITY_STOP);

        m_physics_component->angularVelocity(m_cw_pivot_angular_velocity,
                                             M_DEFAULT_UP);
    }
    else if(m_left_tread == TreadState::FORWARD
            && m_right_tread == TreadState::STOP)
    {
        // Turn right forward (cw).
        m_physics_component->linearVelocity(M_TREAD_VELOCITY_FORWARD
                                            * m_orientation_component
                                                ->orientation().normalize());

        m_physics_component->angularVelocity(m_cw_turn_angular_velocity,
                                             M_DEFAULT_UP);
    }
    else if(m_left_tread == TreadState::BACKWARD
            && m_right_tread == TreadState::FORWARD)
    {
        // Pivot left (ccw).
        m_physics_component->linearVelocity(M_TREAD_VELOCITY_STOP);

        m_physics_component->angularVelocity(m_ccw_pivot_angular_velocity,
                                             M_DEFAULT_UP);
    }
    else if(m_left_tread == TreadState::BACKWARD
            && m_right_tread == TreadState::BACKWARD)
    {
        // Move backward.
        m_physics_component->angularVelocity(M_ANGULAR_VELOCITY_STOP,
                                             M_DEFAULT_UP);

        m_physics_component->linearVelocity(M_TREAD_VELOCITY_BACKWARD
                                            * m_orientation_component
                                                ->orientation().normalize());
    }
    else if(m_left_tread == TreadState::BACKWARD
            && m_right_tread == TreadState::STOP)
    {
        // Turn left backward (ccw).
        m_physics_component->linearVelocity(M_TREAD_VELOCITY_BACKWARD
                                            * m_orientation_component
                                                ->orientation().normalize());

        m_physics_component->angularVelocity(m_ccw_turn_angular_velocity,
                                             M_DEFAULT_UP);
    }
    else if(m_left_tread == TreadState::STOP
            && m_right_tread == TreadState::FORWARD)
    {
        // Turn left forward (ccw).
        m_physics_component->linearVelocity(M_TREAD_VELOCITY_FORWARD
                                            * m_orientation_component
                                                ->orientation().normalize());

        m_physics_component->angularVelocity(m_ccw_turn_angular_velocity,
                                             M_DEFAULT_UP);

    }
    else if(m_left_tread == TreadState::STOP
            && m_right_tread == TreadState::BACKWARD)
    {
        // Turn right backward (cw).
        m_physics_component->linearVelocity(M_TREAD_VELOCITY_BACKWARD
                                            * m_orientation_component
                                                ->orientation().normalize());

        m_physics_component->angularVelocity(m_cw_turn_angular_velocity,
                                             M_DEFAULT_UP);
    }
    else if(m_left_tread == TreadState::STOP
            && m_right_tread == TreadState::STOP)
    {
        // Stop movement.
        m_physics_component->linearVelocity(M_TREAD_VELOCITY_STOP);

        m_physics_component->angularVelocity(M_ANGULAR_VELOCITY_STOP,
                                             M_DEFAULT_UP);
    }

    return *this;
}

inline Vector3d TankControlComponent::position() const
{
    return m_position_component->position();
}

inline Quaternion TankControlComponent::orientation() const
{
    return m_orientation_component->orientation();
}

inline Vector3d TankControlComponent::linearVelocity() const
{
    return m_physics_component->linearVelocity();
}

inline Quaternion TankControlComponent::angularVelocity() const
{
    return m_physics_component->angularVelocity();
}

inline void TankControlComponent::onAttach(const Entity& entity)
{
    m_position_component = entity.getComponent<PositionComponent>();
    m_orientation_component = entity.getComponent<OrientationComponent>();
    m_physics_component = entity.getComponent<PhysicsComponent>();
}

inline void TankControlComponent::onDetach(const Entity& entity)
{
    m_position_component = nullptr;
    m_orientation_component = nullptr;
    m_physics_component = nullptr;
}

#endif /* TANKCONTROLCOMPONENT_HPP_ */
