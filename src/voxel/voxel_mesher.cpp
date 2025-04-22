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

// x = 0-4 (5) (32)
// y = 5-9 (5) (32)
// z = 10-14 (5) (32)
// w = 15-19 (5) (32)
// h = 20-24 (5) (32)
// dir = 25-37 (3) (8)
// type = 28-37 (10) (1024)
static inline const u64 getQuad(u64 x, u64 y, u64 z, u64 w, u64 h, u64 dir, u64 type) {
    return (type << 28) | (dir << 25) | (h << 20) | (w << 15) | (z << 10) | (y << 5) | x;
}

void generate_voxel_mesh(const u8* voxels, MeshData& meshData) {
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

    // generate quads
    for (u8 y = 0; y < CS_P; y++) {
        for (u8 x = 0; x < CS_P; x++) {
            for (u8 z = 0; z < CS_P; z++) {
                for (u16 j = 0; j < 2; j++) {
                    
                }
            }
        }
    }
    for (u8 axis = 0; axis < 3; axis++) {
        for (u16 i = 0; i < CS_P2; i++) {
            for (u8 j = 0; j < 2; j++) {
                u8 dir = axis * 2 + j;

                u8 dim_1 = i % CS_P;
                u8 dim_2 = i % CS_P * 2;

                u64 face_mask = col_face_masks[(CS_P2 * dir) + i];
                while (face_mask) {
                    u8 tile = pop_lsb(face_mask);
                    u8 dir_1 = i %
                    meshData.quads.push_back(getQuad(0, 0, 0, 1, 1, dir, 1));
                }
            }
        }
    }

    for (u8 axis = 0; axis < 3; axis++) {
        printf("Axis: %i\n", axis);
        for (u16 i = 0; i < CS_P2; i++) {
            u64 col = axis_cols[(CS_P2 * axis) + i];
            printBinary(col_face_masks[(CS_P2 * (axis * 2 + 0)) + i] | col_face_masks[(CS_P2 * (axis * 2 + 1)) + i]);
        }
    }
}

// Returns the least significant bit in a non-zero bitboard.
inline u64 lsb(u64 b) {
#if defined(__GNUC__)  // GCC, Clang, ICX

    return u64(__builtin_ctzll(b));

#elif defined(_MSC_VER)
    #ifdef _WIN64  // MSVC, WIN64

    unsigned long idx;
    _BitScanForward64(&idx, b);
    return u64(idx);

    #else  // MSVC, WIN32
    unsigned long idx;

    if (b & 0xffffffff)
    {
        _BitScanForward(&idx, int32_t(b));
        return u64(idx);
    }
    else
    {
        _BitScanForward(&idx, int32_t(b >> 32));
        return u64(idx + 32);
    }
    #endif
#else  // Compiler is neither GCC nor MSVC compatible
    #error "Compiler not supported."
#endif
}

// Returns the most significant bit in a non-zero bitboard.
inline u64 msb(u64 b) {
#if defined(__GNUC__)  // GCC, Clang, ICX

    return u64(63 ^ __builtin_clzll(b));

#elif defined(_MSC_VER)
    #ifdef _WIN64  // MSVC, WIN64

    unsigned long idx;
    _BitScanReverse64(&idx, b);
    return u64(idx);

    #else  // MSVC, WIN32

    unsigned long idx;

    if (b >> 32)
    {
        _BitScanReverse(&idx, int32_t(b >> 32));
        return u64(idx + 32);
    }
    else
    {
        _BitScanReverse(&idx, int32_t(b));
        return u64(idx);
    }
    #endif
#else  // Compiler is neither GCC nor MSVC compatible
    #error "Compiler not supported."
#endif
}

inline u8 pop_lsb(u64& b) {
    const u64 s = lsb(b);
    b &= b - 1;
    return s;
}

void printBinary(unsigned int num) {
    // Adjust for 32-bit integers
    for (int i = 31; i >= 0; i--) {
        unsigned int mask = 1 << i;
        putchar((num & mask) ? '1' : '0');

        // Optional: Add a space every 4 bits for readability
        // if (i % 4 == 0) {
        //     putchar(' ');
        // }
    }
    putchar('\n');
}