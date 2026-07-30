#include "physics.h"
#include "../ecs.h"

static void tick(ECSPhysics *component, const ECSEntity& entity) {
    // component->velocity += Vec3<f32>(0.0f, -32.656f, 0.0f) * deltaTime;

    // // drag
    // component->velocity -= ((Vec3<f32>(component->velocity.x, 0, component->velocity.z) * 5.0f) * deltaTime);


    // Vec3<f32> stepDir = component->velocity * deltaTime;

    // Intersection intersection = component->aabb.getIntersection(entity.ecs.world, stepDir);

    // if (intersection.intersects) {
    //     component->aabb.pos += intersection.intersectDir;
    //     component->velocity *= intersection.collideSolveForce;
    // } else {
    //     component->aabb.pos += stepDir;
    // }
}

void ecs_physics_init(ECS* ecs) {
    ecs_register(ECS_PHYSICS, ECSPhysics, ecs, new ECSSystem(
        nullptr, // init
        nullptr, // destroy
        nullptr, // render
        nullptr, // update
        (ECSSubscriber)tick // tick
    ));
}