#ifndef BLOCK_BLOCK_MESH_H
#define BLOCK_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getBlockBlockMesh(const BlockStateStruct& state) {
    BlockMesh block(1, 1, 1, 1, 1, 1);
    block.faces[0][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[1][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[2][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[3][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[4][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[5][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    return block;
}

void _loadBlockBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::BLOCK] = &_getBlockBlockMesh;
}

#endif