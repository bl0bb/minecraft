#include "cobblestone_stairs.h"

void _cobblestone_stairs_init() {
    BLOCK_VOXEL_DATA[BlockTypes::COBBLESTONE_STAIRS] = BlockVoxelData(
        BlockStateTypes::STAIR,
        BlockMeshTypes::STAIR,
        false,
        false,
        false,
        false,
        nullptr,
        _cobblestone_stairs_get_texture
    );
}