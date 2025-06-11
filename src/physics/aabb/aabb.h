#ifndef AABB_H
#define AABB_H

#include <limits>

#include "../../voxel/block/voxel_block_world.h"

class Intersection {
public:
    bool intersects;
    Vec3<f32> intersectDir;

    Intersection() {}

    Intersection(bool _intersects, Vec3<f32> _intersectDir) :
    intersects(_intersects),
    intersectDir(_intersectDir) {}
};

class AABB {
public:
    Vec3<f32> pos;
    Vec3<f32> size;

    AABB() {}

    AABB(Vec3<f32> _pos, Vec3<f32> _size) :
    pos(_pos),
    size(_size) {}

    Vec3<f32> worldMax() const {
        return pos + size / 2;
    }

    Vec3<f32> worldMin() const {
        return pos - size / 2;
    }

    Intersection getIntersection(const AABB& other, const Vec3<f32>& moveDir) const {
        AABB movedAABB = *this;
        movedAABB.pos = movedAABB.pos + moveDir;

        Vec3<f32> origAMin = worldMin();
        Vec3<f32> origAMax = worldMax();

        Vec3<f32> aMin = movedAABB.worldMin();
        Vec3<f32> aMax = movedAABB.worldMax();
        Vec3<f32> bMin = other.worldMin();
        Vec3<f32> bMax = other.worldMax();

        bool overlapX = aMax.x >= bMin.x && aMin.x <= bMax.x;
        bool overlapY = aMax.y >= bMin.y && aMin.y <= bMax.y;
        bool overlapZ = aMax.z >= bMin.z && aMin.z <= bMax.z;

        if (!(overlapX && overlapY && overlapZ)) {
            return { false, moveDir };
        }



        f32 dxEntry = (moveDir.x > 0) ? (bMin.x - origAMax.x) : (bMax.x - origAMin.x);
        f32 dyEntry = (moveDir.y > 0) ? (bMin.y - origAMax.y) : (bMax.y - origAMin.y);
        f32 dzEntry = (moveDir.z > 0) ? (bMin.z - origAMax.z) : (bMax.z - origAMin.z);

        Vec3<f32> absMoveDir = moveDir.abs();
        f32 dist = moveDir.magnitude();

        f32 distX = (moveDir.x != 0) ? dxEntry / absMoveDir.x : std::numeric_limits<f32>::infinity();
        f32 distY = (moveDir.y != 0) ? dyEntry / absMoveDir.y : std::numeric_limits<f32>::infinity();
        f32 distZ = (moveDir.z != 0) ? dzEntry / absMoveDir.z : std::numeric_limits<f32>::infinity();




        // Time of collision along each axis
        f32 times[3] = { distX, distY, distZ };
        int minAxis = 0;
        f32 tMin = times[0];

        for (int i = 1; i < 3; ++i) {
            if (times[i] < tMin) {
                tMin = times[i];
                minAxis = i;
            }
        }

        // Clamp tMin to [0,1]
        // tMin = std::clamp(tMin, 0.0f, 1.0f);
        // tMin = -tMin;

        // Movement up to collision
        Vec3<f32> movementBeforeCollision = moveDir * tMin;

        printf("(%lf %lf %lf) (%lf %lf %lf) %lf\n", dxEntry, dyEntry, dzEntry, distX, distY, distZ, tMin);

        // Remaining movement
        // Vec3<f32> remaining = moveDir * (1.0f - tMin);
        // remaining[minAxis] = 0.0f; // Don't move into the collided axis
        // remaining = remaining.normalized();

        // Vec3<f32> finalMove = movementBeforeCollision + remaining;


        Vec3<f32> finalMove = movementBeforeCollision;

        return { true, finalMove };







        // f32 dxEntry = (moveDir.x > 0) ? (bMin.x - origAMax.x) : (bMax.x - origAMin.x);
        // f32 dyEntry = (moveDir.y > 0) ? (bMin.y - origAMax.y) : (bMax.y - origAMin.y);
        // f32 dzEntry = (moveDir.z > 0) ? (bMin.z - origAMax.z) : (bMax.z - origAMin.z);

        // f32 tx = (moveDir.x != 0) ? dxEntry / moveDir.x : std::numeric_limits<f32>::infinity();
        // f32 ty = (moveDir.y != 0) ? dyEntry / moveDir.y : std::numeric_limits<f32>::infinity();
        // f32 tz = (moveDir.z != 0) ? dzEntry / moveDir.z : std::numeric_limits<f32>::infinity();

        // // Find the earliest time of collision
        // f32 tMin = std::min({ tx, ty, tz });

        // tMin = std::clamp(tMin, 0.0f, 1.0f);

        // // Reduce movement only along the first colliding axis
        // Vec3<f32> resolvedMove = moveDir * tMin;

        // return { true, resolvedMove };







        // // Calculate overlap in each axis
        // f32 overlapAmountX = (moveDir.x > 0) ? (bMin.x - aMax.x) : (bMax.x - aMin.x);
        // f32 overlapAmountY = (moveDir.y > 0) ? (bMin.y - aMax.y) : (bMax.y - aMin.y);
        // f32 overlapAmountZ = (moveDir.z > 0) ? (bMin.z - aMax.z) : (bMax.z - aMin.z);

        // // Clamp overlaps to prevent moving past the original position
        // Vec3<f32> resolvedMove = moveDir;

        // if (overlapX) resolvedMove.x += overlapAmountX;
        // if (overlapY) resolvedMove.y += overlapAmountY;
        // if (overlapZ) resolvedMove.z += overlapAmountZ;
        
        // return { true, resolvedMove };
    }

    Intersection getIntersection(const VoxelBlockWorld& blockWorld, const Vec3<f32>& moveDir) const {
        Vec3<f32> aMin = worldMin();
        Vec3<f32> aMax = worldMax();

        i64 startX = aMin.x >= 0 ? aMin.x : aMin.x - 1;
        i64 startY = aMin.y >= 0 ? aMin.y : aMin.y - 1;
        i64 startZ = aMin.z >= 0 ? aMin.z : aMin.z - 1;

        i64 endX = aMax.x >= 0 ? aMax.x : aMax.x - 1;
        i64 endY = aMax.y >= 0 ? aMax.y : aMax.y - 1;
        i64 endZ = aMax.z >= 0 ? aMax.z : aMax.z - 1;

        Intersection intersection(false, moveDir);

        for (i64 x = startX; x <= endX; x++) {
            for (i64 y = startY; y <= endY; y++) {
                for (i64 z = startZ; z <= endZ; z++) {
                    EmbeddedVoxel* voxel;
                    if (!VoxelWorlds::getVoxel(blockWorld, x, y, z, &voxel)) {
                        continue;
                    }

                    if (voxel->type == BlockTypes::AIR) {
                        continue;
                    }

                    AABB blockAABB(Vec3<f32>(x, y, z) + 0.5f, Vec3<f32>(1.0f));

                    Intersection newIntersection = getIntersection(blockAABB, intersection.intersectDir);
                    if (!intersection.intersects || newIntersection.intersects) {
                        intersection = newIntersection;
                    }
                }
            }
        }

        return intersection;
    }
};

#endif