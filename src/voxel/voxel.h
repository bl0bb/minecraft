#ifndef VOXEL_H
#define VOXEL_H

#include "../core/maths.h"
#include "../core/types.h"




// embedded voxel
// x = 0-4 (5) (32)
// y = 5-9 (5) (32)
// z = 10-14 (5) (32)
// type = 15-24 (10) (1024)

constexpr u8 VOXEL_X_SHIFT = 0;
constexpr u8 VOXEL_Y_SHIFT = 5;
constexpr u8 VOXEL_Z_SHIFT = 10;
constexpr u8 VOXEL_TYPE_SHIFT = 15;

constexpr u8 VOXEL_POS_DIM_MASK = 0b11111;
constexpr u16 VOXEL_POS_MASK = VOXEL_POS_DIM_MASK | (VOXEL_POS_DIM_MASK << VOXEL_Y_SHIFT) | (VOXEL_POS_DIM_MASK << VOXEL_Z_SHIFT);
constexpr u16 VOXEL_TYPE_MASK = 0b1111111111;

typedef u32 Voxel;

namespace Voxels {
    inline Voxel create(u8 x, u8 y, u8 z, u16 type) {
        return
            x & VOXEL_POS_DIM_MASK |
            (y & VOXEL_POS_DIM_MASK) << VOXEL_Y_SHIFT |
            (z & VOXEL_POS_DIM_MASK) << VOXEL_Z_SHIFT |
            (type & VOXEL_TYPE_MASK) << VOXEL_TYPE_SHIFT;
    }

    inline Vec3<u8> get_pos(Voxel voxel) {
        return Vec3<u8>(
            voxel & VOXEL_POS_DIM_MASK,
            (voxel >> VOXEL_Y_SHIFT) & VOXEL_POS_DIM_MASK,
            (voxel >> VOXEL_Z_SHIFT) & VOXEL_POS_DIM_MASK
        );
    }

    inline u8 get_type(Voxel voxel) {
        return (voxel >> VOXEL_TYPE_SHIFT) & VOXEL_TYPE_MASK;
    }

    inline Voxel setPos(Voxel voxel, const Vec3<u8>& vec) {
        return (voxel & ~VOXEL_POS_MASK) |
            (vec.x & VOXEL_POS_DIM_MASK) |
            ((vec.y & VOXEL_POS_DIM_MASK) << VOXEL_Y_SHIFT) |
            ((vec.z & VOXEL_POS_DIM_MASK) << VOXEL_Z_SHIFT);
    }
}








// embedded voxel
// type = 0-7 (8) (256)

constexpr u8 EMBEDDED_VOXEL_TYPE_SHIFT = 0;

constexpr u8 EMBEDDED_VOXEL_TYPE_MASK = 0b11111111;

typedef u8 EmbeddedVoxel;

namespace EmbeddedVoxels {
    inline EmbeddedVoxel create(u8 type) {
        return
            (type & EMBEDDED_VOXEL_TYPE_MASK) << EMBEDDED_VOXEL_TYPE_SHIFT;
    }

    inline u8 get_type(EmbeddedVoxel voxel) {
        return (voxel >> EMBEDDED_VOXEL_TYPE_SHIFT) & EMBEDDED_VOXEL_TYPE_MASK;
    }
}









#endif