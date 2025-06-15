#include "short_grass.h"

void _short_grass_init() {
    BLOCK_VOXEL_DATA[BlockTypes::SHORT_GRASS] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::CROSS,
        false,
        false,
        false,
        false,
        nullptr,
        _short_grass_get_texture
    );
}