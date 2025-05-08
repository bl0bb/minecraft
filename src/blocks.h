#ifndef BLOCKS_H
#define BLOCKS_H

#include <vector>

#include "core/types.h"
#include "core/color.h"

constexpr const char* block_textures[] = {
    "",
    "grass_block_top",
    "grass_block_side",
    "dirt",
    "sand",
    "snow",
    "stone",
    "cobblestone",
    "oak_log_top",
    "oak_log",
    "oak_leaves",
    "oak_planks",
    "crafting_table_top",
    "crafting_table_front",
    "crafting_table_side",
    "furnace_top",
    "furnace_side",
    "furnace_front",
    "water",
    "poppy",
};

typedef u16 BlockTexture;
namespace BlockTextures {
enum BlockTextureEnum : BlockTexture {
    NONE,
    GRASS_BLOCK_TOP,
    GRASS_BLOCK_SIDE,
    DIRT,
    SAND,
    SNOW,
    STONE,
    COBBLESTONE,
    OAK_LOG_TOP,
    OAK_LOG,
    OAK_LEAVES,
    OAK_PLANKS,
    CRAFTING_TABLE_TOP,
    CRAFTING_TABLE_FRONT,
    CRAFTING_TABLE_SIDE,
    FURNACE_TOP,
    FURNACE_SIDE,
    FURNACE_FRONT,
    WATER,
    POPPY,
};
}

typedef u16 BlockType;
namespace BlockTypes {
enum BlockTypeEnum : BlockType {
    AIR,
    GRASS,
    DIRT,
    SAND,
    SNOW,
    STONE,
    COBBLESTONE,
    OAK_LOG,
    OAK_LEAVES,
    OAK_PLANKS,
    CRAFTING_TABLE,
    FURNACE,
    WATER,
    POPPY,
};
}

typedef RGBI4 getLightFunc();

struct BlockVoxelData {
    bool transparent;
    bool liquid;
    bool can_emit_light;
    bool animated;

    getLightFunc* get_light;

    BlockTexture texture_top;
    BlockTexture texture_bottom;
    BlockTexture texture_right;
    BlockTexture texture_left;
    BlockTexture texture_back;
    BlockTexture texture_front;

    BlockVoxelData(
        bool _transparent,
        bool _liquid,
        bool _can_emit_light,
        bool _animated,

        getLightFunc _get_light,

        BlockTexture _texture_top,
        BlockTexture _texture_bottom,
        BlockTexture _texture_right,
        BlockTexture _texture_left,
        BlockTexture _texture_back,
        BlockTexture _texture_front
        ) :
    transparent(_transparent),
    liquid(_liquid),
    can_emit_light(_can_emit_light),
    animated(_animated),

    get_light(_get_light),

    texture_top(_texture_top),
    texture_bottom(_texture_bottom),
    texture_right(_texture_right),
    texture_left(_texture_left),
    texture_back(_texture_back),
    texture_front(_texture_front) {
        
    }

    BlockTexture get_face(u8 dir) const {
        if (dir == 0) {
            return texture_top;
        } else if (dir == 1) {
            return texture_bottom;
        } else if (dir == 2) {
            return texture_right;
        } else if (dir == 3) {
            return texture_left;
        } else if (dir == 4) {
            return texture_back;
        } else {
            return texture_front;
        }
    }
};

RGBI4 getLightTest() {
    return Colors::createRGBI4(15, 0, 0, 15);
}

const BlockVoxelData block_voxel_datas[] = {
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::NONE,                  BlockTextures::NONE,           BlockTextures::NONE,                   BlockTextures::NONE,                   BlockTextures::NONE,                    BlockTextures::NONE),
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::GRASS_BLOCK_TOP,       BlockTextures::DIRT,           BlockTextures::GRASS_BLOCK_SIDE,       BlockTextures::GRASS_BLOCK_SIDE,       BlockTextures::GRASS_BLOCK_SIDE,        BlockTextures::GRASS_BLOCK_SIDE),
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::SAND,                  BlockTextures::SAND,           BlockTextures::SAND,                   BlockTextures::SAND,                   BlockTextures::SAND,                    BlockTextures::SAND),
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::DIRT,                  BlockTextures::DIRT,           BlockTextures::DIRT,                   BlockTextures::DIRT,                   BlockTextures::DIRT,                    BlockTextures::DIRT),
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::SNOW,                  BlockTextures::SNOW,           BlockTextures::SNOW,                   BlockTextures::SNOW,                   BlockTextures::SNOW,                    BlockTextures::SNOW),
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::STONE,                 BlockTextures::STONE,          BlockTextures::STONE,                  BlockTextures::STONE,                  BlockTextures::STONE,                   BlockTextures::STONE),
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::COBBLESTONE,           BlockTextures::COBBLESTONE,    BlockTextures::COBBLESTONE,            BlockTextures::COBBLESTONE,            BlockTextures::COBBLESTONE,             BlockTextures::COBBLESTONE),
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::OAK_LOG_TOP,           BlockTextures::OAK_LOG_TOP,    BlockTextures::OAK_LOG,                BlockTextures::OAK_LOG,                BlockTextures::OAK_LOG,                 BlockTextures::OAK_LOG),
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::OAK_LEAVES,            BlockTextures::OAK_LEAVES,     BlockTextures::OAK_LEAVES,             BlockTextures::OAK_LEAVES,             BlockTextures::OAK_LEAVES,              BlockTextures::OAK_LEAVES),
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::OAK_PLANKS,            BlockTextures::OAK_PLANKS,     BlockTextures::OAK_PLANKS,             BlockTextures::OAK_PLANKS,             BlockTextures::OAK_PLANKS,              BlockTextures::OAK_PLANKS),
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::CRAFTING_TABLE_TOP,    BlockTextures::OAK_PLANKS,     BlockTextures::CRAFTING_TABLE_SIDE,    BlockTextures::CRAFTING_TABLE_SIDE,    BlockTextures::CRAFTING_TABLE_FRONT,    BlockTextures::CRAFTING_TABLE_FRONT),
    BlockVoxelData(false,    false,    true,     false,    getLightTest,    BlockTextures::FURNACE_TOP,           BlockTextures::FURNACE_TOP,    BlockTextures::FURNACE_SIDE,           BlockTextures::FURNACE_SIDE,           BlockTextures::FURNACE_FRONT,           BlockTextures::FURNACE_FRONT),
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::WATER,                 BlockTextures::WATER,          BlockTextures::WATER,                  BlockTextures::WATER,                  BlockTextures::WATER,                   BlockTextures::WATER),
    BlockVoxelData(false,    false,    false,    false,    nullptr,         BlockTextures::POPPY,                 BlockTextures::POPPY,          BlockTextures::POPPY,                  BlockTextures::POPPY,                  BlockTextures::POPPY,                   BlockTextures::POPPY),
};

#endif