#include "snow.h"

void _snow_init() {
    BLOCK_VOXEL_DATA[BlockTypes::SNOW] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _snow_get_texture
    );
}