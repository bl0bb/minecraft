#ifndef CROSS_BLOCK_MESH_H
#define CROSS_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getCrossBlockMesh(const BlockStateStruct& state) {
    (void)state;
    BlockMesh cross(1, 1, 1, 1, 1, 1, true, true, true, true, true, true);
    cross.faces[0][0] = BlockFace(0,    0,    0, 0, 16 - 1, 16 - 1,    0, 0, 16 - 1, 16 - 1);
    cross.faces[1][0] = BlockFace(0,    0,    0, 0, 16 - 1, 16 - 1,    0, 0, 16 - 1, 16 - 1);
    cross.faces[2][0] = BlockFace(0,    0,    0, 0, 16 - 1, 16 - 1,    0, 0, 16 - 1, 16 - 1);
    cross.faces[3][0] = BlockFace(0,    0,    0, 0, 16 - 1, 16 - 1,    0, 0, 16 - 1, 16 - 1);
    cross.faces[4][0] = BlockFace(0,    0,    0, 0, 16 - 1, 16 - 1,    0, 0, 16 - 1, 16 - 1);
    cross.faces[5][0] = BlockFace(0,    0,    0, 0, 16 - 1, 16 - 1,    0, 0, 16 - 1, 16 - 1);
    return cross;
}

void _loadCrossBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::CROSS] = _getCrossBlockMesh;
}

#endif