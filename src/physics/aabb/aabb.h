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

    Intersection getIntersection(const AABB& other, const Vec3<f32>& moveDir) const {
        if (moveDir.magnitude() == 0) {
            return { false, moveDir };
        }
        
        Vec3<f32> aMin = worldMin();
        Vec3<f32> aMax = worldMax();
        Vec3<f32> bMin = other.worldMin();
        Vec3<f32> bMax = other.worldMax();

        bool checkOverlapX = aMax.x > bMin.x && aMin.x < bMax.x;
        bool checkOverlapY = aMax.y > bMin.y && aMin.y < bMax.y;
        bool checkOverlapZ = aMax.z > bMin.z && aMin.z < bMax.z;
        if (checkOverlapX && checkOverlapY && checkOverlapZ) {
            // TODO: move out of block if somehow clipped inside?
        }
        
        AABB movedAABB = *this;
        movedAABB.pos = movedAABB.pos + moveDir;

        Vec3<f32> movedAMin = movedAABB.worldMin();
        Vec3<f32> movedAMax = movedAABB.worldMax();

        bool overlapX = movedAMax.x > bMin.x && movedAMin.x < bMax.x;
        bool overlapY = movedAMax.y > bMin.y && movedAMin.y < bMax.y;
        bool overlapZ = movedAMax.z > bMin.z && movedAMin.z < bMax.z;

        if (!(overlapX && overlapY && overlapZ)) {
            return { false, moveDir };
        }

        Vec3<f32> absMoveDir = moveDir.abs();


        std::array<f32, 3> times;
        times.fill(std::numeric_limits<f32>::infinity());

        if ((moveDir.x > 0 && other.intersectsDir(1)) || (moveDir.x < 0 && other.intersectsDir(0))) {
            f32 dxEntry = (moveDir.x > 0) ? (bMin.x - aMax.x) : (bMax.x - aMin.x);
            times[0] = abs(dxEntry) / absMoveDir.x;
        }

        if ((moveDir.y > 0 && other.intersectsDir(3)) || (moveDir.y < 0 && other.intersectsDir(2))) {
            f32 dyEntry = (moveDir.y > 0) ? (bMin.y - aMax.y) : (bMax.y - aMin.y);
            times[1] = abs(dyEntry) / absMoveDir.y;
        }

        if ((moveDir.z > 0 && other.intersectsDir(5)) || (moveDir.z < 0 && other.intersectsDir(4))) {
            f32 dzEntry = (moveDir.z > 0) ? (bMin.z - aMax.z) : (bMax.z - aMin.z);
            times[2] = abs(dzEntry) / absMoveDir.z;
        }




        // Time of collision along each axis
        int minAxis = 0;
        f32 tMin = times[0];

        for (int i = 1; i < 3; ++i) {
            if (times[i] < tMin) {
                tMin = times[i];
                minAxis = i;
            }
        }

        // we didnt actually collide even though we intersected because of intersect dir flags
        if (tMin == std::numeric_limits<f32>::infinity()) {
            return { false, moveDir };
        }

        tMin = std::clamp(tMin, 0.0f, 1.0f);

        // Movement up to collision
        Vec3<f32> movementBeforeCollision = moveDir * tMin;




        // Remaining movement
        Vec3<f32> remaining = moveDir;
        remaining[minAxis] = 0.0f; // Don't move into the collided axis
        // remaining = remaining.normalized() * (moveDir.magnitude() * (1.0f - tMin));
        remaining = remaining * (1.0f - tMin);

        Vec3<f32> finalMove = movementBeforeCollision + remaining;




        Vec3<f32> collideSolveForce(1.0f);
        collideSolveForce[minAxis] = 0.0f;
        return { true, finalMove, collideSolveForce };
    }

    Intersection getIntersection(const VoxelBlockWorld& blockWorld, const Vec3<f32>& moveDir) const {
        Vec3<f32> origAMin = worldMin();
        Vec3<f32> origAMax = worldMax();

        AABB movedAABB = *this;
        movedAABB.pos = movedAABB.pos + moveDir;
        Vec3<f32> movedAMin = movedAABB.worldMin();
        Vec3<f32> movedAMax = movedAABB.worldMax();

        Vec3<f32> aMin = Vec3<f32>::min(origAMin, movedAMin);
        Vec3<f32> aMax = Vec3<f32>::max(origAMax, movedAMax);

        i64 startX = aMin.x >= 0 ? aMin.x : aMin.x - 1;
        i64 startY = aMin.y >= 0 ? aMin.y : aMin.y - 1;
        i64 startZ = aMin.z >= 0 ? aMin.z : aMin.z - 1;

        i64 endX = aMax.x >= 0 ? aMax.x : aMax.x - 1;
        i64 endY = aMax.y >= 0 ? aMax.y : aMax.y - 1;
        i64 endZ = aMax.z >= 0 ? aMax.z : aMax.z - 1;

        Intersection intersection(false, moveDir, Vec3<f32>(1.0f));

        i64 sizeX = endX - startX + 1;
        i64 sizeY = endY - startY + 1;
        i64 sizeZ = endZ - startZ + 1;
        EmbeddedVoxel cachedVoxels[sizeX * sizeY * sizeZ];
        for (i64 x = startX; x <= endX; x++) {
            for (i64 y = startY; y <= endY; y++) {
                for (i64 z = startZ; z <= endZ; z++) {
                    i64 ix = x - startX;
                    i64 iy = y - startY;
                    i64 iz = z - startZ;
                    
                    i64 index = ix + (iy * sizeX) + (iz * sizeX * sizeY);

                    EmbeddedVoxel* voxel;
                    if (!VoxelWorlds::getVoxel(blockWorld, x, y, z, &voxel)) {
                        cachedVoxels[index] = 0;
                        continue;
                    }

                    cachedVoxels[index] = *voxel;
                }
            }
        }


        for (i64 x = startX; x <= endX; x++) {
            for (i64 y = startY; y <= endY; y++) {
                for (i64 z = startZ; z <= endZ; z++) {
                    i64 ix = x - startX;
                    i64 iy = y - startY;
                    i64 iz = z - startZ;

                    EmbeddedVoxel& voxel = cachedVoxels[ix + (iy * sizeX) + (iz * sizeX * sizeY)];
                    if (voxel.type == BlockTypes::AIR) {
                        continue;
                    }

                    u8 collideFlag = 0;
                    if (x < endX) collideFlag |= (cachedVoxels[(ix + 1) + (iy * sizeX) + (iz * sizeX * sizeY)].type == BlockTypes::AIR) << 0;
                    if (x > startX) collideFlag |= (cachedVoxels[(ix - 1) + (iy * sizeX) + (iz * sizeX * sizeY)].type == BlockTypes::AIR) << 1;
                    if (y < endY) collideFlag |= (cachedVoxels[ix + ((iy + 1) * sizeX) + (iz * sizeX * sizeY)].type == BlockTypes::AIR) << 2;
                    if (y > startY) collideFlag |= (cachedVoxels[ix + ((iy - 1) * sizeX) + (iz * sizeX * sizeY)].type == BlockTypes::AIR) << 3;
                    if (z < endZ) collideFlag |= (cachedVoxels[ix + (iy * sizeX) + ((iz + 1) * sizeX * sizeY)].type == BlockTypes::AIR) << 4;
                    if (z > startZ) collideFlag |= (cachedVoxels[ix + (iy * sizeX) + ((iz - 1) * sizeX * sizeY)].type == BlockTypes::AIR) << 5;

                    AABB blockAABB(Vec3<f32>(x, y, z) + 0.5f, Vec3<f32>(1.0f), collideFlag);

                    Intersection newIntersection = getIntersection(blockAABB, intersection.intersectDir);
                    if (!intersection.intersects || newIntersection.intersects) {
                        Vec3<f32> newCollideSolveForce = intersection.collideSolveForce * newIntersection.collideSolveForce;
                        intersection = newIntersection;
                        intersection.collideSolveForce = newCollideSolveForce;
                    }
                }
            }
        }

        return intersection;
    }
};

#endif