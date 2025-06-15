#include "oak_planks.h"

void _oak_planks_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_PLANKS] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _oak_planks_get_texture
    );
}