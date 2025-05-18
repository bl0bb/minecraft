#ifndef VOXEL_CHUNK_MESH_COMMON_H
#define VOXEL_CHUNK_MESH_COMMON_H

#include "../../core/types.h"

// x = 0-4 (5) (32)
// y = 5-9 (5) (32)
// z = 10-14 (5) (32)
// face_x = 15-18 (4) (16)
// face_y = 19-22 (4) (16)
// face_depth = 23-26 (4) (16)
// face_w = 27-30 (4) (16)
// face_h = 31-34 (4) (16)
// uv_x = 35-38 (4) (16)
// uv_y = 39-42 (4) (16)
// uv_w = 43-46 (4) (16)
// uv_h = 47-50 (4) (16)
// uv_rot = 51-52 (2) (4)
// dir = 53-55 (3) (8)
// type = 56-63 (8) (256)

struct VoxelFace {
    u64 data1;

    VoxelFace() {};
    
    VoxelFace(u64 x, u64 y, u64 z, u64 face_x, u64 face_y, u64 face_depth, u64 face_w, u64 face_h, u64 uv_x, u64 uv_y, u64 uv_w, u64 uv_h, u64 uv_rot, u64 dir, u64 type) :
    data1(  (x           <<  0) |
            (y           <<  5) |
            (z           << 10) |
            (face_x      << 15) |
            (face_y      << 19) |
            (face_depth  << 23) |
            (face_w      << 27) |
            (face_h      << 31) |
            (uv_x        << 35) |
            (uv_y        << 39) |
            (uv_w        << 43) |
            (uv_h        << 47) |
            (uv_rot      << 51) |
            (dir         << 53) |
            (type        << 56))
    {};
};

#endif