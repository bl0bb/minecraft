#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include "components/position.h"
// #include "components/camera.h"
// #include "components/control.h"
#include "components/physics.h"
// #include "components/movement.h"
#include "components/blocklook.h"
// #include "components/debug.h"
// #include "components/light.h"

#define _ECS_DECL_COMPONENT(_name)\
    extern void _name##_init();\
    _name##_init(ecs);

static inline void _ecs_init_internal(ECS *ecs) {
    _ECS_DECL_COMPONENT(c_position);
    // _ECS_DECL_COMPONENT(c_camera);
    // _ECS_DECL_COMPONENT(c_control);
    _ECS_DECL_COMPONENT(c_physics);
    // _ECS_DECL_COMPONENT(c_movement);
    _ECS_DECL_COMPONENT(c_blocklook);
    // _ECS_DECL_COMPONENT(c_debug);
    // _ECS_DECL_COMPONENT(c_light);
}

#define ECSCOMPONENT_LAST C_LIGHT
enum ECSComponent {
    C_POSITION = 0,
    // C_CAMERA,
    // C_CONTROL,
    C_PHYSICS,
    // C_MOVEMENT,
    C_BLOCKLOOK,
    // C_DEBUG,
    // C_LIGHT,
};

#endif