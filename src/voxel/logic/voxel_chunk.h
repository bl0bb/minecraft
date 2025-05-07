#ifndef VOXEL_CHUNK_H
#define VOXEL_CHUNK_H

#include <glad/glad.h>

#include <cstring>

#include "../../core/types.h"
#include "../../core/shader.h"

#include "voxel.h"


// CS = chunk size
// CS_P = chunk size padded
static constexpr u8 CS = 32;
static constexpr u16 CS_2 = CS * CS;
static constexpr u16 CS_3 = CS * CS * CS;
static constexpr u8 CS_P = CS + 2;
static constexpr u16 CS_P2 = CS_P * CS_P;
static constexpr u16 CS_P3 = CS_P * CS_P * CS_P;



// helpers
constexpr inline int get_zxy_index_p(int x, int y, int z) {
    return z + (x * CS_P) + (y * CS_P2);
}
constexpr inline int get_zxy_index(int x, int y, int z) {
    return z + (x * CS) + (y * CS_2);
}

constexpr inline int getAxisIndex(const int axis, const int a, const int b, const int c) {
    if (axis == 0) return b + (a * CS_P) + (c * CS_P2);
    else if (axis == 1) return b + (c * CS_P) + (a * CS_P2);
    else return c + (a * CS_P) + (b * CS_P2);
}


template<typename VoxelType>
class BaseVoxelChunk {
public:
    using voxel_type = VoxelType;

    VoxelType* voxels;

    BaseVoxelChunk() {
        voxels = new VoxelType[CS_3];
        std::memset(voxels, 0, CS_3);
    }

    ~BaseVoxelChunk() {
        // C++ LOOOOVES TO CALL THIS WHEN I DONT WANT TO SO DONT DO ANYTHING
        // free(voxels);
    }
};

class VoxelChunk : public BaseVoxelChunk<EmbeddedVoxel> {
public:
    Vec3<i64> pos;
};

#endif