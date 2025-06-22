#ifndef ECS_PHYSICS_H
#define ECS_PHYSICS_H

#include <iostream>

// COMPONENTS
struct ECSPosition {
    Vec3<f32> pos;

    Vec3<u8> chunkBlock;
    bool chunkBlockChanged : 1;
    Vec3<i64> chunkPos;
    bool chunkPosChanged : 1;
};

// SYSTEM
class ECSPositionSystem : public ECSSystem {
private:
    ComponentManager* cm;

    template<typename T>
    size_t getTypeID() const {
        static size_t id = typeCounter++;
        return id;
    }

    inline static size_t typeCounter = 0;

public:
    ECSPositionSystem(ECSComponentManager* cm) : cm(cm) {}

    void tick(float dt) override {
        // for (EntityID id : entities) {
        //     auto pos = cm->getComponent<ECSPosition>(id);
        //     if (pos && vel) {
        //         pos->x += vel->dx * dt;
        //         pos->y += vel->dy * dt;
        //     }
        // }
    }

    void render() override {
        
    }

    ECSComponentMask getSignature() const override {
        ECSComponentMask mask;
        mask.set(getTypeID<Position>());
        mask.set(getTypeID<Velocity>());
        return mask;
    }
};

#endif