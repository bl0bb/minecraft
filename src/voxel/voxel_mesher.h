#ifndef VOXEL_MESHER_H
#define VOXEL_MESHER_H

#include <vector>

#include "../core/types.h"

// CS = chunk size
static constexpr u8 CS = 32;

// Padded chunk size
static constexpr u8 CS_P = CS + 2;
static constexpr u16 CS_2 = CS * CS;
static constexpr u16 CS_P2 = CS_P * CS_P;
static constexpr u16 CS_P3 = CS_P * CS_P * CS_P;


struct MeshData {
    std::vector<u64>* vertices = nullptr;
};



inline int get_zxy_index(int x, int y, int z) {
    return z + (x * CS_P) + (y * CS_P2);
}

inline const int getAxisIndex(const int axis, const int a, const int b, const int c) {
    if (axis == 0) return b + (a * CS_P) + (c * CS_P2);
    else if (axis == 1) return b + (c * CS_P) + (a * CS_P2);
    else return c + (a * CS_P) + (b * CS_P2);
}

// x = 0-4 (5) (32)
// y = 5-9 (5) (32)
// z = 10-14 (5) (32)
// w = 15-19 (5) (32)
// h = 20-24 (5) (32)
// dir = 25-37 (3) (8)
// type = 28-31 (4) (16)
inline const u64 getQuad(u64 x, u64 y, u64 z, u64 w, u64 h, u64 dir, u64 type) {
    return (type << 28) | (dir << 25) | (h << 20) | (w << 15) | (z << 10) | (y << 5) | x;
}

void generate_voxel_mesh(const EmbeddedVoxel* voxels, MeshData& meshData);

#endif