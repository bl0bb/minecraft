#include "physics.h"
#include "../ecs.h"

static void tick(ECSPhysics *component, const ECSEntity& entity) {
    
}

void ecs_physics_init(ECS& ecs) {
    ecs_register(ECS_PHYSICS, ECSPhysics, ecs, new ECSSystem(
        nullptr, // init
        nullptr, // destroy
        nullptr, // render
        nullptr, // update
        (ECSSubscriber)tick // tick
    ));
}