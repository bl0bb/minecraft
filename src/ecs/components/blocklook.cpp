#include "blocklook.h"
#include "../ecs.h"

#include "../../physics/raycast/raycast.h"

static void tick(ECSBlockLook *component, const ECSEntity& entity) {
    ECSCamera& camera = *(ECSCamera*)entity.ecs->entityGetComponent(entity, ECS_CAMERA);
    RaycastResult raycastResult = raycast(*(VoxelBlockWorld*)entity.ecs->world, camera.camera.position, camera.camera.front * 16);
    component->hasBlock = raycastResult.success;
    if (component->hasBlock) {
        component->pos = raycastResult.blockPos;
        component->dir = raycastResult.dir;
    }
}

void ecs_blocklook_init(ECS* ecs) {
    ecs_register(ECS_BLOCKLOOK, ECSBlockLook, ecs, new ECSSystem(
        nullptr, // init
        nullptr, // destroy
        nullptr, // render
        nullptr, // update
        (ECSSubscriber)tick // tick
    ));
}