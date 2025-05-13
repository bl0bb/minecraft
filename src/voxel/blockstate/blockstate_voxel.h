#ifndef BLOCKSTATE_VOXEL_H
#define BLOCKSTATE_VOXEL_H

#include "../../core/maths.h"
#include "../../core/types.h"

#include "../../blocks.h"

// blockstate voxel
struct BlockStateVoxel;
using BlockStateValue = std::variant<
    i8,
    i16,
    i32,
    i64,
    f32,
    f64,
    std::string,
    std::vector<i8>
>;
struct BlockStateVoxel {
    BlockStateValue state;

    BlockStateVoxel() {

    }
};

#endif