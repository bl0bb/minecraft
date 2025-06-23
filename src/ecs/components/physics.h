#ifndef ECS_PHYSICS_H
#define ECS_PHYSICS_H

#include "../../core/maths.h"
#include "../../physics/aabb/aabb.h"

struct ECSPhysics {
    Vec3<f32> velocity;
    AABB aabb;
};

#endif