#include "water.h"

void _water_init() {
    BLOCK_VOXEL_DATA[BlockTypes::WATER] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _water_get_texture
    );
}