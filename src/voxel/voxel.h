#ifndef VOXEL_H
#define VOXEL_H

#include "../core/maths.h"
#include "../core/types.h"

// x = 0-4 (5) (32)
// y = 5-14 (10) (1024)
// z = 15-19 (5) (32)
// type = 20-29 (10) (1024)

constexpr u8 VOXEL_XZ_MASK = 0b11111;
constexpr u8 VOXEL_Y_MASK = 0b1111111111;
constexpr u8 VOXEL_TYPE_MASK = 0b1111111111;

constexpr u8 VOXEL_X_SHIFT = 0;
constexpr u8 VOXEL_Y_SHIFT = 5;
constexpr u8 VOXEL_Z_SHIFT = 15;
constexpr u8 VOXEL_TYPE_SHIFT = 20;

typedef u64 Voxel;

namespace Voxels {
    inline Voxel Voxels::create(u8 x, u8 y, u8 z, u16 type) {
        return
            x & VOXEL_XZ_MASK |
            (y & VOXEL_Y_MASK) << VOXEL_Y_SHIFT |
            (z & VOXEL_XZ_MASK) << VOXEL_Z_SHIFT |
            (type & VOXEL_TYPE_MASK) << VOXEL_TYPE_SHIFT;
    }

    inline Vec3<u16> pos(Voxel voxel) {
        return Vec3<u16>(
            voxel & VOXEL_XZ_MASK,
            (voxel >> VOXEL_Y_SHIFT) & VOXEL_Y_MASK,
            (voxel >> VOXEL_Z_SHIFT) & VOXEL_XZ_MASK
        );
    }

    inline Vec3<u16> setPos(Voxel voxel, const Vec3<u16>& vec) {
        return Vec3<u16>(
            voxel & VOXEL_XZ_MASK,
            (voxel >> VOXEL_Y_SHIFT) & VOXEL_Y_MASK,
            (voxel >> VOXEL_Z_SHIFT) & VOXEL_XZ_MASK
        );
    }
}

#endif