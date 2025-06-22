#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include "../core/types.h"

class ECS;

#define ENTITY_NONE 0

// using EntityID = u64;
typedef u64 EntityID;

struct ECSEntity {
    // id of the entity
    EntityID id;

    // index for lookup in component arrays
    u64 index;

    // the system this entity is part of
    ECS *ecs;

    // constructors
    ECSEntity() {}
    ECSEntity(EntityID _id, u64 _index, ECS *_ecs) : id(_id), index(_index), ecs(_ecs) {}
};

#endif