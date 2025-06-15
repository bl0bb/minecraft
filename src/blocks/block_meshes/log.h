#ifndef LOG_BLOCK_MESH_H
#define LOG_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getLogBlockMesh(const BlockStateStruct& state) {
    auto dir = std::get<LogBlockState>(state).direction;

    BlockMesh block(1);
    block.elements[0] = BlockElement(0, 0, 0,    16, 16, 16,    0, 0, 0,    0, 0, 0,    6);
    block.elements[0].faces[0] = BlockFace(0,    1,    0, 0, 16, 16,    0);
    block.elements[0].faces[1] = BlockFace(1,    1,    0, 0, 16, 16,    0);
    block.elements[0].faces[2] = BlockFace(2,    1,    0, 0, 16, 16,    dir == 0 ? 1 : 0);
    block.elements[0].faces[3] = BlockFace(3,    1,    0, 0, 16, 16,    dir == 0 ? 1 : 0);
    block.elements[0].faces[4] = BlockFace(4,    1,    0, 0, 16, 16,    dir == 0 ? 1 : 0);
    block.elements[0].faces[5] = BlockFace(5,    1,    0, 0, 16, 16,    dir == 0 ? 1 : 0);
    return block;
}

static void _loadLogBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::LOG] = _getLogBlockMesh;
}

#endif