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
    "stripped_oak_log_top",
    "stripped_oak_log",
    "crafting_table_top",
    "crafting_table_front",
    "crafting_table_side",
    "furnace_top",
    "furnace_side",
    "furnace_front",
    "water",
    "poppy",
    "torch",
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
    STRIPPED_OAK_LOG_TOP,
    STRIPPED_OAK_LOG,
    CRAFTING_TABLE_TOP,
    CRAFTING_TABLE_FRONT,
    CRAFTING_TABLE_SIDE,
    FURNACE_TOP,
    FURNACE_SIDE,
    FURNACE_FRONT,
    WATER,
    POPPY,
    TORCH,
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
    STRIPPED_OAK_LOG,
    CRAFTING_TABLE,
    FURNACE,
    WATER,
    POPPY,
    TORCH,
};
}

typedef RGBI4 (*getLightFunc)();
typedef BlockTexture (*getTextureFunc)(const BlockStateStruct& state, u8 dir);

struct BlockVoxelData {
    u8 stateType;
    u8 meshType;

    bool transparent;
    bool liquid;
    bool can_emit_light;
    bool animated;

    getLightFunc get_light;

    getTextureFunc get_texture;

    BlockVoxelData(
        u8 _stateType,
        u8 _meshType,

        bool _transparent,
        bool _liquid,
        bool _can_emit_light,
        bool _animated,

        getLightFunc _get_light,

        getTextureFunc _get_texture
        ) :
    stateType(_stateType),
    meshType(_meshType),

    transparent(_transparent),
    liquid(_liquid),
    can_emit_light(_can_emit_light),
    animated(_animated),

    get_light(_get_light),

    get_texture(_get_texture)
    {}
};

BlockVoxelData* BLOCK_VOXEL_DATA = (BlockVoxelData*)malloc(sizeof(BlockVoxelData) * (BlockTypes::TORCH + 1));

#endif