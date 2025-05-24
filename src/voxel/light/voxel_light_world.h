#ifndef VOXEL_LIGHT_WORLD_H
#define VOXEL_LIGHT_WORLD_H

#include "../logic/voxel_world.h"

class VoxelLightChunk : public BaseVoxelChunk<RGBIS4> {
public:

};

class VoxelLightWorld : public VoxelWorld<VoxelLightChunk> {
public:

};

#endif