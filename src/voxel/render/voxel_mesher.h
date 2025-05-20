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
    // voxel as binary for each x,y,z axis, positive and negative
    u64 axis_cols[3 * CS_P2 * 2] = {0};
    // opaque voxel as binary for each x,y,z axis, positive and negative
    u64 opaque_cols[3 * CS_P2 * 2] = {0};
    // merge voxel as binary for each x,y,z axis, positive and negative
    u64 merge_cols[3 * CS_P2 * 2] = {0};

    // the cull mask to perform greedy slicing, based on solids on previous axis_cols
    u64 col_face_masks[3 * CS_P2 * 2] = {0};

    // load voxels and block states with padding for quick lookup
    EmbeddedVoxel* blocks[CS_P3] = {nullptr};
    BlockStateStruct* blockStates[CS_P3] = {nullptr};



    auto addVoxelAxis = [&voxelBlockStateWorld, &axis_cols, &blocks, &blockStates, &merge_cols, &opaque_cols](EmbeddedVoxel* voxel, u8 x, u8 y, u8 z, i64 world_x, i64 world_y, i64 world_z) {
        BlockVoxelData blockData = BLOCK_VOXEL_DATA[voxel->type];

        BlockStateStruct* state = VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, world_x, world_y, world_z)->state;
        BlockMesh blockMesh = BLOCK_MESHES[blockData.meshType](*state);

        blocks[get_zxy_index_p(x, y, z)] = voxel;
        blockStates[get_zxy_index_p(x, y, z)] = state;

        axis_cols[z + (y * CS_P) + CS_P2 * 0] |= (u64)1 << x;
        axis_cols[z + (y * CS_P) + CS_P2 * 1] |= (u64)1 << x;
        axis_cols[x + (z * CS_P) + CS_P2 * 2] |= (u64)1 << y;
        axis_cols[x + (z * CS_P) + CS_P2 * 3] |= (u64)1 << y;
        axis_cols[x + (y * CS_P) + CS_P2 * 4] |= (u64)1 << z;
        axis_cols[x + (y * CS_P) + CS_P2 * 5] |= (u64)1 << z;

        // z,y - x axis
        if (blockMesh.culls(0)) {
            // positive
            merge_cols[z + (y * CS_P) + CS_P2 * 0] |= (u64)1 << x;
        }
        if (blockMesh.culls(1)) {
            // negative
            merge_cols[z + (y * CS_P) + CS_P2 * 1] |= (u64)1 << x;
        }

        // x,z - y axis
        if (blockMesh.culls(2)) {
            // positive
            merge_cols[x + (z * CS_P) + CS_P2 * 2] |= (u64)1 << y;
        }
        if (blockMesh.culls(3)) {
            // negative
            merge_cols[x + (z * CS_P) + CS_P2 * 3] |= (u64)1 << y;
        }

        // x,y - z axis
        if (blockMesh.culls(4)) {
            // positive
            merge_cols[x + (y * CS_P) + CS_P2 * 4] |= (u64)1 << z;
        }
        if (blockMesh.culls(5)) {
            // negative
            merge_cols[x + (y * CS_P) + CS_P2 * 5] |= (u64)1 << z;
        }

        if (blockData.transparent == false) {
            opaque_cols[z + (y * CS_P) + CS_P2 * 0] |= (u64)1 << x;
            opaque_cols[z + (y * CS_P) + CS_P2 * 1] |= (u64)1 << x;
            opaque_cols[x + (z * CS_P) + CS_P2 * 2] |= (u64)1 << y;
            opaque_cols[x + (z * CS_P) + CS_P2 * 3] |= (u64)1 << y;
            opaque_cols[x + (y * CS_P) + CS_P2 * 4] |= (u64)1 << z;
            opaque_cols[x + (y * CS_P) + CS_P2 * 5] |= (u64)1 << z;
        }
    };


    // build binary representation for every solid voxel y,x,z axis
    // voxels in this chunk
    for (u8 y = 0; y < CS; y++) {
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                u8 ax = x + 1;
                u8 ay = y + 1;
                u8 az = z + 1;

                i64 world_x = ax - 1 + chunk.pos.x * CS;
                i64 world_y = ay - 1 + chunk.pos.y * CS;
                i64 world_z = az - 1 + chunk.pos.z * CS;

                EmbeddedVoxel* voxel = &chunk.voxels[get_zxy_index(x, y, z)];
                if (voxel->type == BlockTypes::AIR) {
                    continue;
                }

                // TODO: check if voxel is solid (not see through)
                addVoxelAxis(voxel, ax, ay, az, world_x, world_y, world_z);
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

                    
                    // TODO: check if voxel is solid (not see through)
                    EmbeddedVoxel* voxel;
                    bool has_voxel = VoxelWorlds::getVoxel(voxelWorld, world_x, world_y, world_z, &voxel);

                    if (has_voxel == false) {
                        continue;
                    }
                    if (voxel->type == BlockTypes::AIR) {
                        continue;
                    }

                    
                    addVoxelAxis(voxel, x, y, z, world_x, world_y, world_z);
                }
            }
        }
    }




    
    // face culling
    for (u8 axis = 0; axis < 3; axis++) {
        for (u8 j = 0; j < 2; j++) {
            u8 dir = axis * 2 + j;
            for (u16 i = 0; i < CS_P2; i++) {
                u64 axis_col = axis_cols[(CS_P2 * dir) + i];
                u64 merge_col = merge_cols[(CS_P2 * dir) + i];
                u64 opaque_col = opaque_cols[(CS_P2 * dir) + i];

                u64 other_axis_col;
                u64 other_merge_col;
                u64 other_opaque_col;

                if (j == 0) {
                    // positive
                    other_axis_col = axis_col >> 1;
                    other_merge_col = merge_col >> 1;
                    other_opaque_col = opaque_col >> 1;
                } else {
                    // negative
                    other_axis_col = axis_col << 1;
                    other_merge_col = merge_col << 1;
                    other_opaque_col = opaque_col << 1;
                }

                // face is true if
                // merge meets air
                // non-merge meets air
                // non-merge meets non-merge
                u64 mask =  ( // axis_col & // do i need this?
                                (merge_col & ~other_axis_col) |    // merge meets air
                                (~merge_col & ~other_axis_col) |    // non-merge meets air
                                (~merge_col & ~other_merge_col)     // non-merge meets non-merge
                );

                col_face_masks[(CS_P2 * dir) + i] = mask;
            }
        }
    }








    // meshing
    
    // index
    u32 vertexIdx = 0;

    // generate quads
    for (u8 axis = 0; axis < 3; axis++) {
        for (u8 j = 0; j < 2; j++) {
            u8 dir = axis * 2 + j;
            for (u8 dim_2 = 0; dim_2 < CS; dim_2++) {
                for (u8 dim_1 = 0; dim_1 < CS; dim_1++) {
                    u32 index = (dim_1 + 1) + ((dim_2 + 1) * CS_P) + (CS_P2 * dir);
                    u64 face_mask = col_face_masks[index];

                    // remove padding right
                    face_mask >>= 1;
                    // remove padding left
                    face_mask &= ~(u64(1) << CS);

                    while (face_mask) {
                        u8 dim_3 = pop_lsb(face_mask);

                        u8 x, y, z;
                        if (axis == 0) {
                            z = dim_1;
                            y = dim_2;
                            x = dim_3;
                        } else if (axis == 1) {
                            x = dim_1;
                            z = dim_2;
                            y = dim_3;
                        } else {
                            x = dim_1;
                            y = dim_2;
                            z = dim_3;
                        }

                        i64 world_x = x + chunk.pos.x * CS;
                        i64 world_y = y + chunk.pos.y * CS;
                        i64 world_z = z + chunk.pos.z * CS;

                        // printf("(%i %i %i) (%i)\n", world_x, world_y, world_z, chunk.voxels[get_zxy_index(x, y, z)].type);
                        // try {
                        //     BlockStateStruct* state = VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, world_x, world_y, world_z)->state;
                        // } catch (std::runtime_error err) {
                        //     printf("(%i %i %i) (%i)\n", world_x, world_y, world_z, chunk.voxels[get_zxy_index(x, y, z)].type);
                        //     // throw std::runtime_error("wawa");
                        // }
                        // printf("2 %i\n", state);

                        Vec3<i64> world_center = (voxelBlockStateWorld.size / 2) * CS;

                        // printf("(%i %i %i) (%i)\n", world_x, world_y, world_z, chunk.voxels[get_zxy_index(x, y, z)].type);
                        BlockStateStruct* state = voxelBlockStateWorld.chunks[0].voxels[get_zxy_index(x, y, z)].state;
                        // printf("2 %i\n", state);



                        printf("(%i %i %i) (%i)\n", world_x, world_y, world_z, chunk.voxels[get_zxy_index(x, y, z)].type);
                        // BlockVoxelData blockData = BLOCK_VOXEL_DATA[chunk.voxels[get_zxy_index(x, y, z)].type];
                        
                        // BlockTexture blockTexture = blockData.get_texture(*state, dir);
                        // BlockMesh blockMesh = BLOCK_MESHES[blockData.meshType](*state);

                        // for (u8 i = 0; i < blockMesh.counts[dir]; i++) {
                        //     BlockFace face = blockMesh.faces[dir][i];
                        //     vertices[vertexIdx++] = VoxelFace(x, y, z, face.fromX, face.fromY, face.depth, face.width(), face.height(), face.uvFromX, face.uvFromY, face.uvWidth(), face.uvHeight(), face.uvRot, dir, blockTexture);
                        // }
                    }
                }
            }
        }
    }

    return vertexIdx;
}

#endif