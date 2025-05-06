#ifndef VOXEL_LIGHT_CHUNK_H
#define VOXEL_LIGHT_CHUNK_H

#include "../../core/types.h"
#include "../../core/color.h"
#include "../../core/shader.h"

#include "../logic/voxel_world.h"
#include "../logic/voxel_chunk.h"
#include "../logic/voxel.h"

class VoxelLightChunk {
public:
    VoxelChunk* chunk;

    RGBA4* voxels;

    VoxelLightChunk() {
        voxels = new RGBA4[CS_3];
        std::memset(voxels, 0, CS_3);
    }

    ~VoxelLightChunk() {
        free(voxels);
    }
};

#endif