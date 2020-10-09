#include "pch.h"
#include "Entity.h"

namespace Rival {

    Entity::Entity(int width, int height)
        : width(width),
          height(height),
          deleted(false),
          id(-1) {}

    void Entity::attach(std::unique_ptr<EntityComponent> component) {
        component->onAttach(this);
        components.push_back(std::move(component));
    }

    void Entity::onSpawn(
            Scenario* newScenario, int newId, int newX, int newY) {
        scenario = newScenario;
        id = newId;
        x = newX;
        y = newY;

        for (const auto& component : components) {
            component->onEntitySpawned(scenario);
        }
    }

    void Entity::update() {

        // Update our Components
        for (const auto& component : components) {
            if (!component->isDeleted()) {
                component->update();
            }
        }

        // Remove deleted Components
        for (auto it = components.begin(); it != components.end();) {
            const auto& component = *it;
            if (component->isDeleted()) {
                it = components.erase(it);
            } else {
                ++it;
            }
        }
    }

    const bool Entity::isDeleted() const {
        return deleted;
    }

    void Entity::markForDeletion() {
        deleted = true;
    }

    const int Entity::getId() const {
        return id;
    }

    const EntityType Entity::getType() const {
        return type;
    }

    int Entity::getX() const {
        return x;
    }

    int Entity::getY() const {
        return y;
    }

    int Entity::getWidth() const {
        return width;
    }

    int Entity::getHeight() const {
        return height;
    }

}  // namespace Rival
