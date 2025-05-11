#ifndef BLOCK_MESHES_H
#define BLOCK_MESHES_H

#include <vector>

#include "core/types.h"
#include "core/maths.h"

struct BlockFace {
    // // 0-3 (4) (16) from x
    // // 4-7 (4) (16) from y
    // // 8-11 (4) (16) to x
    // // 12-15 (4) (16) to y
    // u16 data;

    // BlockFace(u8 _fromX, u8 _fromY, u8 _toX, u8 _toY) : data((_toY << 12) | (_toX << 8) | (_fromY << 4) | _fromX) {}

    // inline u8 fromX() const {
    //     return data & 0b1111;
    // }
    
    // inline u8 fromY() const {
    //     return (data >> 4) & 0b1111;
    // }
    
    // inline u8 toX() const {
    //     return (data >> 8) & 0b1111;
    // }
    
    // inline u8 toY() const {
    //     return (data >> 12) & 0b1111;
    // }

    // inline Vec2<u8> from() const {
    //     return {fromX(), fromY()};
    // }

    // inline Vec2<u8> to() const {
    //     return {toX(), toY()};
    // }

    u8 depth;
    u8 fromX;
    u8 fromY;
    u8 toX;
    u8 toY;

    BlockFace(u8 _depth, u8 _fromX, u8 _fromY, u8 _toX, u8 _toY) : depth(_depth), fromX(_fromX), fromY(_fromY), toX(_toX), toY(_toY) {}

    inline Vec2<u8> from() const {
        return {fromX, fromY};
    }

    inline Vec2<u8> to() const {
        return {toX, toY};
    }
};

struct BlockMesh {
    BlockFace** faces;
    u8* counts;
    u8 facesCount = 0;

    BlockMesh(u8 pxCount, u8 nxCount, u8 pyCount, u8 nyCount, u8 pzCount, u8 nzCount) : faces((BlockFace**)malloc(sizeof(BlockFace*) * 6)), counts((u8*)malloc(sizeof(u8) * 6)) {
        counts[0] = pxCount;
        counts[1] = nxCount;
        counts[2] = pyCount;
        counts[3] = nyCount;
        counts[4] = pzCount;
        counts[5] = nzCount;
        
        faces[0] = (BlockFace*)malloc(sizeof(BlockFace) * pxCount);
        faces[1] = (BlockFace*)malloc(sizeof(BlockFace) * nxCount);
        faces[2] = (BlockFace*)malloc(sizeof(BlockFace) * pyCount);
        faces[3] = (BlockFace*)malloc(sizeof(BlockFace) * nyCount);
        faces[4] = (BlockFace*)malloc(sizeof(BlockFace) * pzCount);
        faces[5] = (BlockFace*)malloc(sizeof(BlockFace) * nzCount);
    }

    BlockMesh(BlockFace** _faces) : faces(_faces) {}
};

std::vector<BlockMesh> BLOCK_MESHES;

void loadBlocksMeshes() {
    // block
    BlockMesh block(1, 1, 1, 1, 1, 1);
    block.faces[0][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[1][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[2][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[3][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[4][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    block.faces[5][0] = BlockFace(0, 0, 0, 16 - 1, 16 - 1);
    BLOCK_MESHES.push_back(block);

    // slab
    BlockMesh slab(1, 1, 1, 1, 1, 1);
    slab.faces[0][0] = BlockFace(0,     0, 0, 16 - 1,  8 - 1);
    slab.faces[1][0] = BlockFace(0,     0, 0, 16 - 1,  8 - 1);
    slab.faces[2][0] = BlockFace(8 - 1, 0, 0, 16 - 1, 16 - 1);
    slab.faces[3][0] = BlockFace(8 - 1, 0, 0, 16 - 1, 16 - 1);
    slab.faces[4][0] = BlockFace(0,     0, 0, 16 - 1,  8 - 1);
    slab.faces[5][0] = BlockFace(0,     0, 0, 16 - 1,  8 - 1);
    BLOCK_MESHES.push_back(slab);

    // stairs
    BlockMesh stair(2, 2, 2, 1, 1, 2);
    stair.faces[0][0] = BlockFace(0,     0,     0,     16 - 1,  8 - 1);
    stair.faces[0][1] = BlockFace(0,     0,     8 - 1,  8 - 1,  8 - 1);
    stair.faces[1][0] = BlockFace(0,     0,     0,     16 - 1, 16 - 1);
    stair.faces[1][1] = BlockFace(0,     8 - 1, 8 - 1, 16 - 1, 16 - 1);
    stair.faces[2][0] = BlockFace(0,     0,     0,     16 - 1,  8 - 1);
    stair.faces[2][1] = BlockFace(8 - 1, 0,     8 - 1, 16 - 1, 16 - 1);
    stair.faces[3][0] = BlockFace(0,     0,     0,     16 - 1, 16 - 1);
    stair.faces[4][0] = BlockFace(0,     0,     0,     16 - 1, 16 - 1);
    stair.faces[5][0] = BlockFace(0,     0,     0,     16 - 1,  8 - 1);
    stair.faces[5][1] = BlockFace(8 - 1, 0,     8 - 1, 16 - 1, 16 - 1);
    BLOCK_MESHES.push_back(stair);
}

#endif