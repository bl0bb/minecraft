#include "oak_stairs.h"

void _oak_stairs_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_STAIRS] = BlockVoxelData(
        BlockStateTypes::STAIR,
        BlockMeshTypes::STAIR,
        false,
        false,
        false,
        false,
        nullptr,
        _oak_stairs_get_texture
    );
}