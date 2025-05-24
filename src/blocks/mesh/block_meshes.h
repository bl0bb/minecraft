#ifndef BLOCK_MESHES_H
#define BLOCK_MESHES_H

#include <vector>

#include "../../core/types.h"
#include "../../core/maths.h"

#include "block_mesh.h"

// block meshes

// standard
#include "../block_meshes/block.h"
#include "../block_meshes/slab.h"
#include "../block_meshes/stair.h"

// log
#include "../block_meshes/log.h"

// advanced
#include "../block_meshes/torch.h"

// glass pane
#include "../block_meshes/glass_pane.h"

void loadBlocksMeshes() {
    // standard building blocks
    _loadBlockBlockMesh();
    _loadSlabBlockMesh();
    _loadStairBlockMesh();

    // log
    _loadLogBlockMesh();

    // advanced
    _loadTorchBlockMesh();

    // glass pane
    _loadGlassPaneBlockMesh();
}

#endif