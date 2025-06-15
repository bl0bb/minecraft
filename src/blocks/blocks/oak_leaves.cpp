#include "oak_leaves.h"

void _oak_leaves_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_LEAVES] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::LEAVES,
        true,
        false,
        false,
        false,
        nullptr,
        _oak_leaves_get_texture
    );
}