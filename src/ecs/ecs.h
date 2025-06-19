#ifndef ECS_H
#define ECS_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <typeindex>
#include <memory>
#include <iostream>

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;

class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
private:
    std::unordered_map<Entity, T> componentMap;
public:
    void InsertData(Entity entity, T component) {
        componentMap[entity] = component;
    }

    void RemoveData(Entity entity) {
        componentMap.erase(entity);
    }

    T& GetData(Entity entity) {
        return componentMap.at(entity);
    }

    bool HasData(Entity entity) {
        return componentMap.find(entity) != componentMap.end();
    }

    void EntityDestroyed(Entity entity) override {
        componentMap.erase(entity);
    }
};

class ComponentManager {
private:
    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> componentArrays;

    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray() {
        std::type_index typeName = typeid(T);
        if (componentArrays.find(typeName) == componentArrays.end()) {
            componentArrays[typeName] = std::make_shared<ComponentArray<T>>();
        }
        return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
    }

public:
    template<typename T>
    void AddComponent(Entity entity, T component) {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        GetComponentArray<T>()->RemoveData(entity);
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        return GetComponentArray<T>()->GetData(entity);
    }

    template<typename T>
    bool HasComponent(Entity entity) {
        return GetComponentArray<T>()->HasData(entity);
    }

    void EntityDestroyed(Entity entity) {
        for (auto& pair : componentArrays) {
            pair.second->EntityDestroyed(entity);
        }
    }
};

class EntityManager {
private:
    Entity nextEntity = 0;
    std::unordered_set<Entity> activeEntities;
public:
    Entity CreateEntity() {
        Entity id = nextEntity++;
        activeEntities.insert(id);
        return id;
    }

    void DestroyEntity(Entity entity) {
        activeEntities.erase(entity);
    }

    bool IsValid(Entity entity) {
        return activeEntities.find(entity) != activeEntities.end();
    }
};

class System {
public:
    std::unordered_set<Entity> entities;

    virtual void Init() {}
    virtual void Destroy() {}

    virtual void OnEntityAdded(Entity) {}
    virtual void OnEntityRemoved(Entity) {}

    virtual void Tick(ComponentManager&) {}
    virtual void Render(ComponentManager&) {}
};

class SystemManager {
private:
    std::vector<std::shared_ptr<System>> systems;
public:
    void AddSystem(std::shared_ptr<System> system) {
        systems.push_back(system);
        system->Init();
    }

    void TickAll(ComponentManager& components) {
        for (auto& system : systems) {
            system->Tick(components);
        }
    }

    void RenderAll(ComponentManager& components) {
        for (auto& system : systems) {
            system->Render(components);
        }
    }

    void DestroyAll() {
        for (auto& system : systems) {
            system->Destroy();
        }
    }
};

class ECS {
public:
    EntityManager entityManager;
    ComponentManager componentManager;
    SystemManager systemManager;

    Entity CreateEntity() {
        return entityManager.CreateEntity();
    }

    void DestroyEntity(Entity entity) {
        entityManager.DestroyEntity(entity);
        componentManager.EntityDestroyed(entity);
    }

    template<typename T>
    void AddComponent(Entity entity, T component) {
        componentManager.AddComponent<T>(entity, component);
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        return componentManager.GetComponent<T>(entity);
    }

    template<typename T>
    bool HasComponent(Entity entity) {
        return componentManager.HasComponent<T>(entity);
    }

    void Tick() {
        systemManager.TickAll(componentManager);
    }

    void Render() {
        systemManager.RenderAll(componentManager);
    }

    void Shutdown() {
        systemManager.DestroyAll();
    }
};


#endif