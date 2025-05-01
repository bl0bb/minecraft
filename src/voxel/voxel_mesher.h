#ifndef VOXEL_MESHER_H
#define VOXEL_MESHER_H

#include <vector>

#include "../core/types.h"

#include "voxel_chunk.h"



inline int get_zxy_index(int x, int y, int z) {
    return z + (x * CS_P) + (y * CS_P2);
}

inline const int getAxisIndex(const int axis, const int a, const int b, const int c) {
    if (axis == 0) return b + (a * CS_P) + (c * CS_P2);
    else if (axis == 1) return b + (c * CS_P) + (a * CS_P2);
    else return c + (a * CS_P) + (b * CS_P2);
}

u32 generate_voxel_mesh(const EmbeddedVoxel* voxels, VoxelFace* vertices);

#endif