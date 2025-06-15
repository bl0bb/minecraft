#include "cobblestone_slab.h"

void _cobblestone_slab_init() {
    BLOCK_VOXEL_DATA[BlockTypes::COBBLESTONE_SLAB] = BlockVoxelData(
        BlockStateTypes::SLAB,
        BlockMeshTypes::SLAB,
        false,
        false,
        false,
        false,
        nullptr,
        _cobblestone_slab_get_texture
    );
}