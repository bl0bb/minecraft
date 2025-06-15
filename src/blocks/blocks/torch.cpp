#include "torch.h"

void _torch_init() {
    BLOCK_VOXEL_DATA[BlockTypes::TORCH] = BlockVoxelData(
        BlockStateTypes::TORCH,
        BlockMeshTypes::TORCH,
        false,
        false,
        true,
        false,
        getTorchLight,
        _torch_get_texture
    );
}