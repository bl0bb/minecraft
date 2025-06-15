#include "furnace.h"

void _furnace_init() {
    BLOCK_VOXEL_DATA[BlockTypes::FURNACE] = BlockVoxelData(
        BlockStateTypes::BLOCK, // TODO: furnace
        BlockMeshTypes::BLOCK,
        false,
        false,
        true,
        false,
        getFurnaceLight,
        _furnace_get_texture
    );
}