#include "camera.h"
#include "../ecs.h"

static void tick(ECSCamera *component, const ECSEntity& entity) {
    
}

void ecs_camera_init(ECS* ecs) {
    ecs_register(ECS_CAMERA, ECSCamera, ecs, new ECSSystem(
        nullptr, // init
        nullptr, // destroy
        nullptr, // render
        nullptr, // update
        (ECSSubscriber)tick // tick
    ));
}