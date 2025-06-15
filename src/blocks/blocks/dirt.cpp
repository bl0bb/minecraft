#include "dirt.h"

void _dirt_init() {
    BLOCK_VOXEL_DATA[BlockTypes::DIRT] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _dirt_get_texture
    );
}