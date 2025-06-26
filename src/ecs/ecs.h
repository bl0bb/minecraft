#ifndef ECS_H
#define ECS_H

#include "../core/types.h"
#include "../core/bitmap.h"

#include "entity.h"
#include "components.h"

#include <cstring>
#include <cassert>

// register define
#define ecs_register(_id, _C, _ecs, _system) _ecs->_ecs_register_internal((_id), sizeof(_C), (_system))


// some memory alignment stuff
// idk if i need it
// better safe than sorry
#define ECS_TAG_SIZE 16

// c++ cries about arithmetic with void*
// using ECSTag = u64;
typedef u64 ECSTag;

inline ECSTag *ECS_PTAG(void *p) {
    return ((ECSTag*)p - ECS_TAG_SIZE);
}

inline ECSTag ECS_TAG(void *p) {
    return *ECS_PTAG(p);
    // return *((ECSTag*)p - ECS_TAG_SIZE);
}

enum ECSTagValues {
    ECS_TAG_USED = 1 << 0
};





typedef void (*ECSSubscriber)(void *component, const ECSEntity& entity);





#define ECSEVENT_LAST ECS_TICK
enum ECSEvent {
    ECS_INIT = 0, ECS_DESTROY, ECS_RENDER, ECS_UPDATE, ECS_TICK
};

class ECSSystem {
public:
    ECSSubscriber init, destroy, render, update, tick;
    ECSSubscriber subscribers[ECSEVENT_LAST + 1];

    // constructors
    ECSSystem() {}
    ECSSystem(ECSSubscriber _init, ECSSubscriber _destroy, ECSSubscriber _render, ECSSubscriber _update, ECSSubscriber _tick) : init(_init), destroy(_destroy), render(_render), update(_update), tick(_tick) {
        subscribers[ECS_INIT] = init;
        subscribers[ECS_DESTROY] = destroy;
        subscribers[ECS_RENDER] = render;
        subscribers[ECS_UPDATE] = update;
        subscribers[ECS_TICK] = tick;
    }
};



class ECSComponentList {
public:
    // members
    // reference to the system this component is part of
    ECSSystem *system;

    // array of pointers to the components
    void *components;

    // size of each component individually
    i64 componentSize;


    // constructors
    ECSComponentList() {}
    ECSComponentList(i64 _componentSize, ECSSystem *_system) : componentSize(_componentSize), system(_system) {}


    // methods
    inline i64 getComponentSize() {
        return componentSize + ECS_TAG_SIZE;
    }

    inline void *getComponentInList(u64 index) {
        return (u8*)components + ((index + ECS_TAG_SIZE) * getComponentSize()) + ECS_TAG_SIZE;
    }
};


class ECSWorld {
public:

};


class ECS {
public:
    // members
    ECSComponentList lists[ECSCOMPONENT_LAST + 1];
    EntityID *ids;
    Bitmap used;
    i64 capacity;
    EntityID nextEntityID;

    // the world this entity is part of
    // can be any type of world
    // e.g. a world with different screen and window properties / functions for a UI system
    // or a world with blocks and entities for the 3d game world
    void *world;

    void init(/*struct World *world*/);

    void callEvent(enum ECSEvent event);


    // register components
    void _ecs_register_internal(enum ECSComponent id, i64 componentSize, ECSSystem *system);



    // entity functions
    // create a new entity
    ECSEntity createEntity();

    void deleteEntity(ECSEntity entity);





    // entity component functions
    void entityAddComponent(ECSEntity entity, enum ECSComponent component_id, void *value);

    void entityRemoveComponent(ECSEntity entity, enum ECSComponent component_id);

    bool entityHasComponent(ECSEntity entity, enum ECSComponent component);

    void* entityGetComponent(ECSEntity entity, enum ECSComponent component);


    // clean up
    void destroy(struct ECS *self);
};

#endif