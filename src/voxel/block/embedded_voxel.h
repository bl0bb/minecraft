#ifndef EMBEDDED_VOXEL_H
#define EMBEDDED_VOXEL_H

#include "../../core/maths.h"
#include "../../core/types.h"

#include "../../blocks/blocks.h"

struct EmbeddedVoxel {
    BlockType type;

    EmbeddedVoxel() : type(BlockTypes::AIR) {

    }

    EmbeddedVoxel(BlockType _type) : type(_type) {

    }
};

#endif