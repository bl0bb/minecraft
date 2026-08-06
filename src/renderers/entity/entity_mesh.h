#ifndef ENTITY_MESH_H
#define ENTITY_MESH_H

#include "../../core/types.h"

struct EntityMeshElement {
    // pos
    f32 posX;
    f32 posY;
    f32 posZ;

    // rot
    f32 rotX;
    f32 rotY;
    f32 rotZ;

    // size
    f32 sizeX;
    f32 sizeY;
    f32 sizeZ;

    EntityMeshElement() {}
    
    EntityMeshElement(
        // pos
        f32 _posX,
        f32 _posY,
        f32 _posZ,

        // rot
        f32 _rotX,
        f32 _rotY,
        f32 _rotZ,

        // size
        f32 _sizeX,
        f32 _sizeY,
        f32 _sizeZ
    ) :
        // pos
        posX(_posX),
        posY(_posY),
        posZ(_posZ),

        // rot
        rotX(_rotX),
        rotY(_rotY),
        rotZ(_rotZ),

        // size
        sizeX(_sizeX),
        sizeY(_sizeY),
        sizeZ(_sizeZ)
    {}
};

struct EntityMeshFace {
    // element
    u32 element;

    // dir
    u32 dir;

    // uv
    u32 uv_rot;

    u32 uv_x;
    u32 uv_y;

    u32 uv_w;
    u32 uv_h;

    EntityMeshFace() {}
    
    EntityMeshFace(
        // element
        u32 _element,

        // dir
        u32 _dir,

        // uv
        u32 _uv_rot,

        u32 _uv_x,
        u32 _uv_y,

        u32 _uv_w,
        u32 _uv_h
    ) :
        // element
        element(_element),

        // dir
        dir(_dir),

        // uv
        uv_rot(_uv_rot),

        uv_x(_uv_x),
        uv_y(_uv_y),

        uv_w(_uv_w),
        uv_h(_uv_h)
    {}
};

#endif