#ifndef GLASS_PANE_BLOCK_MESH_H
#define GLASS_PANE_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getGlassPaneBlockMesh(const BlockStateStruct& state) {
    (void)state;
    BlockMesh block(1);
    block.elements[0] = BlockElement(0, 0, 0,    16, 16, 16,    0, 0, 0,    0, 0, 0,    6);
    block.elements[0].faces[0] = BlockFace(0,    1,    0, 0, 16, 16,    0);
    block.elements[0].faces[1] = BlockFace(1,    1,    0, 0, 16, 16,    0);
    block.elements[0].faces[2] = BlockFace(2,    1,    0, 0, 16, 16,    0);
    block.elements[0].faces[3] = BlockFace(3,    1,    0, 0, 16, 16,    0);
    block.elements[0].faces[4] = BlockFace(4,    1,    0, 0, 16, 16,    0);
    block.elements[0].faces[5] = BlockFace(5,    1,    0, 0, 16, 16,    0);
    return block;

    // auto connections = std::get<GlassPaneBlockState>(state).connections;

    // u8 hasRight = (connections >> 0) & 1;
    // u8 hasLeft  = (connections >> 1) & 1;
    // u8 hasBack  = (connections >> 2) & 1;
    // u8 hasFront = (connections >> 3) & 1;

    // u8 rightCount = 1;
    // u8 leftCount = 1;
    // u8 topCount = 1;
    // u8 bottomCount = 1;
    // u8 backCount = 1;
    // u8 frontCount = 1;
    // if (hasRight) { backCount++; frontCount++; topCount++; bottomCount++; }
    // if (hasLeft)  { backCount++; frontCount++; topCount++; bottomCount++; }
    // if (hasBack)  { rightCount++; leftCount++; topCount++; bottomCount++; }
    // if (hasFront) { rightCount++; leftCount++; topCount++; bottomCount++; }

    // u8 rightIdx = 0;
    // u8 leftIdx = 0;
    // u8 topIdx = 0;
    // u8 bottomIdx = 0;
    // u8 backIdx = 0;
    // u8 frontIdx = 0;

    // BlockMesh block(rightCount, leftCount, topCount, bottomCount, backCount, frontCount, false, false, false, false, false, false);

    // u8 rightDepth = hasRight ? 0 : 8;
    // u8 leftDepth = hasLeft ? 0 : 8;
    // u8 backDepth = hasBack ? 0 : 8;
    // u8 frontDepth = hasFront ? 0 : 8;
    // block.faces[0][rightIdx++]  = BlockFace(8, 0, rightDepth,    8, 16, rightDepth,    0,    0,    8, 0, 8, 16);
    // block.faces[1][leftIdx++]   = BlockFace(8, 0,  leftDepth,    8, 16,  leftDepth,    0,    0,    8, 0, 8, 16);
    // block.faces[2][topIdx++]    = BlockFace(8, 8,          0,    8,  8,          0,    0,    0,    8, 8, 8,  8);
    // block.faces[3][bottomIdx++] = BlockFace(8, 8,          0,    8,  8,          0,    0,    0,    8, 8, 8,  8);
    // block.faces[4][backIdx++]   = BlockFace(8, 0,  backDepth,    8, 16,  backDepth,    0,    0,    8, 0, 8, 16);
    // block.faces[5][frontIdx++]  = BlockFace(8, 0, frontDepth,    8, 16, frontDepth,    0,    0,    8, 0, 8, 16);


    // if (hasRight) {
    //     block.faces[2][topIdx++]    = BlockFace(8,  8,  0,    16,  8,  0,    0,    0,    8,  8, 16,  8);
    //     block.faces[3][bottomIdx++] = BlockFace(8,  8,  0,    16,  8,  0,    0,    0,    8,  8, 16,  8);
    //     block.faces[4][backIdx++]   = BlockFace(0,  0,  8,     7, 16,  8,    0,    0,    0,  0,  7, 16);
    //     block.faces[5][frontIdx++]  = BlockFace(9,  0,  8,    16, 16,  8,    0,    0,    9,  0, 16, 16);
    // }
    // if (hasLeft) {
    //     block.faces[2][topIdx++]    = BlockFace(0,  8,  0,     7,  8,  0,    0,    0,    0,  8,  7,  8);
    //     block.faces[3][bottomIdx++] = BlockFace(0,  8,  0,     7,  8,  0,    0,    0,    0,  8,  7,  8);
    //     block.faces[4][backIdx++]   = BlockFace(9,  0,  8,    16, 16,  8,    0,    0,    9,  0, 16, 16);
    //     block.faces[5][frontIdx++]  = BlockFace(0,  0,  8,     7, 16,  8,    0,    0,    0,  0,  7, 16);
    // }
    // if (hasBack) {
    //     block.faces[2][topIdx++]    = BlockFace(8,  8,  0,     8, 16,  0,    0,    0,    8,  8,  8, 16);
    //     block.faces[3][bottomIdx++] = BlockFace(8,  0,  0,     8,  8,  0,    0,    0,    8,  0,  8,  8);
    //     block.faces[0][rightIdx++]  = BlockFace(9,  0,  8,    16, 16,  8,    0,    0,    9,  0, 16, 16);
    //     block.faces[1][leftIdx++]   = BlockFace(0,  0,  8,     7, 16,  8,    0,    0,    0,  0,  7, 16);
    // }
    // if (hasFront) {
    //     block.faces[2][topIdx++]    = BlockFace(8,  0,  0,     8,  8,  0,    0,    0,    8,  0,  8,  8);
    //     block.faces[3][bottomIdx++] = BlockFace(8,  8,  0,     8, 16,  0,    0,    0,    8,  8,  8, 16);
    //     block.faces[0][rightIdx++]  = BlockFace(0,  0,  8,     7, 16,  8,    0,    0,    0,  0,  7, 16);
    //     block.faces[1][leftIdx++]   = BlockFace(9,  0,  8,    16, 16,  8,    0,    0,    9,  0, 16, 16);
    // }

    // return block;
}

void _loadGlassPaneBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::GLASS_PANE] = _getGlassPaneBlockMesh;
}

#endif