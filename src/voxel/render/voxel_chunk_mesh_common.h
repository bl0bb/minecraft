#ifndef VOXEL_CHUNK_MESH_COMMON_H
#define VOXEL_CHUNK_MESH_COMMON_H

#include "../../core/types.h"

struct VoxelFace {
    // pos
    u32 x;
    u32 y;
    u32 z;

    // face
    f32 face_x;
    f32 face_y;
    f32 face_z;

    f32 face_width;
    f32 face_height;

    f32 face_rot_x;
    f32 face_rot_y;
    f32 face_rot_z;

    // uv
    u32 uv_rot;

    u32 uv_x;
    u32 uv_y;

    u32 uv_w;
    u32 uv_h;

    // dir
    u32 dir;

    // type
    u32 type;

    // light
    u32 light[9];

    // ao
    u32 ao;

    VoxelFace() {}
    
    VoxelFace(
        // pos
        u32 _x,
        u32 _y,
        u32 _z,

        // face
        f32 _face_x,
        f32 _face_y,
        f32 _face_z,

        f32 _face_width,
        f32 _face_height,

        f32 _face_rot_x,
        f32 _face_rot_y,
        f32 _face_rot_z,

        // uv
        u32 _uv_rot,

        u32 _uv_x,
        u32 _uv_y,

        u32 _uv_w,
        u32 _uv_h,

        // dir
        u32 _dir,

        // type
        u32 _type,

        // light
        u32 _light[9],

        // ao
        u32 _ao
    ) :
        // pos
        x(_x),
        y(_y),
        z(_z),

        // face
        face_x(_face_x),
        face_y(_face_y),
        face_z(_face_z),

        face_width(_face_width),
        face_height(_face_height),

        face_rot_x(_face_rot_x),
        face_rot_y(_face_rot_y),
        face_rot_z(_face_rot_z),

        // uv
        uv_rot(_uv_rot),

        uv_x(_uv_x),
        uv_y(_uv_y),

        uv_w(_uv_w),
        uv_h(_uv_h),

        // dir
        dir(_dir),

        // type
        type(_type),

        // ao
        ao(_ao)
    {
        std::copy(_light, _light + 9, light);
    }
};

#endif