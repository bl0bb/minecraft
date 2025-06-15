#include "glass_pane.h"

void _glass_pane_init() {
    BLOCK_VOXEL_DATA[BlockTypes::GLASS_PANE] = BlockVoxelData(
        BlockStateTypes::GLASS_PANE,
        BlockMeshTypes::GLASS_PANE,
        true,
        false,
        false,
        false,
        nullptr,
        _glass_pane_get_texture
    );
}