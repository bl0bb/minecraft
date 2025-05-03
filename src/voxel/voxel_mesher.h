#ifndef VOXEL_MESHER_H
#define VOXEL_MESHER_H

#include <vector>

#include "../core/types.h"
#include "../core/bits.h"

#include "../blocks.h"

#include "voxel.h"

#include "voxel_world.h"



u32 generate_voxel_mesh(const VoxelWorld& voxelWorld, const VoxelChunk& chunk, VoxelFace* vertices);

#endif