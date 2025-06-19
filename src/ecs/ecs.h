#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <bitset>
#include <typeindex>
#include <type_traits>
#include <functional>

constexpr u64 MAX_COMPONENTS = 5000;
using EntityID = u32;
using ComponentMask = std::bitset<MAX_COMPONENTS>;

class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void remove(EntityID id) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
private:
    std::vector<T> components;
    std::vector<bool> valid;

public:
    void insert(EntityID id, T component) {
        if (id >= components.size()) components.resize(id + 1);
        components[id] = component;
        valid[id] = true;
    }

    void remove(EntityID id) override {
        if (id < valid.size()) valid[id] = false;
    }

    T* get(EntityID id) {
        if (id < components.size() && valid[id]) return &components[id];
        return nullptr;
    }
};

class ComponentManager {
private:
    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> arrays;
    std::unordered_map<EntityID, ComponentMask> entityMasks;

    template<typename T>
    ComponentArray<T>* getArray() {
        std::type_index type = typeid(T);
        if (!arrays.count(type)) {
            arrays[type] = std::make_unique<ComponentArray<T>>();
        }
        return static_cast<ComponentArray<T>*>(arrays[type].get());
    }

    template<typename T>
    size_t getComponentTypeID() {
        static size_t id = componentCounter++;
        return id;
    }

    inline static size_t componentCounter = 0;

public:
    template<typename T>
    void addComponent(EntityID id, T component) {
        getArray<T>()->insert(id, component);
        entityMasks[id].set(getComponentTypeID<T>(), true);
    }

    template<typename T>
    void removeComponent(EntityID id) {
        getArray<T>()->remove(id);
        entityMasks[id].set(getComponentTypeID<T>(), false);
    }

    template<typename T>
    T* getComponent(EntityID id) {
        return getArray<T>()->get(id);
    }

    ComponentMask getMask(EntityID id) const {
        auto it = entityMasks.find(id);
        return it != entityMasks.end() ? it->second : ComponentMask();
    }
};

class System {
protected:
    std::unordered_set<EntityID> entities;

public:
    virtual ~System() = default;
    virtual void init(EntityID) {}
    virtual void destroy(EntityID) {}
    virtual void tick(float) {}
    virtual void render() {}

    void registerEntity(EntityID id) {
        if (entities.insert(id).second) init(id);
    }

    void unregisterEntity(EntityID id) {
        if (entities.erase(id)) destroy(id);
    }

    virtual ComponentMask getSignature() const = 0;
};

class ECS {
private:
    EntityID nextID = 0;
    ComponentManager componentManager;
    std::unordered_map<std::type_index, std::unique_ptr<System>> systems;

    void updateEntitySystems(EntityID id) {
        auto mask = componentManager.getMask(id);
        for (auto& [_, sys] : systems) {
            auto sig = sys->getSignature();
            if ((mask & sig) == sig)
                sys->registerEntity(id);
            else
                sys->unregisterEntity(id);
        }
    }

public:
    EntityID createEntity() {
        EntityID id = nextID++;
        for (auto& [type, sys] : systems) {
            auto sig = sys->getSignature();
            if ((componentManager.getMask(id) & sig) == sig)
                sys->registerEntity(id);
        }
        return id;
    }

    void destroyEntity(EntityID id) {
        for (auto& [type, sys] : systems) {
            sys->unregisterEntity(id);
        }
    }

    template<typename T>
    void addComponent(EntityID id, T component) {
        componentManager.addComponent<T>(id, component);
        updateEntitySystems(id);
    }

    template<typename T>
    void removeComponent(EntityID id) {
        componentManager.removeComponent<T>(id);
        updateEntitySystems(id);
    }

    template<typename T>
    T* getComponent(EntityID id) {
        return componentManager.getComponent<T>(id);
    }

    template<typename T, typename... Args>
    T* addSystem(Args&&... args) {
        auto sys = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = sys.get();
        systems[typeid(T)] = std::move(sys);
        return ptr;
    }

    void tick(float dt) {
        for (auto& [_, sys] : systems) sys->tick(dt);
    }

    void render() {
        for (auto& [_, sys] : systems) sys->render();
    }
};