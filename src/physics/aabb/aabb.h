#ifndef AABB_H
#define AABB_H

#include <limits>

#include "../../voxel/block/voxel_block_world.h"

class Intersection {
public:
    bool intersects;
    Vec3<f32> intersectDir;
    Vec3<f32> collideSolveForce;

    Intersection() {}

    Intersection(bool _intersects, Vec3<f32> _intersectDir) :
    intersects(_intersects),
    intersectDir(_intersectDir),
    collideSolveForce(Vec3<f32>(1.0f)) {}

    Intersection(bool _intersects, Vec3<f32> _intersectDir, Vec3<f32> _collideSolveForce) :
    intersects(_intersects),
    intersectDir(_intersectDir),
    collideSolveForce(_collideSolveForce) {}
};

class AABB {
public:
    Vec3<f32> pos;
    Vec3<f32> size;
    u8 intersectFlag;

    AABB() {}

    AABB(Vec3<f32> _pos, Vec3<f32> _size) :
    pos(_pos),
    size(_size),
    intersectFlag(0b00111111) {}

    AABB(Vec3<f32> _pos, Vec3<f32> _size, u8 _intersectFlag) :
    pos(_pos),
    size(_size),
    intersectFlag(_intersectFlag) {}

    inline bool intersectsDir(u8 dir) const {
        return (intersectFlag >> dir) & 1;
    }

    inline Vec3<f32> worldMax() const {
        return pos + size / 2;
    }

    inline Vec3<f32> worldMin() const {
        return pos - size / 2;
    }

    Intersection getIntersection(const AABB& other, const Vec3<f32>& moveDir) const;

    Intersection getIntersection(const VoxelBlockWorld& blockWorld, const Vec3<f32>& moveDir) const;
};

#endif