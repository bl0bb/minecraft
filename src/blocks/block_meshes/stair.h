#ifndef STAIR_BLOCK_MESH_H
#define STAIR_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getStairBlockMesh(const BlockStateStruct& state) {
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

        rightIdx = 4; // back
        leftIdx = 5; // front
        topIdx = 2; // top
        bottomIdx = 3; // bottom
        backIdx = 1; // left
        frontIdx = 0; // right
    } else if (dir == 1) {
        // left
        
        rightCount = 1;
        leftCount = 2;
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
    } else if (dir == 2) {
        // back

        rightCount = 2;
        leftCount = 2;
        topCount = 2;
        bottomCount = 1;
        backCount = 2;
        frontCount = 1;

        rightIdx = 1; // left
        leftIdx = 0; // right
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

    if (isFlipped) {
        u8 newRightCount = rightCount;
        u8 newLeftCount = leftCount;
        u8 newTopCount = topCount;
        u8 newBottomCount = bottomCount;
        u8 newBackCount = backCount;
        u8 newFrontCount = frontCount;

        u8 newTopIdx = topIdx;
        u8 newBottomIdx = bottomIdx;
        u8 newBackIdx = backIdx;
        u8 newFrontIdx = frontIdx;

        topIdx = newFrontIdx;
        bottomIdx = newBackIdx;
        backIdx = newTopIdx;
        frontIdx = newBottomIdx;

        if (dir < 2) {
            backCount = newBackCount;
            frontCount = newFrontCount;

            if (dir == 0) {
                // right

                rightCount = newTopCount;
                leftCount = newBottomCount;
                topCount = newLeftCount;
                bottomCount = newRightCount;
            } else {
                // left

                rightCount = newBottomCount;
                leftCount = newTopCount;
                topCount = newRightCount;
                bottomCount = newLeftCount;
            }
        } else {
            rightCount = newRightCount;
            leftCount = newLeftCount;

            if (dir == 2) {
                // back

                topCount = newFrontCount;
                bottomCount = newBackCount;
                backCount = newTopCount;
                frontCount = newBottomCount;
            } else {
                // front

                topCount = newBackCount;
                bottomCount = newFrontCount;
                backCount = newBottomCount;
                frontCount = newTopCount;
            }
        }
    }

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

    if (dir == 0) {
        // right

        // top / bottom
        for (u8 d = 2; d < 4; d++) {
            for (u8 i = 0; i < stair.counts[d]; i++) {
                BlockFace& face = stair.faces[d][i];

                i8 newFromX;
                i8 newFromY;
                i8 newToX;
                i8 newToY;
                if (isFlipped) {
                    newFromX = face.fromY;
                    newFromY = 16 - 1 - face.fromX;
                    newToX = face.toY;
                    newToY = 16 - 1 - face.toX;
                } else {
                    newFromX = 16 - 1 - face.fromY;
                    newFromY = face.fromX;
                    newToX = 16 - 1 - face.toY;
                    newToY = face.toX;
                }

                face.fromX = std::min(newFromX, newToX);
                face.fromY = std::min(newFromY, newToY);
                face.toX = std::max(newFromX, newToX);
                face.toY = std::max(newFromY, newToY);
            }
        }
    } else if (dir == 1) {
        // left

        // top / bottom
        for (u8 d = 2; d < 4; d++) {
            for (u8 i = 0; i < stair.counts[d]; i++) {
                BlockFace& face = stair.faces[d][i];

                i8 newFromX;
                i8 newFromY;
                i8 newToX;
                i8 newToY;
                if (isFlipped) {
                    newFromX = 16 - 1 - face.fromY;
                    newFromY = face.fromX;
                    newToX = 16 - 1 - face.toY;
                    newToY = face.toX;
                } else {
                    newFromX = face.fromY;
                    newFromY = 16 - 1 - face.fromX;
                    newToX = face.toY;
                    newToY = 16 - 1 - face.toX;
                }

                face.fromX = std::min(newFromX, newToX);
                face.fromY = std::min(newFromY, newToY);
                face.toX = std::max(newFromX, newToX);
                face.toY = std::max(newFromY, newToY);
            }
        }
    } else if (dir == 2) {
        // back

        // top / bottom
        for (u8 d = 2; d < 4; d++) {
            for (u8 i = 0; i < stair.counts[d]; i++) {
                BlockFace& face = stair.faces[d][i];

                i8 newFromX = 16 - 1 - face.fromX;
                i8 newFromY = 16 - 1 - face.fromY;
                i8 newToX = 16 - 1 - face.toX;
                i8 newToY = 16 - 1 - face.toY;

                face.fromX = std::min(newFromX, newToX);
                face.fromY = std::min(newFromY, newToY);
                face.toX = std::max(newFromX, newToX);
                face.toY = std::max(newFromY, newToY);
            }
        }
    }

    if (isFlipped) {
        // right
        for (u8 i = 0; i < stair.counts[rightIdx]; i++) {
            BlockFace& face = stair.faces[rightIdx][i];

            i8 newFromX = 16 - 1 - face.fromY;
            i8 newFromY = face.fromX;
            i8 newToX = 16 - 1 - face.toY;
            i8 newToY = face.toX;

            face.fromX = std::min(newFromX, newToX);
            face.fromY = std::min(newFromY, newToY);
            face.toX = std::max(newFromX, newToX);
            face.toY = std::max(newFromY, newToY);
        }

        // left
        for (u8 i = 0; i < stair.counts[leftIdx]; i++) {
            BlockFace& face = stair.faces[leftIdx][i];
            
            i8 newFromX = face.fromY;
            i8 newFromY = 16 - 1 - face.fromX;
            i8 newToX = face.toY;
            i8 newToY = 16 - 1 - face.toX;

            face.fromX = std::min(newFromX, newToX);
            face.fromY = std::min(newFromY, newToY);
            face.toX = std::max(newFromX, newToX);
            face.toY = std::max(newFromY, newToY);
        }
    }

    return stair;
}

void _loadStairBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::STAIR] = &_getStairBlockMesh;
}

#endif