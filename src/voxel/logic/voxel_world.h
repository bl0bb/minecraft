#ifndef VOXEL_WORLD_H
#define VOXEL_WORLD_H

#include "voxel_chunk.h"

template<typename ChunkType>
class VoxelWorld {
public:
    ChunkType* chunks;
    Vec3<u64> size;

    VoxelWorld() {

    }

    ~VoxelWorld() {

    }

    VoxelWorld(Vec3<u64> _size) :
    size(_size) {

    }

    u64 getChunkIndex(u64 x, u64 y, u64 z) const {
        return z + (x * size.z) + (y * size.x * size.z);
    }
};

#endif