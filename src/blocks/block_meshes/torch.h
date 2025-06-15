#ifndef TORCH_BLOCK_MESH_H
#define TORCH_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getTorchBlockMesh(const BlockStateStruct& state) {
    auto direction = std::get<TorchBlockState>(state).direction;
    (void)direction;

    BlockMesh block(1);
    
    if (direction == 0) {
        block.elements[0] = BlockElement(7, 0, 7,    9, 10, 9,    0, 0, 0,    0, 0, 0,    6);
    } else {
        block.elements[0] = BlockElement(-1, 3.5, 7,    1, 13.5, 9,    0, 3.5, 8,    0, 0, -22.5,    6);
    }

    block.elements[0].faces[0] = BlockFace(0,    0,    7, 0, 9, 10,    0);
    block.elements[0].faces[1] = BlockFace(1,    0,    7, 0, 9, 10,    0);
    block.elements[0].faces[2] = BlockFace(2,    0,    7, 8, 9, 10,    0);
    block.elements[0].faces[3] = BlockFace(3,    1,    7, 0, 9,  2,    0);
    block.elements[0].faces[4] = BlockFace(4,    0,    7, 0, 9, 10,    0);
    block.elements[0].faces[5] = BlockFace(5,    0,    7, 0, 9, 10,    0);

    return block;
}

static void _loadTorchBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::TORCH] = _getTorchBlockMesh;
}

#endif