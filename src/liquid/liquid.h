#ifndef LIQUID_H
#define LIQUID_H

#include "../core/types.h"

struct LiquidVoxel {
    u8 level;

    LiquidVoxel() : level(8) {}
};

#endif