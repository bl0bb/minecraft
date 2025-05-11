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
// dir = 35-37 (3) (8)
// type = 38-41 (4) (16)
typedef u64 VoxelFace;
constexpr inline VoxelFace getQuad(u64 x, u64 y, u64 z, u64 face_x, u64 face_y, u64 face_depth, u64 face_w, u64 face_h, u64 dir, u64 type) {
    return (type << 38) | (dir << 35) | (face_h << 31) | (face_w << 27) | (face_depth << 23) | (face_y << 19) | (face_x << 15) | (z << 10) | (y << 5) | x;
}

#endif