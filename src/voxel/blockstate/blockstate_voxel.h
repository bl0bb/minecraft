#ifndef BLOCKSTATE_VOXEL_H
#define BLOCKSTATE_VOXEL_H

#include "../../core/maths.h"
#include "../../core/types.h"

#include "../../blocks/blocks.h"

// blockstate voxel
struct BlockStateVoxel {
    BlockStateStruct* state;

    BlockStateVoxel() {}

    BlockStateVoxel(BlockStateStruct* _state) : state(_state) {}
};

#endif