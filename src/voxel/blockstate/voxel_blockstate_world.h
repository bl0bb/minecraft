#ifndef VOXEL_BLOCKSTATE_WORLD_H
#define VOXEL_BLOCKSTATE_WORLD_H

#include "../logic/voxel_world.h"

#include "blockstate_voxel.h"

class BlockStateVoxelChunk : public BaseVoxelChunk<BlockStateVoxel> {
public:
    Vec3<i64> pos;
};

class VoxelBlockStateWorld : public VoxelWorld<BlockStateVoxelChunk> {
public:
    
};

#endif