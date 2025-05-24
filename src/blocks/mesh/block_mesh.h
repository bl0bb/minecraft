#ifndef BLOCK_MESH_H
#define BLOCK_MESH_H

#include <vector>

#include "../../core/types.h"
#include "../../core/maths.h"

namespace BlockMeshTypes {
enum BlockMeshTypeEnum {
    BLOCK,
    SLAB,
    STAIR,
    LOG,
    TORCH,
    GLASS_PANE,
};
}

struct BlockFace {
    u8 uvRot;

    u8 depth;

    u8 fromX;
    u8 fromY;
    u8 toX;
    u8 toY;

    u8 uvFromX;
    u8 uvFromY;
    u8 uvToX;
    u8 uvToY;

    BlockFace(
        u8 _uvRot,

        u8 _depth,

        u8 _fromX,
        u8 _fromY,
        u8 _toX,
        u8 _toY,

        u8 _uvFromX,
        u8 _uvFromY,
        u8 _uvToX,
        u8 _uvToY) :
        uvRot(_uvRot),

        depth(_depth),

        fromX(_fromX),
        fromY(_fromY),
        toX(_toX),
        toY(_toY),

        uvFromX(_uvFromX),
        uvFromY(_uvFromY),
        uvToX(_uvToX),
        uvToY(_uvToY)
        
        {}

    inline Vec2<u8> from() const {
        return {fromX, fromY};
    }

    inline Vec2<u8> to() const {
        return {toX, toY};
    }

    inline u8 width() const {
        return toX - fromX;
    }

    inline u8 height() const {
        return toY - fromY;
    }


    inline Vec2<u8> uvFrom() const {
        return {uvFromX, uvFromY};
    }

    inline Vec2<u8> uvTo() const {
        return {uvToX, uvToY};
    }

    inline u8 uvWidth() const {
        return uvToX - uvFromX;
    }

    inline u8 uvHeight() const {
        return uvToY - uvFromY;
    }
};

struct BlockMesh {
    BlockFace** faces;
    u8* counts;
    u8 cullFlag;

    BlockMesh(
        u8 pxCount,
        u8 nxCount,
        u8 pyCount,
        u8 nyCount,
        u8 pzCount,
        u8 nzCount,

        bool cullRight,
        bool cullLeft,
        bool cullTop,
        bool cullBottom,
        bool cullBack,
        bool cullFront) :

        faces((BlockFace**)malloc(sizeof(BlockFace*) * 6)),
        counts((u8*)malloc(sizeof(u8) * 6)),
        cullFlag(
            (cullRight  << 0) |
            (cullLeft   << 1) |
            (cullTop    << 2) |
            (cullBottom << 3) |
            (cullBack   << 4) |
            (cullFront  << 5)
        )
        {
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

    constexpr inline bool culls(u8 dir) {
        return (cullFlag >> dir) & 0b1;
    }
};

typedef BlockMesh (*BlockMeshFunc)(const BlockStateStruct& state);

BlockMeshFunc* BLOCK_MESHES = (BlockMeshFunc*)malloc(sizeof(BlockMeshFunc) * (BlockMeshTypes::TORCH + 1));

#endif