#ifndef BLOCKS_H
#define BLOCKS_H

#include <vector>

#include "../core/types.h"
#include "../core/color.h"

#include "blockstate.h"

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
    COBBLESTONE_SLAB,
    COBBLESTONE_STAIRS,
    OAK_LOG,
    OAK_LEAVES,
    OAK_PLANKS,
    OAK_SLAB,
    OAK_STAIRS,
    CRAFTING_TABLE,
    FURNACE,
    WATER,
    POPPY,
};
}

typedef RGBI4 (*getLightFunc)();

struct BlockVoxelData {
    u8 stateType;
    u8 meshType;

    bool transparent;
    bool liquid;
    bool can_emit_light;
    bool animated;

    getLightFunc get_light;

    BlockTexture texture_top;
    BlockTexture texture_bottom;
    BlockTexture texture_right;
    BlockTexture texture_left;
    BlockTexture texture_back;
    BlockTexture texture_front;

    BlockVoxelData(
        u8 _stateType,
        u8 _meshType,

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
    stateType(_stateType),
    meshType(_meshType),

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
            return texture_right;
        } else if (dir == 1) {
            return texture_left;
        } else if (dir == 2) {
            return texture_top;
        } else if (dir == 3) {
            return texture_bottom;
        } else if (dir == 4) {
            return texture_back;
        } else {
            return texture_front;
        }
    }
};

BlockVoxelData* BLOCK_VOXEL_DATA = (BlockVoxelData*)malloc(sizeof(BlockVoxelData) * (BlockTypes::POPPY + 1));

#endif