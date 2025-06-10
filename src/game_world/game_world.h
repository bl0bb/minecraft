#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "../voxel/block/voxel_block_world.h"
#include "../voxel/blockstate/voxel_blockstate_world.h"
#include "../voxel/light/voxel_light_world.h"
#include "../voxel/height/voxel_height_world.h"
#include "../voxel/render/voxel_world_renderer.h"

class GameWorld {
public:
    VoxelBlockWorld blockWorld;
    VoxelBlockStateWorld blockStateWorld;
    VoxelLightWorld lightWorld;
    VoxelHeightWorld heightWorld;
    VoxelWorldRenderer worldRenderer;

    GameWorld() {
        blockWorld = VoxelBlockWorld(world_size);
        blockStateWorld = VoxelBlockStateWorld(world_size);
        lightWorld = VoxelLightWorld(world_size);
        heightWorld = VoxelHeightWorld(height_size);
        worldRenderer = VoxelWorldRenderer(world_size);
    }
};

#endif