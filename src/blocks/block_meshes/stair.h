#ifndef STAIR_BLOCK_MESH_H
#define STAIR_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getStairBlockMesh(const BlockStateStruct& state) {
    (void)state;

    BlockMesh block(2);
    block.elements[0] = BlockElement(0, 0, 0,    16,  8, 16,    0, 0, 0,    0, 0, 0,    6);
    block.elements[0].faces[0] = BlockFace(0,    1,    0, 0, 16,  8,    0);
    block.elements[0].faces[1] = BlockFace(1,    1,    0, 0, 16,  8,    0);
    block.elements[0].faces[2] = BlockFace(2,    1,    0, 0, 16, 16,    0);
    block.elements[0].faces[3] = BlockFace(3,    1,    0, 0, 16, 16,    0);
    block.elements[0].faces[4] = BlockFace(4,    1,    0, 0, 16,  8,    0);
    block.elements[0].faces[5] = BlockFace(5,    1,    0, 0, 16,  8,    0);
    block.elements[1] = BlockElement(0, 8, 0,    16, 16,  8,    0, 0, 0,    0, 0, 0,    6);
    block.elements[1].faces[0] = BlockFace(0,    1,    0, 8,  8, 16,    0);
    block.elements[1].faces[1] = BlockFace(1,    1,    0, 8,  8, 16,    0);
    block.elements[1].faces[2] = BlockFace(2,    1,    0, 0, 16,  8,    0);
    block.elements[1].faces[3] = BlockFace(3,    1,    0, 0, 16,  8,    0);
    block.elements[1].faces[4] = BlockFace(4,    1,    0, 8, 16, 16,    0);
    block.elements[1].faces[5] = BlockFace(5,    1,    0, 8, 16, 16,    0);

    return block;
}

void _loadStairBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::STAIR] = _getStairBlockMesh;
}

#endif