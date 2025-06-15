#ifndef CROSS_BLOCK_MESH_H
#define CROSS_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getCrossBlockMesh(const BlockStateStruct& state) {
    (void)state;
    BlockMesh cross(2);
    
    cross.elements[0] = BlockElement(0, 0, 8,    16, 16, 8,    8, 8, 8,    0,  45, 0,    2);
    cross.elements[0].faces[0] = BlockFace(4,    0,    0, 0, 16, 16,    0);
    cross.elements[0].faces[1] = BlockFace(5,    0,    0, 0, 16, 16,    0);

    cross.elements[1] = BlockElement(0, 0, 8,    16, 16, 8,    8, 8, 8,    0, -45, 0,    2);
    cross.elements[1].faces[0] = BlockFace(4,    0,    0, 0, 16, 16,    0);
    cross.elements[1].faces[1] = BlockFace(5,    0,    0, 0, 16, 16,    0);

    return cross;
}

static void _loadCrossBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::CROSS] = _getCrossBlockMesh;
}

#endif