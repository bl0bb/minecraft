#include "position.h"
#include "../ecs.h"

static void tick(ECSPosition *component, const ECSEntity& entity) {
    
}

void ecs_position_init(ECS& ecs) {
    ecs_register(ECS_POSITION, ECSPosition, ecs, new ECSSystem(
        nullptr, // init
        nullptr, // destroy
        nullptr, // render
        nullptr, // update
        (ECSSubscriber) tick // tick
    ));
}