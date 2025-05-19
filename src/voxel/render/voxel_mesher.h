#ifndef VOXEL_MESHER_H
#define VOXEL_MESHER_H

#include <vector>
#include <unordered_map>

#include "../../core/types.h"
#include "../../core/bits.h"

#include "../../blocks/blocks.h"
#include "../../blocks/mesh/block_mesh.h"

// TODO: remove this and generate mesh at runtime
#include "../../blocks/mesh/block_meshes.h"


#include "../block/voxel_block_world.h"

#include "voxel_chunk_mesh_common.h"

void printBinary(unsigned int num, u8 count) {
    // Adjust for 32-bit integers
    for (int i = count - 1; i >= 0; i--) {
        unsigned int mask = 1 << i;
        putchar((num & mask) ? '1' : '0');
        putchar(' ');

        // Optional: Add a space every 4 bits for readability
        // if (i % 4 == 0) {
        //     putchar(' ');
        // }
    }
}




constexpr inline void dim_to_pos(u8& x, u8& y, u8& z, u8 dim_1, u8 dim_2, u8 dim_3, u8 axis) {
    if (axis == 0) {
        x = dim_1;
        y = dim_2;
        z = dim_3;
    } else if (axis == 1) {
        z = dim_1;
        x = dim_2;
        y = dim_3;
    } else {
        x = dim_1;
        z = dim_2;
        y = dim_3;
    }
}


u32 generate_voxel_mesh(const VoxelBlockWorld& voxelWorld, const VoxelBlockStateWorld& voxelBlockStateWorld, const VoxelChunk& chunk, VoxelFace* vertices) {
    // load voxels and block states with padding for quick lookup
    EmbeddedVoxel* blocks[CS_P3] = {nullptr};
    BlockStateStruct* blockStates[CS_P3] = {nullptr};


    auto addVoxelAxis = [&voxelBlockStateWorld, &voxelWorld, &blocks, &blockStates](u8 x, u8 y, u8 z, i64 world_x, i64 world_y, i64 world_z) {
        EmbeddedVoxel* voxel;

        bool has_voxel = VoxelWorlds::getVoxel(voxelWorld, world_x, world_y, world_z, &voxel);
        if (has_voxel == false) {
            return;
        }
        
        if (voxel->type == BlockTypes::AIR) {
            return;
        }

        BlockVoxelData blockData = BLOCK_VOXEL_DATA[voxel->type];

        BlockStateStruct* state = VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, world_x, world_y, world_z)->state;

        blocks[get_zxy_index_p(x, y, z)] = voxel;
        blockStates[get_zxy_index_p(x, y, z)] = state;
    };


    // build binary representation for every solid voxel y,x,z axis
    // voxels in this chunk
    for (u8 y = 0; y < CS; y++) {
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                u8 ax = x + 1;
                u8 ay = y + 1;
                u8 az = z + 1;

                i64 world_x = x + chunk.pos.x * CS;
                i64 world_y = y + chunk.pos.y * CS;
                i64 world_z = z + chunk.pos.z * CS;

                addVoxelAxis(ax, ay, az, world_x, world_y, world_z);
            }
        }
    }

    // voxels in neighboring chunks
    for (u8 axis = 0; axis < 3; axis++) {
        for (u8 dim_1 = 0; dim_1 < CS_P; dim_1++) {
            for (u8 dim_2 = 0; dim_2 < CS_P; dim_2++) {
                for (u8 dim_3 = 0; dim_3 < CS_P; dim_3 += CS_P - 1) {
                    u8 x, y, z;
                    if (axis == 0) {
                        z = dim_1;
                        x = dim_3;
                        y = dim_2;
                    } else if (axis == 1) {
                        x = dim_1;
                        y = dim_3;
                        z = dim_2;
                    } else {
                        x = dim_1;
                        z = dim_3;
                        y = dim_2;
                    }

                    i64 world_x = x - 1 + chunk.pos.x * CS;
                    i64 world_y = y - 1 + chunk.pos.y * CS;
                    i64 world_z = z - 1 + chunk.pos.z * CS;
                    
                    addVoxelAxis(x, y, z, world_x, world_y, world_z);
                }
            }
        }
    }






    // meshing
    
    // index
    u32 vertexIdx = 0;

    // generate quads new
    for (u8 x = 1; x < CS_P - 1; x++) {
        for (u8 y = 1; y < CS_P - 1; y++) {
            for (u8 z = 1; z < CS_P - 1; z++) {
                EmbeddedVoxel* voxel = blocks[get_zxy_index_p(x, y, z)];
                if (voxel == nullptr) {
                    continue;
                }
                BlockVoxelData blockData = BLOCK_VOXEL_DATA[voxel->type];

                BlockStateStruct* state = blockStates[get_zxy_index_p(x, y, z)];

                for (u8 dir = 0; dir < 6; dir++) {
                    u8 newX = x;
                    u8 newY = y;
                    u8 newZ = z;
                    if (dir == 0) newX++;
                    else if (dir == 1) newX--;
                    else if (dir == 2) newY++;
                    else if (dir == 3) newY--;
                    else if (dir == 4) newZ++;
                    else newZ--;

                    BlockMesh blockMesh = BLOCK_MESHES[blockData.meshType](*state);

                    auto addFace = [blocks, blockStates, vertices, &vertexIdx, state, &blockMesh, &blockData, dir, x, y, z]() {
                        BlockTexture blockTexture = blockData.get_texture(*state, dir);

                        for (u8 i = 0; i < blockMesh.counts[dir]; i++) {
                            BlockFace face = blockMesh.faces[dir][i];
                            vertices[vertexIdx++] = VoxelFace(x - 1, y - 1, z - 1, face.fromX, face.fromY, face.depth, face.width(), face.height(), face.uvFromX, face.uvFromY, face.uvWidth(), face.uvHeight(), face.uvRot, dir, blockTexture);
                        }
                    };

                    EmbeddedVoxel* otherVoxel = blocks[get_zxy_index_p(newX, newY, newZ)];
                    if (otherVoxel == nullptr) {
                        // no other voxel, add face
                        addFace();
                        continue;
                    }
                    
                    BlockVoxelData otherBlockData = BLOCK_VOXEL_DATA[otherVoxel->type];
                    BlockStateStruct* otherState = blockStates[get_zxy_index_p(newX, newY, newZ)];
                    BlockMesh otherBlockMesh = BLOCK_MESHES[otherBlockData.meshType](*otherState);


                    // check whether or not to create a face based on other voxel

                    // if other block culls (hides full face) and is not transparent
                    // then we cant see this face
                    // so continue
                    if (otherBlockMesh.culls(dir) && otherBlockData.transparent == false) {
                        continue;
                    }

                    addFace();
                }
            }
        }
    }

    return vertexIdx;
}

#endif