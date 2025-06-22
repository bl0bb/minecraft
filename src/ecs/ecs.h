#ifndef ECS_H
#define ECS_H

#include "../core/types.h"
#include "../core/bitmap.h"

#include "components.h"

#include <cstring>
#include <cassert>

// some memory alignment stuff
// idk if i need it
// better safe than sorry
#define ECS_TAG_SIZE 16

// c++ cries about arithmetic with void*
using ECSTag = u64;

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




#define ENTITY_NONE 0




typedef void (*ECSSubscriber)(void *component, ECSEntity entity);



// typedef u64 EntityID;
using EntityID = u64;

struct ECSEntity {
    // id of the entity
    EntityID id;

    // index for lookup in component arrays
    u64 index;

    // the system this entity is part of
    ECS *ecs;
};





#define ECSEVENT_LAST ECS_TICK
enum ECSEvent {
    ECS_INIT = 0, ECS_DESTROY, ECS_RENDER, ECS_UPDATE, ECS_TICK
};

class ECSSystem {
public:
    ECSSubscriber init, destroy, render, update, tick;
    ECSSubscriber subscribers[ECSEVENT_LAST + 1];
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
    i64 getComponentSize() {
        return componentSize + ECS_TAG_SIZE;
    }

    void *getComponentInList(u64 index) {
        return (u8*)components + ((index + ECS_TAG_SIZE) * getComponentSize()) + ECS_TAG_SIZE;
    }
};



class ECS {
public:
    // members
    ECSComponentList lists[ECSCOMPONENT_LAST + 1];
    EntityID *ids;
    Bitmap used;
    i64 capacity;
    EntityID nextEntityID;

    void init(/*struct World *world*/) {
        capacity = 64;
        ids = (EntityID*)calloc(capacity, sizeof(EntityID));
        used = Bitmap::createWithInit(capacity, 1);
        nextEntityID = 1;
        // world = world;

        // defined in ecs.h
        _ecs_init_internal(this);
    }

    void event(enum ECSEvent event) {
        for (i64 i = 0; i <= ECSCOMPONENT_LAST; i++) {
            ECSComponentList* list = &lists[i];
            
            // get this component's subscriber for this event
            ECSSubscriber func = list->system->subscribers[event];
            if (func == NULL) {
                continue;
            }

            for (i64 j = 0; j < capacity; j++) {
                void *component = list->getComponentInList(j); // ECSCL_GET(list, j);
                if (ECS_TAG(component) & ECS_TAG_USED) {
                    // func(component, (ECSEntity) { .id = ids[j], .index = j, .ecs = self });
                    func(component, ECSEntity(ids[j], j, this));
                }
            }
        }
    }


    // register components
    void _ecs_register_internal(enum ECSComponent id, i64 componentSize, ECSSystem *system) {
        ECSComponentList list = ECSComponentList(componentSize, system);
        list.components = calloc(capacity, list.getComponentSize());
        lists[id] = list;
    }



    // entity functions
    // create a new entity
    ECSEntity createEntity() {
        // entity index
        i64 i;

        // search for an open id (checking 64 bits at a time)
        for (i = 0; i < capacity; i += 64) {
            if (((u64*)used.data)[(i / 64)] != 0xFFFFFFFFFFFFFFFF) {
                break;
            }
        }

        if (i == capacity) {
            // we reached end of array which means we need to allocate more space
            i64 old_capacity = capacity;
            capacity *= 2;

            // reallocate bitmap, clear new allocation
            used.reAlloc(capacity);
            std::memset(
                ((u8*)used.data) + (BITMAP_SIZE_TO_BYTES(old_capacity)),
                0, BITMAP_SIZE_TO_BYTES(capacity) - BITMAP_SIZE_TO_BYTES(old_capacity));
            
            // reallocate index -> ID map, clear new allocation
            ids = (EntityID*)realloc(ids, capacity * sizeof(u64));
            std::memset(
                ids + old_capacity, 0,
                (capacity - old_capacity) * sizeof(u64));

            // reallocate component lists
            for (i64 j = 0; j <= ECSCOMPONENT_LAST; j++) {
                ECSComponentList* list = &lists[j];
                list->components = realloc(
                    list->components,
                    capacity * list->getComponentSize());

                // initialize new allocation
                std::memset(
                    (u8*)list->components + (old_capacity * list->getComponentSize()), 0,
                    (capacity - old_capacity) * list->getComponentSize()); 
            }
        } else {
            // we checked 64 bits at a time earlier
            // but we dont know which one of the bits are vacant
            // so thats what were finding now
            for (; i < capacity; i++) {
                if (!used.get(i)) {
                    break;
                }
            }
        }

        // mark this entity as used
        used.set(i);

        EntityID id = nextEntityID++;
        ids[i] = id;

        return ECSEntity(id, i, this);

        // return (ECSEntity) {
        //     .id = id,
        //     .index = i,
        //     .ecs = this
        // };
    }

    void deleteEntity(ECSEntity entity) {
        assert(used.get(entity.index));

        // remove components
        for (size_t j = 0; j <= ECSCOMPONENT_LAST; j++) {
            ECSComponentList* list = &lists[j];
            ECSSubscriber destroy = list->system->destroy;

            // mark this component as unused
            void *component = list->getComponentInList(entity.index); // ECSCL_GET(list, entity.index);
            *ECS_PTAG(component) &= ~ECS_TAG_USED;

            // run destructor if non-null
            if (destroy != NULL) {
                destroy(component, entity);
            }
        }

        // mark this entity's index as unused
        used.clr(entity.index);

        // remove this id map entry
        ids[entity.index] = ENTITY_NONE;
    }





    // entity component functions
    void entityAddComponent(ECSEntity entity, enum ECSComponent component_id, void *value) {
        ECSComponentList* list = &entity.ecs->lists[component_id];
        ECSSubscriber init = list->system->init;
        void *component = list->getComponentInList(entity.index); // ECSCL_GET(list, entity.index);

        // mark the component as used
        assert(!(ECS_TAG(component) & ECS_TAG_USED));
        *ECS_PTAG(component) |= ECS_TAG_USED;

        if (value != NULL) {
            memcpy(component, value, list->componentSize);
        }

        // run the initializer if it is not null
        if (init != NULL) {
            init(component, entity);
        }
    }

    void entityRemoveComponent(ECSEntity entity, enum ECSComponent component_id) {
        ECSComponentList* list = &entity.ecs->lists[component_id];
        ECSSubscriber destroy = list->system->destroy;
        void *component = list->getComponentInList(entity.index); // ECSCL_GET(list, entity.index);

        // mark the component as unused
        assert(ECS_TAG(component) & ECS_TAG_USED);
        *ECS_PTAG(component) &= ~ECS_TAG_USED;

        // run the destructor if it is not null
        if (destroy != NULL) {
            destroy(component, entity);
        }
    }

    bool entityHasComponent(ECSEntity entity, enum ECSComponent component) {
        // return ECS_TAG(ECSCL_GET(&entity.ecs->lists[component], entity.index)) & ECS_TAG_USED;
        return ECS_TAG(entity.ecs->lists[component]->getComponentInList(entity.index)) & ECS_TAG_USED;
    }

    void* entityGetComponent(ECSEntity entity, enum ECSComponent component) {
        assert(entityHasComponent(entity, component));
        return entity.ecs->lists[component]->getComponentInList(entity.index); // ECSCL_GET(&entity.ecs->lists[component], entity.index);
    }


    // clean up
    void destroy(struct ECS *self) {
        used.destroy();
        free(ids);

        for (size_t i = 0; i <= ECSCOMPONENT_LAST; i++) {
            free(lists[i].components);
        }
    }
}

#endif