#ifndef VOXEL_CHUNK_MESH_COMMON_H
#define VOXEL_CHUNK_MESH_COMMON_H

#include "../../core/types.h"

// x = 0-4 (5) (32)
// y = 5-9 (5) (32)
// z = 10-14 (5) (32)
// dir = 15-17 (3) (8)
// type = 18-21 (4) (16)
typedef u64 VoxelFace;
constexpr inline VoxelFace getQuad(u64 x, u64 y, u64 z, u64 dir, u64 type) {
    return (type << 18) | (dir << 15) | (z << 10) | (y << 5) | x;
}

#endif