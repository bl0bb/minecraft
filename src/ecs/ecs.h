#ifndef ECS_H
#define ECS_H

#include <vector>

#include "../core/types.h"

typedef void (*ECSSubscriber)(void *, struct Entity);

struct ECSSystem {
    ECSSubscriber init;
    ECSSubscriber destroy;
    ECSSubscriber render;
    ECSSubscriber update;
    ECSSubscriber tick;
};

struct ECSComponentList {
    ECSSystem* system;
    std::vector<void*> components;
};

struct ECS {
    ECSComponentList* lists;
    ECSEntityId* ids
    u64 size;
    ECSEntityId next_entity_id;
};

enum class ECSTagValues : u8 {
    ECS_TAG_USED = 1 << 0,
};

#endif