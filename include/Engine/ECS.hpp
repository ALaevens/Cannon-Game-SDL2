#ifndef ECS_H
#define ECS_H
#pragma once

#include <cstddef>
#include <type_traits>
#include <bitset>
#include <array>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include <SDL.h>
#include <cassert>
// Based on the ECS tutorial by Vittorio Romeo
// More of an EC than an ECS since logic is in the components
// and there isnt a system class
namespace ECS
{
    struct Component;
    class Entity;
    class Manager;

    using ComponentID = std::size_t;
    using Group = std::size_t;

    namespace Internal {
        inline ComponentID getUniqueComponentID() noexcept {
            static ComponentID lastID = 0u;
            return lastID++;
        }
    }

    template <typename T>
    inline ComponentID getComponentTypeID() noexcept {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
        static ComponentID typeID = Internal::getUniqueComponentID();
        return typeID;
    }

    constexpr std::size_t maxComponents = 32;
    using ComponentBitset = std::bitset<maxComponents>;
    using ComponentArray = std::array<Component*, maxComponents>;

    constexpr std::size_t maxGroups = 32;
    using GroupBitset = std::bitset<maxGroups>;

    struct Component {
        Entity* entity;

        virtual void init() {}
        virtual void update(double ms) {}
        virtual void draw(SDL_Renderer* renderer) {}
        virtual ~Component() {}
    };

    class Entity {
    public:
        Entity(Manager& p_manager);

        void update(double ms);

        void draw(SDL_Renderer* renderer);

        bool isAlive();
        void destroy();

        template <typename T>
        bool hasComponent() const {
            return componentBitset[getComponentTypeID<T>()];
        }

        bool hasGroup(Group p_group) const noexcept;

        void addGroup(Group p_group) noexcept;

        void delGroup(Group p_group) noexcept;

        template <typename T, typename... TArgs>
        T& addComponent(TArgs&&... p_Args) {
            assert(!hasComponent<T>());

            T* c = new T(std::forward<TArgs>(p_Args)...);
            c->entity = this;
            std::unique_ptr<Component> uPtr{c};
            components.emplace_back(std::move(uPtr));

            componentArray[getComponentTypeID<T>()] = c;
            componentBitset[getComponentTypeID<T>()] = true;

            c->init();
            return *c;
        }

        template <typename T>
        T& getComponent() const {
            assert(hasComponent<T>());
            auto ptr(componentArray[getComponentTypeID<T>()]);
            return *reinterpret_cast<T*>(ptr);
        }

    private:
        Manager& manager;

        bool alive = true;
        std::vector<std::unique_ptr<Component>> components;
        ComponentArray componentArray;
        ComponentBitset componentBitset;
        GroupBitset groupBitset;
    };

    class Manager {
    public:
        void update(double ms);
        
        void draw(SDL_Renderer* renderer);

        void draw(SDL_Renderer* renderer, std::vector<Entity*>& p_entities);

        void addToGroup(Entity* p_entity, Group p_group);

        std::vector<Entity*>& getEntitiesByGroup(Group p_group);

        void refresh();

        Entity& addEntity();

    private:
        std::vector<std::unique_ptr<Entity>> entities;
        std::array<std::vector<Entity*>, maxGroups> groupedEntities;
    };
} // namespace ECS


#endif