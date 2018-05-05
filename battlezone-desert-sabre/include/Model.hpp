/*
 * \file Model.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef MODEL_HPP_
#define MODEL_HPP_
#include <iostream>
#include <chrono>
#include <random>
#include <unordered_map>
#include <vector>

#include "EnemyTankEntity.hpp"
#include "FloorEntity.hpp"
#include "PlayerTankEntity.hpp"
#include "ShellEntity.hpp"

#define PLAYER_DEATH_TIME 5.0f

class Model
{
public:

    inline void init();

    inline std::shared_ptr<TankControlComponent> getPlayerControl() const;

    inline PhysicsComponent::PhysicsState getPlayerPhysicsState(const float& alpha =
        0.0f);

    inline bool isPlayerDead() const;

    inline void update(const float& t = 0.0f, const float& dt = 0.0f);

    inline void render(const float& alpha = 0.0f) const;

protected:

    inline void initEntities();

    inline void reSpawnPlayerTank();

    inline void spawnEnemyTank();

    inline void generateTank(const Vector3d& position,
                             const float& angle,
                             const bool& is_player = false);

    inline void checkPlayerDeath(const float& t);

    inline void rePositionTanks();

    inline void resolveContact();

    inline void shootIfDesired();

    inline void shellTimeOuts(const float& t);

    inline void updateAi();

    inline void updatePhysics(const float& t, const float& dt);

    inline void acquireTarget();

    inline void firePlayerShell();

    inline void fireShell(std::shared_ptr<TankEntity>& tank_entity,
                          const bool& is_player = false);

    inline unsigned int generateId();

    inline float generateAngle();

    unsigned int m_id_counter;

    bool m_is_player_dead;
    float m_player_death_time;

    std::mt19937 m_rng;
    std::normal_distribution<float> m_rng_distribution;

    std::shared_ptr<PlayerTankEntity> m_player;

    std::unordered_map<unsigned int, std::shared_ptr<Entity> > m_entity_index;
    std::unordered_map<unsigned int, std::shared_ptr<TankEntity> > m_tank_index;
    std::unordered_map<unsigned int, std::shared_ptr<EnemyTankEntity> > m_enemy_tank_index;
    std::unordered_map<unsigned int, std::shared_ptr<ShellEntity> > m_shell_index;
};

inline void Model::init()
{
    m_id_counter = 0;
    m_is_player_dead = false;
    m_player_death_time = 0.0f;

    m_rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch()
        .count());

    initEntities();
}

inline std::shared_ptr<TankControlComponent> Model::getPlayerControl() const
{
    return m_player->getComponent<TankControlComponent>();
}

inline PhysicsComponent::PhysicsState Model::getPlayerPhysicsState(const float& alpha)
{
    return m_player->getComponent<PhysicsComponent>()->interpolate(alpha);
}

inline bool Model::isPlayerDead() const
{
    return m_is_player_dead;
}

inline void Model::update(const float& t, const float& dt)
{

    // Test player death status
    checkPlayerDeath(dt);

    // Reposition tanks that stray outside play area.
    rePositionTanks();

    // Test for shell/tank collisions, remove contacted non-player entities from
    // game.
    resolveContact();

    // Fire tank shell if desired.
    shootIfDesired();

    // Test shells for time out, remove if necessary.
    shellTimeOuts(dt);

    // Update enemy AI.
    updateAi();

    // Update physics.
    updatePhysics(t, dt);

    // Update enemy shoot status.
    acquireTarget();
}

inline void Model::render(const float& alpha) const
{
    for(auto entity : m_entity_index)
    {
        entity.second->render(alpha);
    }
}

inline void Model::initEntities()
{
    // Player tank.
    generateTank(Vector3d(0.0f, 0.175f, 0.0f), 0.0f, true);

    // Enemy tanks.
    generateTank(Vector3d(0.0f, 0.0f, -100.0f), 0.0f);
    generateTank(Vector3d(0.0f, 0.0f, 120.0f), 0.0f);
    generateTank(Vector3d(-120.0f, 0.0f, 0.0f), 0.0f);
    generateTank(Vector3d(120.0f, 0.0f, 0.0f), 0.0f);

    // Floor.
    unsigned int id = generateId();
    m_entity_index.emplace(id,
                           std::shared_ptr<FloorEntity>(new FloorEntity(id,
                                                                        Vector3d(0.0f,
                                                                                 -1.0f,
                                                                                 0.0f))));
}

inline void Model::reSpawnPlayerTank()
{
    m_entity_index.emplace(m_player->getId(), m_player);
    m_tank_index.emplace(m_player->getId(), m_player);
    m_player->getComponent<TankControlComponent>()->isShellActive(false);
}

inline void Model::spawnEnemyTank()
{
    float angle = generateAngle();

    Quaternion orientation(angle, Vector3d(0.0f, 1.0f, 0.0f));
    orientation.normalize();

    Vector3d position = orientation
                        * (100.0f * TankControlComponent::M_DEFAULT_AT)
                        + getPlayerControl()->position();

    generateTank(position, angle);
}

inline void Model::generateTank(const Vector3d& position,
                                const float& angle,
                                const bool& is_player)
{
    unsigned int id;

    if(is_player)
    {
        id = generateId();
        m_player =
            std::shared_ptr<PlayerTankEntity>(new PlayerTankEntity(id,
                                                                   position,
                                                                   angle,
                                                                   Vector3d(0.0f,
                                                                            1.0f,
                                                                            0.0f),
                                                                   200.0f));

        m_entity_index.emplace(id, m_player);
        m_tank_index.emplace(id, m_player);
        return;
    }

    id = generateId();
    std::shared_ptr<EnemyTankEntity> enemy(new EnemyTankEntity(id,
                                                               position,
                                                               angle,
                                                               Vector3d(0.0f,
                                                                        1.0f,
                                                                        0.0f),
                                                               200.0f));

    m_entity_index.emplace(id, enemy);
    m_tank_index.emplace(id, enemy);
    m_enemy_tank_index.emplace(id, enemy);
}

inline void Model::checkPlayerDeath(const float& t)
{
    if(m_is_player_dead)
    {
        m_player_death_time += t;

        if(PLAYER_DEATH_TIME <= m_player_death_time)
        {
            m_is_player_dead = false;
            m_player_death_time = 0.0f;
            reSpawnPlayerTank();
        }
    }
}

inline void Model::rePositionTanks()
{
    const float scale = -0.95f;

    for(auto tank : m_tank_index)
    {
        std::shared_ptr<TankControlComponent> control = tank.second
            ->getComponent<TankControlComponent>();

        Vector3d position = control->position();

        if(position.lengthSquared() > 22500.0f)
        {
            tank.second->getComponent<PositionComponent>()->translateTo(scale
                                                                        * position);
        }
    }
}

inline void Model::resolveContact()
{
// Test for shell/tank collisions.

    std::vector<std::shared_ptr<TankEntity> > contacted_tanks;
    std::vector<std::shared_ptr<ShellEntity> > contacted_shells;

    for(auto shell : m_shell_index)
    {
        for(auto tank : m_tank_index)
        {
            if((shell.second->getComponent<PositionComponent>()->position()
                - tank.second->getComponent<PositionComponent>()->position())
                   .lengthSquared()
               <= (1.0f + 0.25f) * (1.0f + 0.25f))
            {
                contacted_tanks.push_back(tank.second);
                contacted_shells.push_back(shell.second);

                if(m_tank_index.count(shell.second->sourceTankId()) != 0)
                {
                    m_tank_index[shell.second->sourceTankId()]
                        ->getComponent<TankControlComponent>()->isShellActive(false);
                }
            }
        }
    }

// Remove contacted shells/tanks.

    for(auto shell : contacted_shells)
    {
        m_entity_index.erase(shell->getId());
        m_shell_index.erase(shell->getId());
    }

    for(auto tank : contacted_tanks)
    {
        if(tank == m_player)
        {
            m_is_player_dead = true;
            m_entity_index.erase(tank->getId());
            m_tank_index.erase(tank->getId());
        }
        else
        {
            m_entity_index.erase(tank->getId());
            m_tank_index.erase(tank->getId());
            m_enemy_tank_index.erase(tank->getId());
            spawnEnemyTank();
        }
    }
}

inline void Model::shootIfDesired()
{
// Fire player shell if desired.

    if(!m_is_player_dead
       && m_player->getComponent<TankControlComponent>()->isShotDesired())
    {
        firePlayerShell();
    }

// Fire enemy shell if desired.

    for(auto enemy : m_enemy_tank_index)
    {
        std::shared_ptr<TankControlComponent> enemy_control = enemy.second
            ->getComponent<TankControlComponent>();

        if(!m_is_player_dead && enemy_control->isShotDesired())
        {
            std::shared_ptr<TankEntity> tank_entity = enemy.second;
            fireShell(tank_entity);
        }
    }
}

inline void Model::shellTimeOuts(const float& t)
{
    std::vector<std::shared_ptr<ShellEntity> > dead_shells;

    for(auto shell : m_shell_index)
    {
        shell.second->incrementTime(t);
        if(shell.second->isDead())
        {
            dead_shells.push_back(shell.second);

            if(m_tank_index.count(shell.second->sourceTankId()) != 0)
            {
                m_tank_index[shell.second->sourceTankId()]
                    ->getComponent<TankControlComponent>()->isShellActive(false);
            }
        }
    }

    for(auto shell : dead_shells)
    {
        m_entity_index.erase(shell->getId());
        m_shell_index.erase(shell->getId());
    }
}

inline void Model::updateAi()
{
// Enemy search for player.

    std::shared_ptr<TankControlComponent> player_control = m_player
        ->getComponent<TankControlComponent>();

    for(auto enemy : m_enemy_tank_index)
    {
        std::shared_ptr<TankControlComponent> enemy_control = enemy.second
            ->getComponent<TankControlComponent>();

        Vector3d displacement = player_control->position()
                                - enemy_control->position();

        if(!m_is_player_dead && displacement.lengthSquared() <= 10000.0f) //100^2
        {
            // Player found, begin intercept.

            Vector3d ray_direction = enemy_control->orientation().normalize()
                                     * TankControlComponent::M_DEFAULT_AT;

            ray_direction.normalize();

            float A = Vector3d::dot(ray_direction, ray_direction);
            float B = Vector3d::dot(2 * ray_direction,
                                    enemy_control->position()
                                    - player_control->position());
            float C = Vector3d::dot(enemy_control->position()
                                    - player_control->position(),
                                    enemy_control->position()
                                    - player_control->position())
                      - 1.0f;

            float discriminant = (B * B) - (4 * A * C);

            if(discriminant < 0)
            {
                float angle;
                Vector3d axis;

                Quaternion::rotationBetweenVectors(ray_direction, displacement)
                    .normalize().angleAxis(angle, axis);

                axis.normalize();

                enemy_control->leftTreadStop();
                enemy_control->rightTreadStop();

                if(axis.y <= 0.0f)
                {
                    enemy_control->leftTreadForward();
                    enemy_control->rightTreadBackward();
                }
                else
                {
                    enemy_control->leftTreadBackward();
                    enemy_control->rightTreadForward();
                }
            }
            else
            {
                float t = (-B - sqrt(discriminant)) / (2.0f * A);

                if(t >= 0.0f && t <= 100.0f)
                {
                    enemy_control->leftTreadStop();
                    enemy_control->rightTreadStop();
                    enemy_control->leftTreadForward();
                    enemy_control->rightTreadForward();
                }
                else
                {
                    float angle;
                    Vector3d axis;

                    Quaternion::rotationBetweenVectors(ray_direction,
                                                       displacement).normalize()
                        .angleAxis(angle, axis);

                    axis.normalize();

                    enemy_control->leftTreadStop();
                    enemy_control->rightTreadStop();

                    if(axis.y <= 0.0f)
                    {
                        enemy_control->leftTreadForward();
                        enemy_control->rightTreadBackward();
                    }
                    else
                    {
                        enemy_control->leftTreadBackward();
                        enemy_control->rightTreadForward();
                    }
                }
            }
        }
        else
        {
            // Player not found, patrol.
            enemy_control->leftTreadStop();
            enemy_control->rightTreadStop();
            enemy_control->leftTreadForward();
        }
    }
}

inline void Model::updatePhysics(const float& t, const float& dt)
{
    for(auto entity : m_entity_index)
    {
        entity.second->update(t, dt);
    }
}

inline void Model::acquireTarget()
{
    std::shared_ptr<TankControlComponent> player_control = m_player
        ->getComponent<TankControlComponent>();

    for(auto enemy : m_enemy_tank_index)
    {
        std::shared_ptr<TankControlComponent> enemy_control = enemy.second
            ->getComponent<TankControlComponent>();

        // Acquire targeting solution, if possible.

        Vector3d displacement = enemy_control->position()
                                - player_control->position();

        if(!m_is_player_dead && displacement.lengthSquared() <= 5625.0f) //75^2
        {
            // Player in range, indicate desire to fire if prudent.

            Vector3d ray_direction = enemy_control->orientation().normalize()
                                     * TankControlComponent::M_DEFAULT_AT;

            ray_direction.normalize();

            float A = Vector3d::dot(ray_direction, ray_direction);
            float B = Vector3d::dot(2 * ray_direction, displacement);
            float C = Vector3d::dot(displacement, displacement) - 1.0f;

            float discriminant = (B * B) - (4 * A * C);

            if(discriminant < 0)
            {
                enemy_control->isShotDesired(false);
            }
            else
            {
                enemy_control->isShotDesired(true);
            }
        }
        else
        {
            enemy_control->isShotDesired(false);
        }
    }
}

inline void Model::firePlayerShell()
{
    std::shared_ptr<TankEntity> tank_entity = m_player;
    fireShell(tank_entity, true);
}

inline void Model::fireShell(std::shared_ptr<TankEntity>& tank_entity,
                             const bool& is_player)
{
    std::shared_ptr<TankControlComponent> control = tank_entity
        ->getComponent<TankControlComponent>();

    if(!control->isShellActive())
    {
        control->isShellActive(true);

        Vector3d mounting;

        if(is_player)
        {
            mounting = Vector3d(0.0f, 0.0f, -3.0f); //-2.0
        }
        else
        {
            mounting = Vector3d(0.0f, 0.175f, -3.0f); //-2.0
        }

        Quaternion orientation = control->orientation().normalize();

        Vector3d position = control->position() + (orientation * mounting);

        float angle;
        Vector3d axis;

        orientation.angleAxis(angle, axis);

        axis.normalize();

        unsigned int id = generateId();
        std::shared_ptr<ShellEntity> shell(new ShellEntity(id,
                                                           5.0f,
                                                           tank_entity->getId(),
                                                           position,
                                                           angle,
                                                           axis));

        m_entity_index.emplace(id, shell);
        m_shell_index.emplace(id, shell);

        m_entity_index[id]->getComponent<PhysicsComponent>()->linearVelocity(Vector3d(0.0f,
                                                                                      0.0f,
                                                                                      -102.4f)
                                                                             * orientation);
    }
}

inline unsigned int Model::generateId()
{
    return m_id_counter++;
}

inline float Model::generateAngle()
{
    return m_rng_distribution(m_rng) * 2 * 3.1459f;
}

#endif /* MODEL_HPP_ */
