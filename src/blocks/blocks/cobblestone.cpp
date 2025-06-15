#include "cobblestone.h"

void _cobblestone_init() {
    BLOCK_VOXEL_DATA[BlockTypes::COBBLESTONE] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _cobblestone_get_texture
    );
}