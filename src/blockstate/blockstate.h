#ifndef BLOCKSTATE_H
#define BLOCKSTATE_H

#include <string>

#include "../core/types.h"

typedef u8 BlockStateType;
namespace BlockStateTypes {
enum BlockStateTypeEnum : BlockStateType {
    BLOCK,
    SLAB,
    STAIR,
};
}

class BlockState {
public:
    BlockStateType type;
    std::string name;
};

#endif