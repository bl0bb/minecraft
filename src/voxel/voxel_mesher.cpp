#include <vector>

#include "../core/maths.h"
#include "../core/types.h"

#include "voxel_mesher.h"




static inline const int getAxisIndex(const int axis, const int a, const int b, const int c) {
    if (axis == 0) return b + (a * CS_P) + (c * CS_P2);
    else if (axis == 1) return b + (c * CS_P) + (a * CS_P2);
    else return c + (a * CS_P) + (b * CS_P2);
}

static inline const void insertQuad(std::vector<u64>& vertices, u64 quad, int& vertexI, int& maxVertices) {
    if (vertexI >= maxVertices - 6) {
        vertices.resize(maxVertices * 2, 0);
        maxVertices *= 2;
    }

    vertices[vertexI] = quad;

    vertexI++;
}

static inline const u64 getQuad(u64 x, u64 y, u64 z, u64 w, u64 h, u64 type) {
    return (type << 32) | (h << 24) | (w << 18) | (z << 12) | (y << 6) | x;
}

void generate_voxel_mesh(const u8* voxels, MeshData& meshData) {
    // 0 is just a random number
    meshData.vertices = new std::vector<u64>(0);

    // solid voxel as binary for each x,y,z axis
    u64 axis_cols[3 * CS_P2 * CH] = {0};
    // the cull mask to perform greedy slicing, based on solids on previous axis_cols
    u64 col_face_masks[3 * CS_P2 * CH] = {0};

    // build binary representation for every solid voxel y,x,z axis
    for (u8 y = 0; y < CS_P; y++) {
        for (u8 x = 0; x < CS_P; x++) {
            for (u8 z = 0; z < CS_P; z++) {
                // TODO: create Vec3<T1, T2, T3>
                Vec3<u16> pos(x - 1, y - 1, z - 1);
                // TODO: check if voxel is solid (not see through)
                if (true) {
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
            col_face_masks[(CS_P2 * (axis * 2 + 1)) + i] = col & ~(col >> 1);
            // sample descending axis, and set true when air meets solid
            col_face_masks[(CS_P2 * (axis * 2 + 0)) + i] = col & ~(col << 1);
        }
    }
}