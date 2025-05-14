#ifndef BLOCKSTATES_H
#define BLOCKSTATES_H

#include <variant>

#include "../core/types.h"
#include "blockstate.h"

struct BlockBlockState : public BlockState {
    // a block is just a block
    // no state needed
};

struct SlabBlockState : public BlockState {
    // 0 = bottom
    // 1 = top
    u8 placement;
};

struct StairBlockState : public BlockState {
    // 0 = right
    // 1 = left
    // 2 = top
    // 3 = bottom
    // 4 = back
    // 5 = front
    // XYYY
    // where X is if the stair is facing up or down
    // and YYY is which direction the stair is facing
    u8 direction;

    StairBlockState() {}

    StairBlockState(u8 _direction) : direction(_direction) {

    }
};

using BlockStateStruct = std::variant<
    BlockBlockState,
    SlabBlockState,
    StairBlockState
>;

// namespace BlockStates {

// BlockStateStruct* getBlockState(BlockStateStruct* blockState) {
//     return;
// }

// }

#endif