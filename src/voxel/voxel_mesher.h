#ifndef VOXEL_MESHER_H
#define VOXEL_MESHER_H

#include <vector>

#include "../core/types.h"

// CS = chunk size
static constexpr u8 CS = 32;

// CH = chunk height 1024
static constexpr u16 CH = 1024;

// Padded chunk size
static constexpr u8 CS_P = CS + 2;
static constexpr u16 CS_2 = CS * CS;
static constexpr u16 CS_P2 = CS_P * CS_P;
static constexpr u16 CS_P3 = CS_P * CS_P * CS_P;

// voxel mask
static constexpr u64 P_MASK = ((1ull << CS) - 1) << 1;

struct MeshData {
    u64* faceMasks = nullptr;       // CS_2 * 6
    u64* opaqueMask = nullptr;      // CS_P2
    u8* forwardMerged = nullptr;    // CS_2
    u8* rightMerged = nullptr;      // CS
    std::vector<u64>* vertices = nullptr;
    int vertexCount = 0;
    int maxVertices = 0;
    int faceVertexBegin[6] = {0};
    int faceVertexLength[6] = {0};
};

void generate_voxel_mesh(const u8* voxels, MeshData& meshData);

#endif