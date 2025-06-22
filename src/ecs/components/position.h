#ifndef ECS_POSITION_H
#define ECS_POSITION_H

#include "../../core/maths.h"

struct ECSPosition {
    Vec3<f32> pos;

    Vec3<u8> chunkBlock;
    bool chunkBlockChanged : 1;
    Vec3<i64> chunkPos;
    bool chunkPosChanged : 1;
};

#endif