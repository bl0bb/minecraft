#include <vector>

#include "../core/types.h"
#include "../core/bits.h"

#include "../blocks.h"

#include "voxel.h"

#include "voxel_mesher.h"










void printBinary(unsigned int num, u8 count) {
    // Adjust for 32-bit integers
    for (int i = count - 1; i >= 0; i--) {
        unsigned int mask = 1 << i;
        putchar((num & mask) ? '1' : '0');

        // Optional: Add a space every 4 bits for readability
        // if (i % 4 == 0) {
        //     putchar(' ');
        // }
    }
}







u32 generate_voxel_mesh(const EmbeddedVoxel* voxels, VoxelFace* vertices) {
    // solid voxel as binary for each x,y,z axis
    u64 axis_cols[3 * CS_P2] = {0};
    // the cull mask to perform greedy slicing, based on solids on previous axis_cols
    u64 col_face_masks[3 * CS_P2 * 2] = {0};
    // masks that represent surfaces to perform greedy meshing
    u64 surface_face_masks[3 * CS_P2 * 2] = {0};

    // index
    u32 vertexIdx = 0;

    // build binary representation for every solid voxel y,x,z axis
    for (u8 y = 0; y < CS_P; y++) {
        for (u8 x = 0; x < CS_P; x++) {
            for (u8 z = 0; z < CS_P; z++) {
                // TODO: create Vec3<T1, T2, T3>
                Vec3<u16> pos(x - 1, y - 1, z - 1);
                // TODO: check if voxel is solid (not see through)
                if (voxels[get_zxy_index(x, y, z)]) {
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

    // face culling
    for (u8 axis = 0; axis < 3; axis++) {
        for (u8 j = 0; j < 2; j++) {
            u8 dir = axis * 2 + j;
            for (u16 i = 0; i < CS_P2; i++) {
                u64 col = axis_cols[(CS_P2 * axis) + i];

                // sample ascending axis, and set true when air meets solid
                // u64 mask = col & ~(col >> 1);
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
    for (u8 axis = 0; axis < 3; axis++) {
        for (u8 j = 0; j < 2; j++) {
            u8 dir = axis * 2 + j;
            for (u8 dim_2 = 0; dim_2 < CS_P; dim_2++) {
                for (u8 dim_1 = 0; dim_1 < CS_P; dim_1++) {
                    u32 index = (CS_P2 * dir) + (dim_2 * CS_P) + dim_1;
                    u64 face_mask = col_face_masks[index];

                    // // remove padding right
                    // face_mask >>= 1;
                    // // remove padding left
                    // face_mask &= !(u64(1) << CS);

                    while (face_mask) {
                        u8 dim_3 = pop_lsb(face_mask);


                        // surface_face_masks[dim_1 + (dim_2 * CS_P) + (CS_P2 * dir)] |= u64(1) << dim_3;





                        // u8 x, y, z;
                        // if (axis == 2) {
                        //     // 0 2
                        //     x = dim_1;
                        //     z = dim_2;
                        //     y = dim_3;
                        // } else if (axis == 0) {
                        //     // 1 0
                        //     z = dim_1;
                        //     y = dim_2;
                        //     x = dim_3;
                        // } else {
                        //     // 2 1
                        //     x = dim_1;
                        //     y = dim_2;
                        //     z = dim_3;
                        // }





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




                        surface_face_masks[dim_1 + (dim_3 * CS_P) + (CS_P2 * dir)] |= u64(1) << dim_2;
                    }
                }
            }
        }
    }

    // greedy meshing
    for (u8 axis = 0; axis < 3; axis++) {
        for (u8 j = 0; j < 2; j++) {
            u8 dir = axis * 2 + j;
            for (u8 dim_2 = 0; dim_2 < CS_P; dim_2++) {
                for (u8 dim_1 = 0; dim_1 < CS_P; dim_1++) {
                    u32 index = dim_1 + (dim_2 * CS_P) + (CS_P2 * dir);
                    u64 face_mask = surface_face_masks[index];

                    // if (axis == 0) { //  && dim_1 == 28 && dim_2 == 27
                    //     printBinary(face_mask, 64);
                    //     printf("\n");
                    // }

                    u8 dim_3 = 0;
                    while (dim_3 < CS_P) {
                        dim_3 += lsb(face_mask >> dim_3);
                        if (dim_3 >= CS_P) {
                            // continue;
                            break;
                        }

                        u8 h = countTrailingOnes(face_mask >> dim_3);

                        // 1 = 0b1, 2 = 0b11, 3 = 0b111, etc
                        u64 h_as_mask = (u64(1) << u64(h)) - u64(1);
                        // printf("%i %i %llu %llu\n", h, dim_3, h_as_mask, face_mask >> dim_3);

                        u64 mask = h_as_mask << dim_3;

                        u64 w = 1;
                        while (dim_1 + w < CS_P) {
                            u64 next_row_h = (surface_face_masks[index + w] >> dim_3) & h_as_mask;
                            if (next_row_h != h_as_mask) {
                                break;
                            }

                            surface_face_masks[index + w] &= ~mask;

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

                        printf("(%i %i %i) (%i %i) (%i)\n", x, y, z, w, h, dir);

                        vertices[vertexIdx++] = getQuad(x, y, z, w - 1, h - 1, dir, BlockVoxelDatas::get_face(block_voxel_datas[EmbeddedVoxels::get_type(voxels[get_zxy_index(x, y, z)]) - 1], dir));
                        dim_3 += h;
                    }
                }
            }
        }
    }







    // generate quads
    // for (u8 axis = 0; axis < 3; axis++) {
    //     for (u8 j = 0; j < 2; j++) {
    //         u8 dir = axis * 2 + j;
    //         for (u16 i = 0; i < CS_P2; i++) {
    //             u8 dim_1 = i % CS_P;
    //             u8 dim_2 = i / CS_P;

    //             u64 face_mask = col_face_masks[(CS_P2 * dir) + i];
    //             while (face_mask) {
    //                 u8 tile = pop_lsb(face_mask);
    //                 // // tile is first or tile is last?
    //                 // if (tile == 0) {
    //                 //     continue;
    //                 // }
    //                 u8 dim_3 = tile;

    //                 u8 x, y, z;
    //                 if (axis == 0) {
    //                     x = dim_1;
    //                     z = dim_2;
    //                     y = dim_3;
    //                 } else if (axis == 1) {
    //                     z = dim_1;
    //                     y = dim_2;
    //                     x = dim_3;
    //                 } else {
    //                     x = dim_1;
    //                     y = dim_2;
    //                     z = dim_3;
    //                 }

    //                 // TODO: find faster / better way to find material??
    //                 vertices[vertexIdx++] = getQuad(x, y, z, 1, 1, dir, BlockVoxelDatas::get_face(block_voxel_datas[EmbeddedVoxels::get_type(voxels[get_zxy_index(x, y, z)]) - 1], dir));
    //             }
    //         }
    //     }
    // }












    return vertexIdx;

    // for (u32 i = 0; i < meshData.vertices->size(); i++) {
    //     printf("Face %i: ", i);
    //     u64 face = (*meshData.vertices)[i];
    //     printf("%i %i %i\n", face & 0b11111, (face >> 5) & 0b11111, (face >> 10) & 0b11111);
    //     // printBinary(face & 0b11111, 5);
    //     // printf(" ");
    //     // printBinary((face >> 5) & 0b11111, 5);
    //     // printf(" ");
    //     // printBinary((face >> 10) & 0b11111, 5);
    //     // printf("\n");
    // }
    // printf("%i\n", meshData.vertices->size());
}