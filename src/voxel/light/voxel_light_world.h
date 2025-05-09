#ifndef VOXEL_LIGHT_WORLD_H
#define VOXEL_LIGHT_WORLD_H

#include "../logic/voxel_world.h"

#include "voxel_light_chunk.h"

class VoxelLightWorld : public VoxelWorld<VoxelLightChunk> {
public:
    u8* heightmap;
};

#endif