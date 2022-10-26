#include "Engine/ECS.hpp"

// Based on the ECS tutorial by Vittorio Romeo
// More of an EC than an ECS since logic is in the components
// and there isnt a system class

using namespace ECS;

Entity::Entity(ECS::Manager& p_manager) : manager(p_manager) {}

void Entity::update(double ms) {
    for (auto& c : components) {
        c->update(ms);
    }
}

void Entity::draw(SDL_Renderer* renderer) {
    for (auto& c : components) {
        c->draw(renderer);
    }
}

bool Entity::isAlive() { return alive; }
void Entity::destroy() { alive = false; }

// template <typename T>
// bool Entity::hasComponent() const {
//     return componentBitset[getComponentTypeID<T>()];
// }

bool Entity::hasGroup(Group p_group) const noexcept {
    return groupBitset[p_group];
}

void Entity::addGroup(Group p_group) noexcept {
    groupBitset[p_group] = true;
    manager.addToGroup(this, p_group);
}

void Entity::delGroup(Group p_group) noexcept {
    groupBitset[p_group] = false;
}

// template <typename T, typename... TArgs>
// T& Entity::addComponent(TArgs&&... p_Args) {
//     assert(!hasComponent<T>());

//     T* c = new T(std::forward<TArgs>(p_Args)...);
//     c->entity = this;
//     std::unique_ptr<Component> uPtr{c};
//     components.emplace_back(std::move(uPtr));

//     componentArray[getComponentTypeID<T>()] = c;
//     componentBitset[getComponentTypeID<T>()] = true;

//     c->init();
//     return *c;
// }

// template <typename T>
// T& Entity::getComponent() const {
//     assert(hasComponent<T>());
//     auto ptr(componentArray[getComponentTypeID<T>()]);
//     return *reinterpret_cast<T*>(ptr);
// }


void Manager::update(double ms) {
    for (auto& e : entities) {
        e->update(ms);
    }
}

void Manager::draw(SDL_Renderer* renderer) {
    for (auto& e : entities) {
        e->draw(renderer);
    }
}

void Manager::draw(SDL_Renderer* renderer, std::vector<Entity*>& p_entities) {
    for (auto& e : p_entities) {
        e->draw(renderer);
    }
}

void Manager::addToGroup(Entity* p_entity, Group p_group) {
    groupedEntities[p_group].emplace_back(p_entity);
}

std::vector<Entity*>& Manager::getEntitiesByGroup(Group p_group) {
    return groupedEntities[p_group];
}

void Manager::refresh() {
    for (Group i = 0; i < maxGroups; ++i) {
        auto& v = groupedEntities[i];
        v.erase(
            std::remove_if(
                std::begin(v), 
                std::end(v), 
                [i](Entity* p_entity) {
                    return !p_entity->isAlive() || !p_entity->hasGroup(i);
                }
            ),
            std::end(v)
        );
    }
    
    entities.erase(
        std::remove_if(
            std::begin(entities), 
            std::end(entities),
            [](const std::unique_ptr<Entity>& p_entity) {
                return !p_entity->isAlive();
            }
        ),
        std::end(entities)
    );
}

Entity& Manager::addEntity() {
    Entity* e = new Entity(*this);
    std::unique_ptr<Entity> uPtr{e};
    entities.emplace_back(std::move(uPtr));
    return *e;
}
