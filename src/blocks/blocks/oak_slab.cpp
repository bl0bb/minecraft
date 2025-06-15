#include "oak_slab.h"

void _oak_slab_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_SLAB] = BlockVoxelData(
        BlockStateTypes::SLAB,
        BlockMeshTypes::SLAB,
        false,
        false,
        false,
        false,
        nullptr,
        _oak_slab_get_texture
    );
}