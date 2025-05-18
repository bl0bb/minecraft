#ifndef TORCH_BLOCK_MESH_H
#define TORCH_BLOCK_MESH_H

#include "../mesh/block_mesh.h"
#include "../blockstate.h"

static BlockMesh _getTorchBlockMesh(const BlockStateStruct& state) {
    auto direction = std::get<TorchBlockState>(state).direction;

    BlockMesh torch(1, 1, 1, 1, 1, 1, false, false, false, false, false, false);
    torch.faces[0][0] = BlockFace(0,    7,    7, 0, 8, 9,    7, 0, 8, 9);
    torch.faces[1][0] = BlockFace(0,    7,    7, 0, 8, 9,    7, 0, 8, 9);
    torch.faces[2][0] = BlockFace(0,    6,    7, 7, 8, 8,    7, 8, 8, 9);
    torch.faces[3][0] = BlockFace(0,    0,    7, 7, 8, 8,    7, 0, 8, 1);
    torch.faces[4][0] = BlockFace(0,    7,    7, 0, 8, 9,    7, 0, 8, 9);
    torch.faces[5][0] = BlockFace(0,    7,    7, 0, 8, 9,    7, 0, 8, 9);
    return torch;
}

void _loadTorchBlockMesh() {
    BLOCK_MESHES[BlockMeshTypes::TORCH] = _getTorchBlockMesh;
}

#endif