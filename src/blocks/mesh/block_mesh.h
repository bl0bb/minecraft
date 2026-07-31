#ifndef BLOCK_MESH_H
#define BLOCK_MESH_H

#include <vector>

#include "../../core/types.h"
#include "../../core/maths.h"
#include "../blockstate.h"

namespace BlockMeshTypes {
enum BlockMeshTypeEnum {
    BLOCK,
    SLAB,
    STAIR,
    LOG,
    TORCH,
    GLASS_PANE,
    CROSS,
    LEAVES,
};
}

struct BlockFace {
    // 0 = right
    // 1 = left
    // 2 = top
    // 3 = bottom
    // 4 = back
    // 5 = front
    u8 dir;
    u8 culls;

    u16 uvFromX;
    u16 uvFromY;
    u16 uvToX;
    u16 uvToY;

    u8 uvRot;

    BlockFace(u8 _dir, u8 _culls, u16 _uvFromX, u16 _uvFromY, u16 _uvToX, u16 _uvToY, u8 _uvRot) : dir(_dir), culls(_culls), uvFromX(_uvFromX), uvFromY(_uvFromY), uvToX(_uvToX), uvToY(_uvToY), uvRot(_uvRot) {}
};

struct BlockElement {
    f32 fromX;
    f32 fromY;
    f32 fromZ;
    f32 toX;
    f32 toY;
    f32 toZ;

    f32 rotOrigX;
    f32 rotOrigY;
    f32 rotOrigZ;

    f32 rotX;
    f32 rotY;
    f32 rotZ;

    u8 facesCount;
    BlockFace* faces;

    BlockElement(
        f32 _fromX,
        f32 _fromY,
        f32 _fromZ,
        f32 _toX,
        f32 _toY,
        f32 _toZ,

        f32 _rotOrigX,
        f32 _rotOrigY,
        f32 _rotOrigZ,

        f32 _rotX,
        f32 _rotY,
        f32 _rotZ,

        u8 _facesCount) :
        fromX(_fromX),
        fromY(_fromY),
        fromZ(_fromZ),
        toX(_toX),
        toY(_toY),
        toZ(_toZ),

        rotOrigX(_rotOrigX),
        rotOrigY(_rotOrigY),
        rotOrigZ(_rotOrigZ),

        rotX(_rotX),
        rotY(_rotY),
        rotZ(_rotZ),

        facesCount(_facesCount),
        faces((BlockFace*)malloc(sizeof(BlockFace) * facesCount))
        {}

    inline Vec3<f32> from() const {
        return {fromX, fromY, fromZ};
    }

    inline Vec3<f32> to() const {
        return {toX, toY, toZ};
    }

    inline Vec3<f32> rotOrig() const {
        return {rotOrigX, rotOrigY, rotOrigZ};
    }

    inline Vec3<f32> rot() const {
        return {rotX, rotY, rotZ};
    }
};

struct BlockMesh {
    u8 elementCount;
    BlockElement* elements;

    BlockMesh(u8 _elementCount) :
        elements((BlockElement*)malloc(sizeof(BlockElement) * _elementCount)),
        elementCount(_elementCount)
        {}
    
    bool culls(u8 dir);
};

typedef BlockMesh (*BlockMeshFunc)(const BlockStateStruct& state);

extern BlockMeshFunc* BLOCK_MESHES;

#endif