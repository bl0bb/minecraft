#ifndef BLOCKS_H
#define BLOCKS_H

#include <vector>

#include "core/types.h"

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

// 0-9 top
// 10-19 bottom
// 20-29 right
// 30-39 left
// 40-49 back
// 50-59 front
typedef u64 BlockVoxelData;

namespace BlockVoxelDatas {
    const inline BlockVoxelData create(BlockTexture top, BlockTexture bottom, BlockTexture right, BlockTexture left, BlockTexture back, BlockTexture front) {
        // bitshift 1 for now to prevent the code from thinking all bits being 0 is an empty block
        return ((BlockVoxelData)1 << 63) |
            ((BlockVoxelData)front << 50) |
            ((BlockVoxelData)back << 40) |
            ((BlockVoxelData)left << 30) |
            ((BlockVoxelData)right << 20) |
            ((BlockVoxelData)bottom << 10) |
            (BlockVoxelData)top;
    }
    const inline u16 get_face(BlockVoxelData block, u8 face) {
        return (block >> (face * 10)) & 0b1111111111;
    }
};

const BlockVoxelData block_voxel_datas[] = {
    BlockVoxelDatas::create(BlockTextures::NONE, BlockTextures::NONE, BlockTextures::NONE, BlockTextures::NONE, BlockTextures::NONE, BlockTextures::NONE),
    BlockVoxelDatas::create(BlockTextures::GRASS_BLOCK_TOP, BlockTextures::DIRT, BlockTextures::GRASS_BLOCK_SIDE, BlockTextures::GRASS_BLOCK_SIDE, BlockTextures::GRASS_BLOCK_SIDE, BlockTextures::GRASS_BLOCK_SIDE),
    BlockVoxelDatas::create(BlockTextures::DIRT, BlockTextures::DIRT, BlockTextures::DIRT, BlockTextures::DIRT, BlockTextures::DIRT, BlockTextures::DIRT),
    BlockVoxelDatas::create(BlockTextures::SAND, BlockTextures::SAND, BlockTextures::SAND, BlockTextures::SAND, BlockTextures::SAND, BlockTextures::SAND),
    BlockVoxelDatas::create(BlockTextures::SNOW, BlockTextures::SNOW, BlockTextures::SNOW, BlockTextures::SNOW, BlockTextures::SNOW, BlockTextures::SNOW),
    BlockVoxelDatas::create(BlockTextures::STONE, BlockTextures::STONE, BlockTextures::STONE, BlockTextures::STONE, BlockTextures::STONE, BlockTextures::STONE),
    BlockVoxelDatas::create(BlockTextures::COBBLESTONE, BlockTextures::COBBLESTONE, BlockTextures::COBBLESTONE, BlockTextures::COBBLESTONE, BlockTextures::COBBLESTONE, BlockTextures::COBBLESTONE),
    BlockVoxelDatas::create(BlockTextures::OAK_LOG_TOP, BlockTextures::OAK_LOG_TOP, BlockTextures::OAK_LOG, BlockTextures::OAK_LOG, BlockTextures::OAK_LOG, BlockTextures::OAK_LOG),
    BlockVoxelDatas::create(BlockTextures::OAK_LEAVES, BlockTextures::OAK_LEAVES, BlockTextures::OAK_LEAVES, BlockTextures::OAK_LEAVES, BlockTextures::OAK_LEAVES, BlockTextures::OAK_LEAVES),
    BlockVoxelDatas::create(BlockTextures::OAK_PLANKS, BlockTextures::OAK_PLANKS, BlockTextures::OAK_PLANKS, BlockTextures::OAK_PLANKS, BlockTextures::OAK_PLANKS, BlockTextures::OAK_PLANKS),
    BlockVoxelDatas::create(BlockTextures::CRAFTING_TABLE_TOP, BlockTextures::OAK_PLANKS, BlockTextures::CRAFTING_TABLE_SIDE, BlockTextures::CRAFTING_TABLE_SIDE, BlockTextures::CRAFTING_TABLE_FRONT, BlockTextures::CRAFTING_TABLE_FRONT),
    BlockVoxelDatas::create(BlockTextures::FURNACE_TOP, BlockTextures::FURNACE_TOP, BlockTextures::FURNACE_SIDE, BlockTextures::FURNACE_SIDE, BlockTextures::FURNACE_FRONT, BlockTextures::FURNACE_FRONT),
    BlockVoxelDatas::create(BlockTextures::WATER, BlockTextures::WATER, BlockTextures::WATER, BlockTextures::WATER, BlockTextures::WATER, BlockTextures::WATER),
    BlockVoxelDatas::create(BlockTextures::POPPY, BlockTextures::POPPY, BlockTextures::POPPY, BlockTextures::POPPY, BlockTextures::POPPY, BlockTextures::POPPY),
};

#endif