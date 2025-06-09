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
    CROSS,
};
}

struct BlockFace {
    u8 dir;
    u8 culls;

    u16 uvFromX;
    u16 uvFromY;
    u16 uvToX;
    u16 uvToY;

    u8 uvRot;
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
    
    bool culls(u8 dir) {
        for (u8 i = 0; i < elementCount; i++) {
            BlockElement& element = elements[i];
            for (u8 j = 0; j < element.facesCount; j++) {
                BlockFace& face = element.faces[j];
                if (face.dir != dir) {
                    continue;
                }
                if (face.culls) {
                    return true;
                }
            }
        }
        return false;
    }
};

typedef BlockMesh (*BlockMeshFunc)(const BlockStateStruct& state);

BlockMeshFunc* BLOCK_MESHES = (BlockMeshFunc*)malloc(sizeof(BlockMeshFunc) * (BlockMeshTypes::TORCH + 1));

#endif