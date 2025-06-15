#include "stone.h"

void _stone_init() {
    BLOCK_VOXEL_DATA[BlockTypes::STONE] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _stone_get_texture
    );
}