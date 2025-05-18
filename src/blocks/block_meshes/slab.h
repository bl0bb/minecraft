#ifndef SLAB_BLOCK_MESH_H
#define SLAB_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getSlabBlockMesh(const BlockStateStruct& state) {
    auto placement = std::get<SlabBlockState>(state).placement;

    BlockMesh slab(1, 1, 1, 1, 1, 1, false, false, false, false, false, false);
    slab.faces[0][0] = BlockFace(0,    0,                         0, placement == 0 ? 0 : 8, 16 - 1, (placement == 0 ? 8 : 16) - 1,    0, placement == 0 ? 0 : 8, 16 - 1, (placement == 0 ? 8 : 16) - 1);
    slab.faces[1][0] = BlockFace(0,    0,                         0, placement == 0 ? 0 : 8, 16 - 1, (placement == 0 ? 8 : 16) - 1,    0, placement == 0 ? 0 : 8, 16 - 1, (placement == 0 ? 8 : 16) - 1);
    slab.faces[2][0] = BlockFace(0,    placement == 0 ? 8 : 0,    0,                      0, 16 - 1,                        16 - 1,    0,                      0, 16 - 1,                        16 - 1);
    slab.faces[3][0] = BlockFace(0,    placement == 0 ? 0 : 8,    0,                      0, 16 - 1,                        16 - 1,    0,                      0, 16 - 1,                        16 - 1);
    slab.faces[4][0] = BlockFace(0,    0,                         0, placement == 0 ? 0 : 8, 16 - 1, (placement == 0 ? 8 : 16) - 1,    0, placement == 0 ? 0 : 8, 16 - 1, (placement == 0 ? 8 : 16) - 1);
    slab.faces[5][0] = BlockFace(0,    0,                         0, placement == 0 ? 0 : 8, 16 - 1, (placement == 0 ? 8 : 16) - 1,    0, placement == 0 ? 0 : 8, 16 - 1, (placement == 0 ? 8 : 16) - 1);
    return slab;
}

void _loadSlabBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::SLAB] = _getSlabBlockMesh;
}

#endif