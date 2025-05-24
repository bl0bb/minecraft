#ifndef GLASS_PANE_BLOCK_MESH_H
#define GLASS_PANE_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getGlassPaneBlockMesh(const BlockStateStruct& state) {
    auto connections = std::get<GlassPaneBlockState>(state).connections;

    u8 hasRight = (connections >> 0) & 1;
    u8 hasLeft  = (connections >> 1) & 1;
    u8 hasBack  = (connections >> 2) & 1;
    u8 hasFront = (connections >> 3) & 1;

    u8 rightCount = 1;
    u8 leftCount = 1;
    u8 topCount = 1;
    u8 bottomCount = 1;
    u8 backCount = 1;
    u8 frontCount = 1;
    if (hasRight) { backCount++; frontCount++; topCount++; bottomCount++; }
    if (hasLeft)  { backCount++; frontCount++; topCount++; bottomCount++; }
    if (hasBack)  { rightCount++; leftCount++; topCount++; bottomCount++; }
    if (hasFront) { rightCount++; leftCount++; topCount++; bottomCount++; }

    u8 rightIdx = 0;
    u8 leftIdx = 0;
    u8 topIdx = 0;
    u8 bottomIdx = 0;
    u8 backIdx = 0;
    u8 frontIdx = 0;

    BlockMesh block(rightCount, leftCount, topCount, bottomCount, backCount, frontCount, false, false, false, false, false, false);

    block.faces[0][rightIdx++]  = BlockFace(0,    hasRight ? 0 : 7,    7, 0, 8, 15,    7, 0, 8, 15);
    block.faces[1][leftIdx++]   = BlockFace(0,    hasLeft  ? 0 : 7,    7, 0, 8, 15,    7, 0, 8, 15);
    block.faces[2][topIdx++]    = BlockFace(0,    0,                   7, 7, 8,  8,    7, 7, 8,  8);
    block.faces[3][bottomIdx++] = BlockFace(0,    0,                   7, 7, 8,  8,    7, 7, 8,  8);
    block.faces[4][backIdx++]   = BlockFace(0,    hasBack  ? 0 : 7,    7, 0, 8, 15,    7, 0, 8, 15);
    block.faces[5][frontIdx++]  = BlockFace(0,    hasFront ? 0 : 7,    7, 0, 8, 15,    7, 0, 8, 15);


    if (hasRight) {
        block.faces[2][topIdx++]    = BlockFace(0,    0,    7,  7, 15,  8,    7,  7, 15,  8);
        block.faces[3][bottomIdx++] = BlockFace(0,    0,    7,  7, 15,  8,    7,  7, 15,  8);
        block.faces[4][backIdx++]   = BlockFace(0,    7,    0,  0,  6, 15,    0,  0,  6, 15);
        block.faces[5][frontIdx++]  = BlockFace(0,    7,    9,  0, 15, 15,    9,  0, 15, 15);
    }
    if (hasLeft) {
        block.faces[2][topIdx++]    = BlockFace(0,    0,    0,  7,  6,  8,    0,  7,  6,  8);
        block.faces[3][bottomIdx++] = BlockFace(0,    0,    0,  7,  6,  8,    0,  7,  6,  8);
        block.faces[4][backIdx++]   = BlockFace(0,    7,    9,  0, 15, 15,    9,  0, 15, 15);
        block.faces[5][frontIdx++]  = BlockFace(0,    7,    0,  0,  6, 15,    0,  0,  6, 15);
    }
    if (hasBack) {
        block.faces[2][topIdx++]    = BlockFace(0,    0,    7,  7,  8, 15,    7,  7,  8, 15);
        block.faces[3][bottomIdx++] = BlockFace(0,    0,    7,  0,  8,  8,    7,  0,  8,  8);
        block.faces[0][rightIdx++]  = BlockFace(0,    7,    9,  0, 15, 15,    9,  0, 15, 15);
        block.faces[1][leftIdx++]   = BlockFace(0,    7,    0,  0,  6, 15,    0,  0,  6, 15);
    }
    if (hasFront) {
        block.faces[2][topIdx++]    = BlockFace(0,    0,    7,  0,  8,  8,    7,  0,  8,  8);
        block.faces[3][bottomIdx++] = BlockFace(0,    0,    7,  7,  8, 15,    7,  7,  8, 15);
        block.faces[0][rightIdx++]  = BlockFace(0,    7,    0,  0,  6, 15,    0,  0,  6, 15);
        block.faces[1][leftIdx++]   = BlockFace(0,    7,    9,  0, 15, 15,    9,  0, 15, 15);
    }

    return block;
}

void _loadGlassPaneBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::GLASS_PANE] = _getGlassPaneBlockMesh;
}

#endif