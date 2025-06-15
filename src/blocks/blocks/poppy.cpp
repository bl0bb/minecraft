#include "poppy.h"

void _poppy_init() {
    BLOCK_VOXEL_DATA[BlockTypes::POPPY] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::CROSS,
        false,
        false,
        false,
        false,
        nullptr,
        _poppy_get_texture
    );
}