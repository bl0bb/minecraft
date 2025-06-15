#include "air.h"

void _air_init() {
    BLOCK_VOXEL_DATA[BlockTypes::AIR] = BlockVoxelData(
        BlockStateTypes::BLOCK,
        BlockMeshTypes::BLOCK,
        true,
        false,
        false,
        false,
        nullptr,
        _air_get_texture
    );
}

#endif