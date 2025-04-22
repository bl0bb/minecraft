#ifndef VOXEL_CHUNK_H
#define VOXEL_CHUNK_H

#include "../core/types.h"
#include "voxel.h"

struct VoxelChunk {
    Vec2<u64> pos;
    Voxel* voxels;
    
};

#endif