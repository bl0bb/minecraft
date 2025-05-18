#ifndef _BLOCK_OAK_LOG_H
#define _BLOCK_OAK_LOG_H

#include "../blocks.h"

constexpr inline BlockTexture _oak_log_get_texture(const BlockStateStruct& state, u8 dir) {
    auto logDir = std::get<LogBlockState>(state).direction;

    if (dir & 0b100) {
        return logDir == 2 ? BlockTextures::OAK_LOG_TOP : BlockTextures::OAK_LOG;
    } else if (dir & 0b10) {
        return logDir == 1 ? BlockTextures::OAK_LOG_TOP : BlockTextures::OAK_LOG;
    } else {
        return logDir == 0 ? BlockTextures::OAK_LOG_TOP : BlockTextures::OAK_LOG;
    }
}

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

#endif