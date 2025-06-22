#ifndef ECS_BLOCKLOOK_H
#define ECS_BLOCKLOOK_H

#include "../../core/maths.h"

struct ECSBlockLook {
    bool hasBlock : 1;
    Vec3<i64> pos;
    u8 dir;
};

#endif