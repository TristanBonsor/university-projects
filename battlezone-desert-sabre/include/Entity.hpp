/**
 * \file Entity.hpp
 * \copyright (C) 2014 Tristan W. Bonsor
 */

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <memory>
#include <typeindex>
#include <unordered_map>

/**
 * \class Entity Entity.hpp "include/Entity.hpp"
 * \brief Basic entity class with component functionality.
 *
 * This class provides a basic entity type with functionality for adding,
 * accessing, and removing various components (position, orientation, etc.).
 */
class Entity
{
public:

    /**
     * \class Entity::Component Entity.hpp "include/Entity.hpp"
     * \brief Base entity component type.
     *
     * This class provides a base entity component type from which various
     * components may be derived. Sub-class identification will rely on RTTI.
     */
    class Component
    {
    public:

        virtual ~Component()
        {
        }

        /**
         * Called by an entity when a component is attached. This is where a
         * component can setup references and data based on other attached
         * components.
         * @param entity The entity that the component is being attached to.
         */
        virtual inline void onAttach(const Entity& entity)
        {
        }

        /**
         * Called by an entity when a component is detached. This is where a
         * component can clean-up and resolve references and data based on other
         * attached components.
         * @param entity The entity that the component is being detached from.
         */
        virtual inline void onDetach(const Entity& entity)
        {
        }
    };

    /**
     * Explicit constructor.
     * Create an entity with a specified id.
     * @param id An unsigned integer representing a unique entity id.
     */
    Entity(const unsigned int& id)
        : m_id(id), m_components()
    {
    }

    virtual ~Entity()
    {
    }

    /**
     * Get the entity's id.
     * @return An unsigned integer representing a unique entity id.
     */
    inline unsigned int getId() const;

    /**
     * Add an entity component to the entity.
     * @param component A shared pointer that references the entity component to
     * be added to the entity.
     * @return A reference to the entity.
     */
    inline Entity& attachComponent(const std::shared_ptr<Component>& component);

    /**
     * Get a component that has been added to the entity.
     * @tparam The type of component to get from the entity.
     * @return A shared pointer to the component, if it has been added to
     * entity. Otherwise, returns a null shared pointer.
     */
    template<typename T>
    inline std::shared_ptr<T> getComponent() const;

    /**
     * Remove an entity component from the entity.
     * @tparam The type of component to remove from the entity.
     * @return A reference to the entity.
     */
    template<typename T>
    inline std::shared_ptr<T> detachComponent();

    /**
     *
     */
    virtual inline void update(const float& t = 0.0f, const float& dt = 0.0f);

    /**
     *
     */
    virtual inline void render(const float& alpha = 0.0f);

protected:

    unsigned int m_id; //!< The entity id.

    std::unordered_map<std::type_index, std::shared_ptr<Component> > m_components; //!< Index of all currently added components.
};

inline unsigned int Entity::getId() const
{
    return m_id;
}

inline Entity& Entity::attachComponent(const std::shared_ptr<Entity::Component>& component)
{
    m_components[std::type_index(typeid(*component))] = component;
    component->onAttach(*this);
    return *this;
}

template<typename T>
inline std::shared_ptr<T> Entity::getComponent() const
{
    std::type_index index(typeid(T));

    if(m_components.count(index) != 0)
    {
        return std::static_pointer_cast<T>(m_components.at(index));
    }

    return std::shared_ptr<T>();
}

template<typename T>
inline std::shared_ptr<T> Entity::detachComponent()
{
    std::type_index index(typeid(T));

    if(m_components.count(index) != 0)
    {
        std::shared_ptr<T> component = std::static_pointer_cast<T>(m_components
            .at(index));

        m_components.erase(index);
        component->onDetach(*this);

        return component;
    }

    return std::shared_ptr<T>();
}

inline void Entity::update(const float& t, const float& dt)
{
}

inline void Entity::render(const float& alpha)
{
}

#endif /* ENTITY_HPP_ */
