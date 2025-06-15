#include "oak_log.h"

void _oak_log_init() {
    BLOCK_VOXEL_DATA[BlockTypes::OAK_LOG] = BlockVoxelData(
        BlockStateTypes::LOG,
        BlockMeshTypes::LOG,
        false,
        false,
        false,
        false,
        nullptr,
        _oak_log_get_texture
    );
}