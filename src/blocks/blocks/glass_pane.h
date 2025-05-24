#ifndef _BLOCK_GLASS_PANE_H
#define _BLOCK_GLASS_PANE_H

#include "../blocks.h"

constexpr inline BlockTexture _glass_pane_get_texture(const BlockStateStruct& state, u8 dir) {
    auto conns = std::get<GlassPaneBlockState>(state).connections;

    if (dir == 2 || dir == 3) {
        return BlockTextures::GLASS_PANE_SIDE;
    }
    
    // if ((conns >> (dir > 1 ? dir - 2 : dir)) & 1) {
    //     return BlockTextures::GLASS_PANE_SIDE;
    // }

    return BlockTextures::GLASS;
}

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

#endif