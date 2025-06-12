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
        if (moveDir.magnitude() == 0) {
            return { false, moveDir, };
        }
        
        Vec3<f32> aMin = worldMin();
        Vec3<f32> aMax = worldMax();
        Vec3<f32> bMin = other.worldMin();
        Vec3<f32> bMax = other.worldMax();

        bool checkOverlapX = aMax.x > bMin.x && aMin.x < bMax.x;
        bool checkOverlapY = aMax.y > bMin.y && aMin.y < bMax.y;
        bool checkOverlapZ = aMax.z > bMin.z && aMin.z < bMax.z;
        if (checkOverlapX && checkOverlapY && checkOverlapZ) {
            printf("ORIGINAL INTERSECT\n");
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



        f32 dxEntry = (moveDir.x > 0) ? (bMin.x - aMax.x) : (bMax.x - aMin.x);
        f32 dyEntry = (moveDir.y > 0) ? (bMin.y - aMax.y) : (bMax.y - aMin.y);
        f32 dzEntry = (moveDir.z > 0) ? (bMin.z - aMax.z) : (bMax.z - aMin.z);

        Vec3<f32> absMoveDir = moveDir.abs();

        f32 distX = moveDir.x ? abs(dxEntry) / absMoveDir.x : std::numeric_limits<f32>::infinity();
        f32 distY = moveDir.y ? abs(dyEntry) / absMoveDir.y : std::numeric_limits<f32>::infinity();
        f32 distZ = moveDir.z ? abs(dzEntry) / absMoveDir.z : std::numeric_limits<f32>::infinity();




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

        tMin = std::clamp(tMin, 0.0f, 1.0f);

        // Movement up to collision
        Vec3<f32> movementBeforeCollision = moveDir * tMin;

        printf("(%lf %lf %lf) (%lf %lf %lf) (%lf %lf %lf) %lf\n", dxEntry, dyEntry, dzEntry, distX, distY, distZ, moveDir.x, moveDir.y, moveDir.z, tMin);




        // Remaining movement
        Vec3<f32> remaining = moveDir;
        remaining[minAxis] = 0.0f; // Don't move into the collided axis
        // remaining = remaining.normalized() * (moveDir.magnitude() * (1.0f - tMin));
        remaining = remaining * (1.0f - tMin);

        printf("test a (%lf %lf) (%lf %lf)\n", movedAMax.y, bMin.y, movedAMin.y, bMax.y);
        printf("test b (%lf %lf) (%lf %lf)\n", aMax.y, bMin.y, aMin.y, bMax.y);
        printf("move dir (%lf %lf %lf)\n", remaining.x, remaining.y, remaining.z);

        Vec3<f32> finalMove = movementBeforeCollision + remaining;




        return { true, finalMove };
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

        Intersection intersection(false, moveDir);

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

                    // EmbeddedVoxel& voxel = cachedVoxels[ix + (iy * sizeY) + (iz * sizeY * sizeZ)];
                    // if (voxel.type == BlockTypes::AIR) {
                    //     continue;
                    // }
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
        printf("b\n");

        return intersection;
    }
};

#endif