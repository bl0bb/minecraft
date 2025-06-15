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

    u8 connCount = hasRight + hasLeft + hasBack + hasFront;
    BlockMesh block(1 + connCount);

    block.elements[0] = BlockElement(7, 0, 7,    9, 16, 9,    0, 0, 0,    0, 0, 0,    6 - connCount);

    u8 centerFaceIdx = 0;
    block.elements[0].faces[centerFaceIdx++] = BlockFace(2,    1,    7, 7, 9, 9,    0);
    block.elements[0].faces[centerFaceIdx++] = BlockFace(3,    1,    7, 7, 9, 9,    0);

    u8 elementIdx = 0;

    if (hasRight) {
        block.elements[++elementIdx] = BlockElement(9, 0, 7,    16, 16, 9,    0, 0, 0,    0, 0, 0,    5);
        block.elements[elementIdx].faces[0] = BlockFace(0,    1,    7, 0,  9, 16,    0);
        block.elements[elementIdx].faces[1] = BlockFace(4,    1,    9, 0, 16, 16,    0);
        block.elements[elementIdx].faces[2] = BlockFace(5,    1,    0, 0,  7, 16,    0);

        block.elements[elementIdx].faces[3] = BlockFace(2,    1,    7, 7, 16,  9,    0);
        block.elements[elementIdx].faces[4] = BlockFace(3,    1,    7, 7, 16,  9,    0);
    } else {
        block.elements[0].faces[centerFaceIdx++] = BlockFace(0,    0,    7, 0, 9, 16,    0);
    }

    if (hasLeft) {
        block.elements[++elementIdx] = BlockElement(0, 0, 7,    7, 16, 9,    0, 0, 0,    0, 0, 0,    5);
        block.elements[elementIdx].faces[0] = BlockFace(1,    1,    7, 0,  9, 16,    0);
        block.elements[elementIdx].faces[1] = BlockFace(5,    1,    0, 0,  7, 16,    0);
        block.elements[elementIdx].faces[2] = BlockFace(4,    1,    9, 0, 16, 16,    0);

        block.elements[elementIdx].faces[3] = BlockFace(2,    1,    7, 7, 16,  9,    0);
        block.elements[elementIdx].faces[4] = BlockFace(3,    1,    7, 7, 16,  9,    0);
    } else {
        block.elements[0].faces[centerFaceIdx++] = BlockFace(1,    0,    7, 0, 9, 16,    0);
    }

    if (hasBack) {
        block.elements[++elementIdx] = BlockElement(7, 0, 0,    9, 16, 7,    0, 0, 0,    0, 0, 0,    5);
        block.elements[elementIdx].faces[0] = BlockFace(4,    1,    7, 0,  9, 16,    0);
        block.elements[elementIdx].faces[1] = BlockFace(0,    1,    0, 0,  7, 16,    0);
        block.elements[elementIdx].faces[2] = BlockFace(1,    1,    9, 0, 16, 16,    0);

        block.elements[elementIdx].faces[3] = BlockFace(2,    1,    7, 7, 16,  9,    0);
        block.elements[elementIdx].faces[4] = BlockFace(3,    1,    7, 7, 16,  9,    0);
    } else {
        block.elements[0].faces[centerFaceIdx++] = BlockFace(4,    0,    7, 0, 9, 16,    0);
    }

    if (hasFront) {
        block.elements[++elementIdx] = BlockElement(7, 0, 9,    9, 16, 16,    0, 0, 0,    0, 0, 0,    5);
        block.elements[elementIdx].faces[0] = BlockFace(5,    1,    7, 0,  9, 16,    0);
        block.elements[elementIdx].faces[1] = BlockFace(0,    1,    9, 0, 16, 16,    0);
        block.elements[elementIdx].faces[2] = BlockFace(1,    1,    0, 0,  7, 16,    0);

        block.elements[elementIdx].faces[3] = BlockFace(2,    1,    7, 7, 16,  9,    0);
        block.elements[elementIdx].faces[4] = BlockFace(3,    1,    7, 7, 16,  9,    0);
    } else {
        block.elements[0].faces[centerFaceIdx++] = BlockFace(5,    0,    7, 0, 9, 16,    0);
    }

    return block;
}

static void _loadGlassPaneBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::GLASS_PANE] = _getGlassPaneBlockMesh;
}

#endif