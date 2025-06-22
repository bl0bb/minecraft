#include "ecs.h"

void ECS::init(/*struct World *world*/) {
    capacity = 64;
    ids = (EntityID*)calloc(capacity, sizeof(EntityID));
    used = Bitmap::createWithInit(capacity, 1);
    nextEntityID = 1;
    // world = world;

    // defined in ecs.h
    _ecs_init_internal(this);
}

void ECS::callEvent(enum ECSEvent event) {
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
                ECSEntity entity = ECSEntity(ids[j], u64(j), this);
                func(component, entity);
            }
        }
    }
}


// register components
void ECS::_ecs_register_internal(enum ECSComponent id, i64 componentSize, ECSSystem *system) {
    ECSComponentList list = ECSComponentList(componentSize, system);
    list.components = calloc(capacity, list.getComponentSize());
    lists[id] = list;
}



// entity functions
// create a new entity
ECSEntity ECS::createEntity() {
    // entity index
    i64 i = 0;

    // search for an open id (checking 64 bits at a time)
    for (; i < capacity; i += 64) {
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

void ECS::deleteEntity(ECSEntity entity) {
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
void ECS::entityAddComponent(ECSEntity entity, enum ECSComponent component_id, void *value) {
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

void ECS::entityRemoveComponent(ECSEntity entity, enum ECSComponent component_id) {
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

bool ECS::entityHasComponent(ECSEntity entity, enum ECSComponent component) {
    // return ECS_TAG(ECSCL_GET(&entity.ecs->lists[component], entity.index)) & ECS_TAG_USED;
    return ECS_TAG(entity.ecs->lists[component].getComponentInList(entity.index)) & ECS_TAG_USED;
}

void* ECS::entityGetComponent(ECSEntity entity, enum ECSComponent component) {
    assert(entityHasComponent(entity, component));
    return entity.ecs->lists[component].getComponentInList(entity.index); // ECSCL_GET(&entity.ecs->lists[component], entity.index);
}


// clean up
void ECS::destroy(struct ECS *self) {
    used.destroy();
    free(ids);

    for (size_t i = 0; i <= ECSCOMPONENT_LAST; i++) {
        free(lists[i].components);
    }
}