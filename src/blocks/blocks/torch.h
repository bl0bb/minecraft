#ifndef _BLOCK_TORCH_H
#define _BLOCK_TORCH_H

#include "../blocks.h"

constexpr inline BlockTexture _torch_get_texture(const BlockStateStruct& state, u8 dir) {
    (void)state;
    (void)dir;
    return BlockTextures::TORCH;
}

constexpr inline RGBI4 getTorchLight() {
    return Colors::createRGBI4(15, 15, 15, 15);
}

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

#endif