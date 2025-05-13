#ifndef VOXEL_BLOCK_WORLD_H
#define VOXEL_BLOCK_WORLD_H

#include "../logic/voxel_world.h"

#include "embedded_voxel.h"

class VoxelChunk : public BaseVoxelChunk<EmbeddedVoxel> {
public:
    Vec3<i64> pos;
};

class VoxelBlockWorld : public VoxelWorld<VoxelChunk> {
public:
    
};

#endif