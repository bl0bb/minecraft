#include "crafting_table.h"

void _crafting_table_init() {
    BLOCK_VOXEL_DATA[BlockTypes::CRAFTING_TABLE] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _crafting_table_get_texture
    );
}