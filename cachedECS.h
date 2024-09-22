#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <queue>
#include <typeindex>
#include <memory>
#include <optional>
#include <map>

namespace ECS {
    const int MAX_ENTITIES{ 5000 };
    const int MAX_COMPONENTS{ 32 };

    using EntityID = int;
    using ComponentID = size_t; // Use size_t for component indices

    // Component Pools
    template<typename ComponentType>
    class ComponentPool {
    public:
        ComponentType& add(EntityID entity) {
            if (entity >= data.size()) {
                data.resize(entity + 1);
            }
            if (!data[entity].has_value()) {
                data[entity] = ComponentType(); // Initialize with default constructor
            }
            return data[entity].value();
        }

        void remove(EntityID entity) {
            if (entity < data.size()) {
                data[entity] = std::nullopt;
            }
        }

        ComponentType* get(EntityID entity) {
            if (entity < data.size() && data[entity].has_value()) {
                return &data[entity].value();
            }
            return nullptr;
        }

    private:
        std::vector<std::optional<ComponentType>> data;
    };

    class EntityComponent {
    public:
        EntityComponent() {
            for (int entity = 0; entity < MAX_ENTITIES; ++entity) {
                availableEntities.push(entity);
            }
        }

        EntityID createEntity() {
            if (availableEntities.empty()) {
                throw std::runtime_error("Max entities reached");
            }
            EntityID id = availableEntities.front();
            availableEntities.pop();
            entityComponentMap[id].reset();
            return id;
        }

        void destroyEntity(EntityID entity) {
            entityComponentMap.erase(entity);
            availableEntities.push(entity);
        }

        template<typename ComponentType>
        void addComponent(EntityID entity, ComponentType component) {
            getPool<ComponentType>().add(entity) = component;
            entityComponentMap[entity].set(getComponentID<ComponentType>());
        }

        template<typename ComponentType>
        void removeComponent(EntityID entity) {
            getPool<ComponentType>().remove(entity);
            entityComponentMap[entity].reset(getComponentID<ComponentType>());
        }

        template<typename ComponentType>
        ComponentType* getComponent(EntityID entity) {
            if (entityComponentMap[entity].test(getComponentID<ComponentType>())) {
                return getPool<ComponentType>().get(entity);
            }
            return nullptr;
        }

        template<typename... Components>
        bool hasComponents(EntityID entity) {
            return (entityComponentMap[entity].test(getComponentID<Components>()) && ...);
        }

        template<typename ComponentType>
        void updateComponent(EntityID entity, const ComponentType& component) {
            if (entityComponentMap.find(entity) == entityComponentMap.end()) {
                std::cerr << "Entity not found: " << entity << std::endl;
                return;
            }

            if (!entityComponentMap[entity].test(getComponentID<ComponentType>())) {
                std::cerr << "Component does not exist for entity: " << entity << ". Use addComponent to add it first.\n";
                return;
            }

            getPool<ComponentType>().add(entity) = component;
        }

    private:
        std::unordered_map<EntityID, std::bitset<MAX_COMPONENTS>> entityComponentMap;
        std::queue<EntityID> availableEntities;

        template<typename ComponentType>
        static ComponentID getComponentID() {
            static ComponentID id = nextID++;
            return id;
        }

        template<typename ComponentType>
        ComponentPool<ComponentType>& getPool() {
            static ComponentPool<ComponentType> pool;
            return pool;
        }

        static ComponentID nextID;
    };
} // namespace ECS