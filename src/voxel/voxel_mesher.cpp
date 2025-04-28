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







void generate_voxel_mesh(const EmbeddedVoxel* voxels, MeshData& meshData) {
    // 0 is just a random number
    meshData.vertices = new std::vector<u64>(0);

    // solid voxel as binary for each x,y,z axis
    u64 axis_cols[3 * CS_P2] = {0};
    // the cull mask to perform greedy slicing, based on solids on previous axis_cols
    u64 col_face_masks[3 * CS_P2 * 2] = {0};

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
        for (u16 i = 0; i < CS_P2; i++) {
            u64 col = axis_cols[(CS_P2 * axis) + i];
            // sample ascending axis, and set true when air meets solid
            col_face_masks[(CS_P2 * (axis * 2 + 0)) + i] = col & ~(col >> 1);
            // sample descending axis, and set true when air meets solid
            col_face_masks[(CS_P2 * (axis * 2 + 1)) + i] = col & ~(col << 1);
        }
    }

    // generate quads
    for (u8 axis = 0; axis < 3; axis++) {
        for (u8 j = 0; j < 2; j++) {
            u8 dir = axis * 2 + j;
            for (u16 i = 0; i < CS_P2; i++) {
                u8 dim_1 = i % CS_P;
                u8 dim_2 = i / CS_P;

                u64 face_mask = col_face_masks[(CS_P2 * dir) + i];
                while (face_mask) {
                    u8 tile = pop_lsb(face_mask);
                    // // tile is first or tile is last?
                    // if (tile == 0) {
                    //     continue;
                    // }
                    u8 dim_3 = tile;

                    u8 x, y, z;
                    if (axis == 0) {
                        x = dim_1;
                        z = dim_2;
                        y = dim_3;
                    } else if (axis == 1) {
                        z = dim_1;
                        y = dim_2;
                        x = dim_3;
                    } else {
                        x = dim_1;
                        y = dim_2;
                        z = dim_3;
                    }

                    // TODO: find faster / better way to find material??
                    meshData.vertices->push_back(getQuad(x, y, z, 1, 1, dir, BlockVoxelDatas::get_face(block_voxel_datas[EmbeddedVoxels::get_type(voxels[get_zxy_index(x, y, z)]) - 1], dir)));
                }
            }
        }
    }

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