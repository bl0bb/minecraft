#include "grass.h"

void _grass_init() {
    BLOCK_VOXEL_DATA[BlockTypes::GRASS] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _grass_get_texture
    );
}