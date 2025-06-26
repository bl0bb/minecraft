#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include "components/position.h"
#include "components/camera.h"
// #include "components/control.h"
#include "components/physics.h"
// #include "components/movement.h"
#include "components/blocklook.h"
// #include "components/debug.h"
// #include "components/light.h"

#define _ECS_DECL_COMPONENT(_name)\
    extern void ecs_##_name##_init(ECS *ecs);\
    ecs_##_name##_init(ecs);

static inline void _ecs_init_internal(ECS *ecs) {
    _ECS_DECL_COMPONENT(position);
    _ECS_DECL_COMPONENT(camera);
    // _ECS_DECL_COMPONENT(control);
    _ECS_DECL_COMPONENT(physics);
    // _ECS_DECL_COMPONENT(movement);
    _ECS_DECL_COMPONENT(blocklook);
    // _ECS_DECL_COMPONENT(debug);
    // _ECS_DECL_COMPONENT(light);
}



// #define ECSCOMPONENT_LAST ECS_LIGHT
#define ECSCOMPONENT_LAST ECS_BLOCKLOOK

enum ECSComponent {
    ECS_POSITION = 0,
    ECS_CAMERA,
    // ECS_CONTROL,
    ECS_PHYSICS,
    // ECS_MOVEMENT,
    ECS_BLOCKLOOK,
    // ECS_DEBUG,
    // ECS_LIGHT,
};

#endif