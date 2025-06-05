#ifndef VOXEL_BLOCK_WORLD_H
#define VOXEL_BLOCK_WORLD_H

#include "../logic/voxel_world.h"

#include "embedded_voxel.h"

class VoxelBlockChunk : public BaseVoxelChunk<EmbeddedVoxel> {
public:
    Vec3<i64> pos;
};

class VoxelBlockWorld : public VoxelWorld<VoxelBlockChunk> {
public:
    using VoxelWorld<VoxelBlockChunk>::VoxelWorld;
};

#endif