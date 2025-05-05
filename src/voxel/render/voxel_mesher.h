#ifndef VOXEL_MESHER_H
#define VOXEL_MESHER_H

#include <vector>

#include "../../core/types.h"
#include "../../core/bits.h"

#include "../../blocks.h"


#include "../logic/voxel.h"
#include "../logic/voxel_game_world.h"

#include "voxel_chunk_mesh_common.h"

u32 generate_voxel_mesh(const VoxelGameWorld& voxelWorld, const VoxelChunk& chunk, VoxelFace* vertices);

#endif