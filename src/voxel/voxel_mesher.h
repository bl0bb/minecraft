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
    std::vector<u64>* faces = nullptr;
};

void generate_voxel_mesh(const u8* voxels, MeshData& meshData);

#endif