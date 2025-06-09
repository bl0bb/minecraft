#ifndef SLAB_BLOCK_MESH_H
#define SLAB_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getSlabBlockMesh(const BlockStateStruct& state) {
    auto placement = std::get<SlabBlockState>(state).placement;

    BlockMesh block(1);
    block.elements[0] = BlockElement(0, placement == 0 ? 0 : 8, 0,    16, placement == 0 ? 8 : 16, 16,    0, 0, 0,    0, 0, 0,    6);
    block.elements[0].faces[0] = BlockFace(0,    0,    0, placement == 0 ? 0 : 8, 16, placement == 0 ? 8 : 16,    0);
    block.elements[0].faces[1] = BlockFace(1,    0,    0, placement == 0 ? 0 : 8, 16, placement == 0 ? 8 : 16,    0);
    block.elements[0].faces[2] = BlockFace(2,    0,    0, 0, 16, 16,    0);
    block.elements[0].faces[3] = BlockFace(3,    0,    0, 0, 16, 16,    0);
    block.elements[0].faces[4] = BlockFace(4,    0,    0, placement == 0 ? 0 : 8, 16, placement == 0 ? 8 : 16,    0);
    block.elements[0].faces[5] = BlockFace(5,    0,    0, placement == 0 ? 0 : 8, 16, placement == 0 ? 8 : 16,    0);
    return block;
}

void _loadSlabBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::SLAB] = _getSlabBlockMesh;
}

#endif