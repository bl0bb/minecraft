#include <unordered_map>

#include "voxel_mesher.h"










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


u32 generate_voxel_mesh(const VoxelGameWorld& voxelWorld, const VoxelChunk& chunk, VoxelFace* vertices) {
    // solid voxel as binary for each x,y,z axis
    u64 axis_cols[3 * CS_P2] = {0};

    // the cull mask to perform greedy slicing, based on solids on previous axis_cols
    u64 col_face_masks[3 * CS_P2 * 2] = {0};

    // masks that represent surfaces to perform greedy meshing
    std::unordered_map<u64, u64[3 * CS_P2 * 2]> surface_face_masks;

    // index
    u32 vertexIdx = 0;


    // build binary representation for every solid voxel y,x,z axis
    // voxels in this chunk
    for (u8 y = 0; y < CS; y++) {
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                u8 ax = x + 1;
                u8 ay = y + 1;
                u8 az = z + 1;

                // TODO: check if voxel is solid (not see through)
                if (chunk.voxels[get_zxy_index(x, y, z)].type) {
                    // x,z - y axis
                    axis_cols[ax + (az * CS_P)] |= (u64)1 << ay;
                    // z,y - x axis
                    axis_cols[az + (ay * CS_P) + CS_P2] |= (u64)1 << ax;
                    // x,y - z axis
                    axis_cols[ax + (ay * CS_P) + CS_P2 * 2] |= (u64)1 << az;
                }
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
                        x = dim_1;
                        y = dim_3;
                        z = dim_2;
                    } else if (axis == 1) {
                        z = dim_1;
                        x = dim_3;
                        y = dim_2;
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
                    if (has_voxel == true && voxel->type) {
                        // x,z - y axis
                        axis_cols[x + (z * CS_P)] |= (u64)1 << y;
                        // z,y - x axis
                        axis_cols[z + (y * CS_P) + CS_P2] |= (u64)1 << x;
                        // x,y - z axis
                        axis_cols[x + (y * CS_P) + CS_P2 * 2] |= (u64)1 << z;
                    }
                }
            }
        }
    }





    // face culling
    for (u8 axis = 0; axis < 3; axis++) {
        for (u8 j = 0; j < 2; j++) {
            u8 dir = axis * 2 + j;
            for (u16 i = 0; i < CS_P2; i++) {
                u64 col = axis_cols[(CS_P2 * axis) + i];

                // sample axis, and set true when air meets solid
                u64 mask = col;
                if (j == 0) {
                    mask &= ~(col >> 1);
                } else {
                    mask &= ~(col << 1);
                }

                col_face_masks[(CS_P2 * dir) + i] = mask;
            }
        }
    }






    // binary greedy meshing

    // greedy meshing setup
    // setup planes
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

                        // x,z - y axis
                        // x,y - z axis y face

                        // z,y - x axis
                        // z,x - y axis x face

                        // x,y - z axis
                        // x,z - y axis z face





                        /*
                        if dir is 0
                        each mask is going upward (y (dim_3))
                        and each bit is determining if a face is present and facing upwards or downwards (y (dim_3))
                        x (dim_1) and z (dim_2) is used for indexig the mask

                        now to convert to a binary plane
                        y faces are aligned the z axis
                        indexed with x and y
                        */




                        

                        u8 x, y, z;
                        if (axis == 0) {
                            x = dim_1;
                            y = dim_3;
                            z = dim_2;
                        } else if (axis == 1) {
                            z = dim_1;
                            x = dim_3;
                            y = dim_2;
                        } else {
                            x = dim_1;
                            z = dim_3;
                            y = dim_2;
                        }
                        surface_face_masks[chunk.voxels[get_zxy_index(x, y, z)].type][dim_1 + (dim_3 * CS) + (CS_2 * dir)] |= u64(1) << dim_2;
                    }
                }
            }
        }
    }

    // greedy meshing
    for (auto& [block_type, block_face_masks] : surface_face_masks) {
        for (u8 axis = 0; axis < 3; axis++) {
            for (u8 j = 0; j < 2; j++) {
                u8 dir = axis * 2 + j;
                for (u8 dim_2 = 0; dim_2 < CS; dim_2++) {
                    for (u8 dim_1 = 0; dim_1 < CS; dim_1++) {
                        u32 index = dim_1 + (dim_2 * CS) + (CS_2 * dir);
                        u64 face_mask = block_face_masks[index];

                        u8 dim_3 = 0;
                        while (dim_3 < CS) {
                            dim_3 += lsb(face_mask >> dim_3);
                            if (dim_3 >= CS) {
                                break;
                                // continue;
                            }

                            u8 h = countTrailingOnes(face_mask >> dim_3);

                            // 1 = 0b1, 2 = 0b11, 3 = 0b111, etc
                            u64 h_as_mask = (u64(1) << u64(h)) - u64(1);
                            // printf("%i %i %llu %llu\n", h, dim_3, h_as_mask, face_mask >> dim_3);

                            u64 mask = h_as_mask << dim_3;

                            u64 w = 1;
                            while (dim_1 + w < CS) {
                                u64 next_row_h = (block_face_masks[index + w] >> dim_3) & h_as_mask;
                                if (next_row_h != h_as_mask) {
                                    break;
                                }

                                block_face_masks[index + w] &= ~mask;

                                w++;
                            }

                            u8 x, y, z;
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

                            // printf("(%i %i %i) (%i %i) (%i)\n", x, y, z, w, h, dir);

                            vertices[vertexIdx++] = getQuad(x, y, z, w - 1, h - 1, dir, block_voxel_datas[chunk.voxels[get_zxy_index(x, y, z)].type].get_face(dir));
                            dim_3 += h;
                        }
                    }
                }
            }
        }
    }


    return vertexIdx;
}