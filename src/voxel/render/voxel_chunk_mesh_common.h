#ifndef VOXEL_CHUNK_MESH_COMMON_H
#define VOXEL_CHUNK_MESH_COMMON_H

#include "../../core/types.h"

// x = 0-4 (5) (32)
// y = 5-9 (5) (32)
// z = 10-14 (5) (32)
// w = 15-19 (5) (32)
// h = 20-24 (5) (32)
// dir = 25-37 (3) (8)
// type = 28-31 (4) (16)
typedef u64 VoxelFace;
constexpr inline VoxelFace getQuad(u64 x, u64 y, u64 z, u64 w, u64 h, u64 dir, u64 type) {
    return (type << 28) | (dir << 25) | (h << 20) | (w << 15) | (z << 10) | (y << 5) | x;
}

#endif