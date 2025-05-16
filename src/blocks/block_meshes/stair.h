#ifndef STAIR_BLOCK_MESH_H
#define STAIR_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getStairBlockMesh(const BlockStateStruct& state) {
    if (false) {
        BlockMesh stair(2, 2, 2, 1, 1, 2);
        stair.faces[0][0] = BlockFace(0, 0,     0,     16 - 1,  8 - 1);
        stair.faces[0][1] = BlockFace(0, 9 - 1, 9 - 1, 16 - 1, 16 - 1);
        stair.faces[1][0] = BlockFace(0, 0,     0,     16 - 1,  8 - 1);
        stair.faces[1][1] = BlockFace(0, 0,     9 - 1,  8 - 1, 16 - 1);
        stair.faces[2][0] = BlockFace(8, 0,     0,     16 - 1,  8 - 1);
        stair.faces[2][1] = BlockFace(0, 0,     9 - 1, 16 - 1, 16 - 1);
        stair.faces[3][0] = BlockFace(0, 0,     0,     16 - 1, 16 - 1);
        stair.faces[4][0] = BlockFace(0, 0,     0,     16 - 1, 16 - 1);
        stair.faces[5][0] = BlockFace(0, 0,     0,     16 - 1,  8 - 1);
        stair.faces[5][1] = BlockFace(8, 0,     9 - 1, 16 - 1, 16 - 1);
        return stair;
    }

    // something here is creating a bug

    u8 rightCount;
    u8 leftCount;
    u8 topCount;
    u8 bottomCount;
    u8 backCount;
    u8 frontCount;
    
    u8 rightIdx;
    u8 leftIdx;
    u8 topIdx;
    u8 bottomIdx;
    u8 backIdx;
    u8 frontIdx;

    auto fullDir = std::get<StairBlockState>(state).direction;
    u8 dir = fullDir & 0b11;
    u8 isFlipped = (fullDir >> 2) & 0b1;
    if (dir == 0) {
        // right
        rightCount = 2;
        leftCount = 1;
        topCount = 2;
        bottomCount = 1;
        backCount = 2;
        frontCount = 2;

        rightIdx = 5; // front
        leftIdx = 4; // back
        topIdx = 2; // top
        bottomIdx = 3; // bottom
        backIdx = 0; // right
        frontIdx = 1; // left
    } else if (dir == 1) {
        // left
        rightCount = 1;
        leftCount = 2;
        topCount = 2;
        bottomCount = 1;
        backCount = 2;
        frontCount = 2;

        rightIdx = 4; // back
        leftIdx = 5; // front
        topIdx = 2; // top
        bottomIdx = 3; // bottom
        backIdx = 1; // left
        frontIdx = 0; // right
    } else if (dir == 2) {
        // back
        rightCount = 2;
        leftCount = 2;
        topCount = 2;
        bottomCount = 1;
        backCount = 2;
        frontCount = 1;

        rightIdx = 0; // right
        leftIdx = 1; // left
        topIdx = 2; // top
        bottomIdx = 3; // bottom
        backIdx = 5; // front
        frontIdx = 4; // back
    } else {
        // front
        rightCount = 2;
        leftCount = 2;
        topCount = 2;
        bottomCount = 1;
        backCount = 1;
        frontCount = 2;

        rightIdx = 0; // right
        leftIdx = 1; // left
        topIdx = 2; // top
        bottomIdx = 3; // bottom
        backIdx = 4; // back
        frontIdx = 5; // front
    }

    printf("mjuauuuu\n");

    BlockMesh stair(rightCount, leftCount, topCount, bottomCount, backCount, frontCount);

    stair.faces[rightIdx][0] =  BlockFace(0, 0,     0,     16 - 1,  8 - 1);
    stair.faces[rightIdx][1] =  BlockFace(0, 9 - 1, 9 - 1, 16 - 1, 16 - 1);
    stair.faces[leftIdx][0] =   BlockFace(0, 0,     0,     16 - 1,  8 - 1);
    stair.faces[leftIdx][1] =   BlockFace(0, 0,     9 - 1,  8 - 1, 16 - 1);
    stair.faces[topIdx][0] =    BlockFace(8, 0,     0,     16 - 1,  8 - 1);
    stair.faces[topIdx][1] =    BlockFace(0, 0,     9 - 1, 16 - 1, 16 - 1);
    stair.faces[bottomIdx][0] = BlockFace(0, 0,     0,     16 - 1, 16 - 1);
    stair.faces[backIdx][0] =   BlockFace(0, 0,     0,     16 - 1, 16 - 1);
    stair.faces[frontIdx][0] =  BlockFace(0, 0,     0,     16 - 1,  8 - 1);
    stair.faces[frontIdx][1] =  BlockFace(8, 0,     9 - 1, 16 - 1, 16 - 1);

    printf("bluh 2 (%i %i) (%i %i %i %i %i %i)\n", dir, isFlipped, rightCount, leftCount, topCount, bottomCount, backCount, frontCount);

    if (dir == 0) {
        // right

        // this aint working
        // come up with new method of rotating
        // also
        // some bug crashes program

        // top
        for (u8 i = 0; i < stair.counts[2]; i++) {
            BlockFace& face = stair.faces[2][i];

            i8 newFromX = 16 - 1 - face.fromY;
            i8 newFromY = 16 - 1 - face.fromX;
            i8 newToX = 16 - 1 - face.toY;
            i8 newToY = 16 - 1 - face.toX;

            face.fromX = newFromX;
            face.fromY = newFromY;
            face.toX = newToX;
            face.toY = newToY;
        }

        // bottom
        for (u8 i = 0; i < stair.counts[3]; i++) {
            BlockFace& face = stair.faces[3][i];
            
            i8 newFromX = 16 - 1 - face.fromY;
            i8 newFromY = 16 - 1 - face.fromX;
            i8 newToX = 16 - 1 - face.toY;
            i8 newToY = 16 - 1 - face.toX;

            face.fromX = newFromY;
            face.fromY = newFromX;
            face.toX = newToY;
            face.toY = newToX;
        }
    } else if (dir == 1) {
        // left
        
        // top
        for (u8 i = 0; i < stair.counts[2]; i++) {
            BlockFace& face = stair.faces[2][i];

            i8 newFromX = face.fromY;
            i8 newFromY = face.fromX;
            i8 newToX = face.toY;
            i8 newToY = face.toX;

            face.fromX = newFromX;
            face.fromY = newFromY;
            face.toX = newToX;
            face.toY = newToY;
        }

        // bottom
        for (u8 i = 0; i < stair.counts[3]; i++) {
            BlockFace& face = stair.faces[3][i];
            
            i8 newFromX = face.fromY;
            i8 newFromY = face.fromX;
            i8 newToX = face.toY;
            i8 newToY = face.toX;

            face.fromX = newFromY;
            face.fromY = newFromX;
            face.toX = newToY;
            face.toY = newToX;
        }
    } else if (dir == 2) {
        // back

        // top
        for (u8 i = 0; i < stair.counts[2]; i++) {
            BlockFace& face = stair.faces[2][i];
            face.fromX = 15 - face.fromX;
            face.fromY = 15 - face.fromY;
            face.toX = 15 - face.toX;
            face.toY = 15 - face.toY;
        }

        // bottom
        for (u8 i = 0; i < stair.counts[3]; i++) {
            BlockFace& face = stair.faces[3][i];
            face.fromX = 15 - face.fromX;
            face.fromY = 15 - face.fromY;
            face.toX = 15 - face.toX;
            face.toY = 15 - face.toY;
        }
    } else {
        // front
        
    }

    return stair;
}

void _loadStairBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::STAIR] = &_getStairBlockMesh;
}

#endif