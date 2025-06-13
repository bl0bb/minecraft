#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include "../core/types.h"

using ECSEntityId = u64;

struct ECSEntity {
    ECSEntityId id;
    u64 index;
    ECS* ecs;
};

#endif