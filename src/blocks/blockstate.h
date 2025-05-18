#ifndef BLOCKSTATE_H
#define BLOCKSTATE_H

#include <variant>
#include <string>

#include "../core/types.h"

// definition
typedef u8 BlockStateType;
namespace BlockStateTypes {
enum BlockStateTypeEnum : BlockStateType {
    BLOCK,
    SLAB,
    STAIR,
    LOG,
    TORCH,
};
}

class BlockState {
public:
    BlockStateType type;
    std::string name;
};




// states

// standard
struct BlockBlockState : public BlockState {
    // a block is just a block
    // no state needed
};

struct SlabBlockState : public BlockState {
    // 0 = bottom
    // 1 = top
    u8 placement;

    SlabBlockState() {}

    SlabBlockState(u8 _placement) : placement(_placement) {}

    // Copy constructor
    SlabBlockState(const SlabBlockState& other) : BlockState(other), placement(other.placement) {}
};

struct StairBlockState : public BlockState {
    // 0 = right
    // 1 = left
    // 2 = back
    // 3 = front
    // XYY
    // where X is if the stair is facing up or down
    // and YYY is which direction the stair is facing
    u8 direction;

    StairBlockState() {}

    StairBlockState(u8 _direction) : direction(_direction) {}

    // Copy constructor
    StairBlockState(const StairBlockState& other) : BlockState(other), direction(other.direction) {}
};

// directional
struct LogBlockState : public BlockState {
    // 0 = right / left
    // 1 = top / bottom
    // 2 = back / front
    u8 direction;

    LogBlockState() {}

    LogBlockState(u8 _direction) : direction(_direction) {}

    // Copy constructor
    LogBlockState(const LogBlockState& other) : BlockState(other), direction(other.direction) {}
};

// advanced
struct TorchBlockState : public BlockState {
    // 0 = top
    // 1 = right
    // 2 = left
    // 3 = back
    // 4 = front
    u8 direction;

    TorchBlockState() {}

    TorchBlockState(u8 _direction) : direction(_direction) {}

    // Copy constructor
    TorchBlockState(const TorchBlockState& other) : BlockState(other), direction(other.direction) {}
};

using BlockStateStruct = std::variant<
    BlockBlockState,
    SlabBlockState,
    StairBlockState,
    LogBlockState,
    TorchBlockState
>;

#endif