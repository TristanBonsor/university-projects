/**
 * \file FloorEntity.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef FLOORENTITY_HPP_
#define FLOORENTITY_HPP_

#include "FloorRenderComponent.hpp"

class FloorEntity: public Entity
{
public:

    FloorEntity(const unsigned int& id, const Vector3d& position = Vector3d())
        : Entity(id)
    {
        attachComponent(std::shared_ptr<Entity::Component>(new PositionComponent(position)));
        attachComponent(std::shared_ptr<Entity::Component>(new FloorRenderComponent()));
    }

    virtual ~FloorEntity()
    {
    }

    virtual inline void render(const float& alpha = 0.0f);

protected:

};

inline void FloorEntity::render(const float& alpha)
{
    Vector3d position = getComponent<PositionComponent>()->position();

    std::shared_ptr<FloorRenderComponent> renderable = getComponent<
        FloorRenderComponent>();

    renderable->translateTo(position);
    renderable->render();
}

#endif /* FLOORENTITY_HPP_ */
