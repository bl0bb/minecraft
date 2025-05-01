#ifndef VOXEL_WORLD_H
#define VOXEL_WORLD_H

#include <glad/glad.h>

#include <vector>

#include "voxel_chunk.h"

class VoxelWorld {
public:
    std::vector<VoxelChunk> chunks;
};

#endif