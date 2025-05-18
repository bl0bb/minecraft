#ifndef LOG_BLOCK_MESH_H
#define LOG_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getLogBlockMesh(const BlockStateStruct& state) {
    auto dir = std::get<LogBlockState>(state).direction;

    BlockMesh block(1, 1, 1, 1, 1, 1, true, true, true, true, true, true);
    block.faces[0][0] = BlockFace(0,                   0,    0, 0, 16 - 1, 16 - 1,    0, 0, 16 - 1, 16 - 1);
    block.faces[1][0] = BlockFace(0,                   0,    0, 0, 16 - 1, 16 - 1,    0, 0, 16 - 1, 16 - 1);
    block.faces[2][0] = BlockFace(dir == 0 ? 1 : 0,    0,    0, 0, 16 - 1, 16 - 1,    0, 0, 16 - 1, 16 - 1);
    block.faces[3][0] = BlockFace(dir == 0 ? 1 : 0,    0,    0, 0, 16 - 1, 16 - 1,    0, 0, 16 - 1, 16 - 1);
    block.faces[4][0] = BlockFace(dir == 0 ? 1 : 0,    0,    0, 0, 16 - 1, 16 - 1,    0, 0, 16 - 1, 16 - 1);
    block.faces[5][0] = BlockFace(dir == 0 ? 1 : 0,    0,    0, 0, 16 - 1, 16 - 1,    0, 0, 16 - 1, 16 - 1);

    return block;
}

void _loadLogBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::LOG] = _getLogBlockMesh;
}

#endif