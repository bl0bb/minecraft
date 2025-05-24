#ifndef BLOCK_H
#define BLOCK_H

// blocks
#include "blocks/air.h"
#include "blocks/cobblestone_slab.h"
#include "blocks/cobblestone_stairs.h"
#include "blocks/cobblestone.h"
#include "blocks/crafting_table.h"
#include "blocks/dirt.h"
#include "blocks/furnace.h"
#include "blocks/glass_pane.h"
#include "blocks/grass.h"
#include "blocks/oak_leaves.h"
#include "blocks/oak_log.h"
#include "blocks/oak_planks.h"
#include "blocks/oak_slab.h"
#include "blocks/oak_stairs.h"
#include "blocks/poppy.h"
#include "blocks/sand.h"
#include "blocks/snow.h"
#include "blocks/stone.h"
#include "blocks/stripped_oak_log.h"
#include "blocks/torch.h"
#include "blocks/water.h"

// other includes
#include "blocks.h"

void loadBlocks() {
    _air_init();
    _cobblestone_slab_init();
    _cobblestone_stairs_init();
    _cobblestone_init();
    _crafting_table_init();
    _dirt_init();
    _furnace_init();
    _glass_pane_init();
    _grass_init();
    _oak_leaves_init();
    _oak_log_init();
    _oak_planks_init();
    _oak_slab_init();
    _oak_stairs_init();
    _poppy_init();
    _sand_init();
    _snow_init();
    _stone_init();
    _stripped_oak_log_init();
    _torch_init();
    _water_init();
}

#endif