#ifndef VOXEL_BLOCKSTATE_WORLD_H
#define VOXEL_BLOCKSTATE_WORLD_H

#include "../logic/voxel_world.h"

#include "blockstate_voxel.h"

class BlockStateVoxelChunk : public BaseVoxelChunk<BlockStateVoxel> {
public:
    Vec3<i64> pos;

    BlockStateVoxelChunk() : BaseVoxelChunk() {
        for (u8 x = 0; x < CS; x++) {
            for (u8 y = 0; y < CS; y++) {
                for (u8 z = 0; z < CS; z++) {
                    voxels[get_zxy_index(x, y, z)] = new BlockStateStruct();
                }
            }
        }
    }
};

class VoxelBlockStateWorld : public VoxelWorld<BlockStateVoxelChunk> {
public:
    using VoxelWorld<BlockStateVoxelChunk>::VoxelWorld;
};

#endif