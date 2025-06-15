#include "stripped_oak_log.h"

void _stripped_oak_log_init() {
    BLOCK_VOXEL_DATA[BlockTypes::STRIPPED_OAK_LOG] = BlockVoxelData(
        BlockStateTypes::LOG,
        BlockMeshTypes::LOG,
        false,
        false,
        false,
        false,
        nullptr,
        _stripped_oak_log_get_texture
    );
}