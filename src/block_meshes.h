#ifndef BLOCK_MESHES_H
#define BLOCK_MESHES_H

#include "core/types.h"

struct BlockFace {
    u8* vertices;
};

struct BlockMesh {
    BlockFace faces[6];
    u8 faceCull;

    bool cullsDir(u8 dir) {
        return (faceCull >> dir) & 0b1;
    }
};

#endif