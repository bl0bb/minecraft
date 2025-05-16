#ifndef BLOCK_MESHES_H
#define BLOCK_MESHES_H

#include <vector>

#include "../../core/types.h"
#include "../../core/maths.h"

#include "block_mesh.h"

// block meshes
#include "../block_meshes/block.h"
#include "../block_meshes/slab.h"
#include "../block_meshes/stair.h"

void loadBlocksMeshes() {
    _loadBlockBlockMesh();
    _loadSlabBlockMesh();
    _loadStairBlockMesh();
}

#endif