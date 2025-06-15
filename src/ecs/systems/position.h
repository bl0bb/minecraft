#ifndef ECS_SYSTEM_POSITION_H
#define ECS_SYSTEM_POSITION_H

#include "../../core/maths.h"

struct PositionECS {
    Vec3<f32> pos;

    Vec3<i64> blockPos;
    bool blockChanged: 1;

    Vec3<i64> chunkPos;
    bool chunkChanged: 1;
};

#endif