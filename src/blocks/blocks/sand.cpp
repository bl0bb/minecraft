#include "sand.h"

void _sand_init() {
    BLOCK_VOXEL_DATA[BlockTypes::SAND] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        false,
        false,
        false,
        false,
        nullptr,
        _sand_get_texture
    );
}