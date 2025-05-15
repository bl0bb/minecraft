#ifndef BLOCK_MESHES_H
#define BLOCK_MESHES_H

#include <vector>

#include "../../core/types.h"
#include "../../core/maths.h"

#include "block_mesh.h"

namespace BlockMeshTypes {
enum BlockMeshTypeEnum {
    BLOCK,
    SLAB,
    STAIR,
};
}

std::vector<BlockMesh> BLOCK_MESHES;

void loadBlocksMeshes() {
    // block
    BlockMesh block(1, 1, 1, 1, 1, 1);
    block.faces[0][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[1][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[2][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[3][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[4][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[5][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    BLOCK_MESHES.push_back(block);

    // slab
    BlockMesh slab(1, 1, 1, 1, 1, 1);
    slab.faces[0][0] = BlockFace(0, 0, 0, 16 - 1,  8 - 1);
    slab.faces[1][0] = BlockFace(0, 0, 0, 16 - 1,  8 - 1);
    slab.faces[2][0] = BlockFace(8, 0, 0, 16 - 1, 16 - 1);
    slab.faces[3][0] = BlockFace(8, 0, 0, 16 - 1, 16 - 1);
    slab.faces[4][0] = BlockFace(0, 0, 0, 16 - 1,  8 - 1);
    slab.faces[5][0] = BlockFace(0, 0, 0, 16 - 1,  8 - 1);
    BLOCK_MESHES.push_back(slab);

    // stairs
    BlockMesh stair(2, 2, 2, 1, 1, 2);
    stair.faces[0][0] = BlockFace(0, 0,     0,     16 - 1,  8 - 1);
    stair.faces[0][1] = BlockFace(0, 9 - 1, 9 - 1, 16 - 1, 16 - 1);
    stair.faces[1][0] = BlockFace(0, 0,     0,     16 - 1,  8 - 1);
    stair.faces[1][1] = BlockFace(0, 0,     9 - 1,  8 - 1, 16 - 1);
    stair.faces[2][0] = BlockFace(8, 0,     0,     16 - 1,  8 - 1);
    stair.faces[2][1] = BlockFace(0, 0,     9 - 1, 16 - 1, 16 - 1);
    stair.faces[3][0] = BlockFace(0, 0,     0,     16 - 1, 16 - 1);
    stair.faces[4][0] = BlockFace(0, 0,     0,     16 - 1, 16 - 1);
    stair.faces[5][0] = BlockFace(0, 0,     0,     16 - 1,  8 - 1);
    stair.faces[5][1] = BlockFace(8, 0,     9 - 1, 16 - 1, 16 - 1);
    BLOCK_MESHES.push_back(stair);
}

#endif