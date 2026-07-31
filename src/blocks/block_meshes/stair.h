#ifndef STAIR_BLOCK_MESH_H
#define STAIR_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getStairBlockMesh(const BlockStateStruct& state) {
    auto direction = std::get<StairBlockState>(state).direction;
    bool isFlipped = direction & 0b100;
    auto facingDirection = direction & 0b011;

    u8 bottomStart = isFlipped ? 8 : 0;
    u8 topStart = isFlipped ? 0 : 8;

    BlockMesh block(2);
    block.elements[0] = BlockElement(0, bottomStart, 0,    16,  bottomStart + 8, 16,    0, 0, 0,    0, 0, 0,    6);
    block.elements[0].faces[0] = BlockFace(0,    0,    0, 0, 16,  8,    0);
    block.elements[0].faces[1] = BlockFace(1,    0,    0, 0, 16,  8,    0);
    block.elements[0].faces[2] = BlockFace(2,    0,    0, 0, 16, 16,    0);
    block.elements[0].faces[3] = BlockFace(3,    0,    0, 0, 16, 16,    0);
    block.elements[0].faces[4] = BlockFace(4,    0,    0, 0, 16,  8,    0);
    block.elements[0].faces[5] = BlockFace(5,    0,    0, 0, 16,  8,    0);
    if (facingDirection == 0) {
        block.elements[1] = BlockElement(0, topStart, 0,    8, topStart + 8,  16,    0, 0, 0,    0, 0, 0,    6);
        block.elements[1].faces[0] = BlockFace(0,    0,    0, topStart,  16, topStart + 8,    0);
        block.elements[1].faces[1] = BlockFace(1,    0,    0, topStart,  16, topStart + 8,    0);
        block.elements[1].faces[2] = BlockFace(2,    0,    8,        0,  16,           16,    0);
        block.elements[1].faces[3] = BlockFace(3,    0,    8,        0,  16,           16,    0);
        block.elements[1].faces[4] = BlockFace(4,    0,    0, topStart,   8, topStart + 8,    0);
        block.elements[1].faces[5] = BlockFace(5,    0,    8, topStart,  16, topStart + 8,    0);
    } else if (facingDirection == 1) {
        block.elements[1] = BlockElement(8, topStart, 0,    16, topStart + 8,  16,    0, 0, 0,    0, 0, 0,    6);
        block.elements[1].faces[0] = BlockFace(0,    0,    0, topStart,  16, topStart + 8,    0);
        block.elements[1].faces[1] = BlockFace(1,    0,    0, topStart,  16, topStart + 8,    0);
        block.elements[1].faces[2] = BlockFace(2,    0,    0,        0,   8,           16,    0);
        block.elements[1].faces[3] = BlockFace(3,    0,    0,        0,   8,           16,    0);
        block.elements[1].faces[4] = BlockFace(4,    0,    8, topStart,  16, topStart + 8,    0);
        block.elements[1].faces[5] = BlockFace(5,    0,    0, topStart,   8, topStart + 8,    0);
    } else if (facingDirection == 2) {
        block.elements[1] = BlockElement(0, topStart, 0,    16, topStart + 8,  8,    0, 0, 0,    0, 0, 0,    6);
        block.elements[1].faces[0] = BlockFace(0,    0,    8, topStart, 16, topStart + 8,    0);
        block.elements[1].faces[1] = BlockFace(1,    0,    0, topStart,  8, topStart + 8,    0);
        block.elements[1].faces[2] = BlockFace(2,    0,    0,        0, 16,            8,    0);
        block.elements[1].faces[3] = BlockFace(3,    0,    0,        8, 16,           16,    0);
        block.elements[1].faces[4] = BlockFace(4,    0,    0, topStart, 16, topStart + 8,    0);
        block.elements[1].faces[5] = BlockFace(5,    0,    0, topStart, 16, topStart + 8,    0);
    } else {
        block.elements[1] = BlockElement(0, topStart, 8,    16, topStart + 8,  16,    0, 0, 0,    0, 0, 0,    6);
        block.elements[1].faces[0] = BlockFace(0,    0,    0, topStart,  8, topStart + 8,    0);
        block.elements[1].faces[1] = BlockFace(1,    0,    8, topStart, 16, topStart + 8,    0);
        block.elements[1].faces[2] = BlockFace(2,    0,    0,        8, 16,           16,    0);
        block.elements[1].faces[3] = BlockFace(3,    0,    0,        0, 16,            8,    0);
        block.elements[1].faces[4] = BlockFace(4,    0,    0, topStart, 16, topStart + 8,    0);
        block.elements[1].faces[5] = BlockFace(5,    0,    0, topStart, 16, topStart + 8,    0);

        if (facingDirection != 3) {
            std::cout << "Invalid stair direction " << facingDirection << std::endl;
        }
    }

    return block;
}

static void _loadStairBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::STAIR] = _getStairBlockMesh;
}

#endif