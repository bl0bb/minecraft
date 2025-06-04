#ifndef AABB_H
#define AABB_H

#include "../../voxel/block/voxel_block_world.h"

class Intersection {
public:
    bool intersects;
    Vec3<f32> intersectPos;
    Vec3<f32> intersectDir;

    Intersection() {}

    Intersection(bool _intersects, Vec3<f32> _intersectPos, Vec3<f32> _intersectDir) :
    intersects(_intersects),
    intersectPos(_intersectPos),
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

    Intersection getIntersection(const AABB& other) const {
        Vec3<f32> aMin = worldMin();
        Vec3<f32> aMax = worldMax();
        Vec3<f32> bMin = other.worldMin();
        Vec3<f32> bMax = other.worldMax();

        Intersection intersection;
        intersection.intersects =   (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
                                    (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
                                    (aMin.z <= bMax.z && aMax.z >= bMin.z);
        
        if (intersection.intersects) {
            // Calculate overlap on each axis
            Vec3<f32> overlapMin = Vec3<f32>(Vec3<f32>::max(aMin, bMin));
            Vec3<f32> overlapMax = Vec3<f32>(Vec3<f32>::min(aMax, bMax));
            Vec3<f32> overlap = overlapMax - overlapMin;

            // If there's no overlap, return zero vector
            if (overlap.x <= 0 || overlap.y <= 0 || overlap.z <= 0) {
                intersection.intersectPos = Vec3<f32>(0.0f);
            } else {
                // Compute direction for minimal displacement
                Vec3<f32> centerA = (aMin + aMax) * 0.5f;
                Vec3<f32> centerB = (bMin + bMax) * 0.5f;
                Vec3<f32> direction = (centerA - centerB).sign(); // Direction to resolve overlap

                // Smallest axis to resolve overlap
                f32 xOverlap = direction.x * overlap.x;
                f32 yOverlap = direction.y * overlap.y;
                f32 zOverlap = direction.z * overlap.z;

                // Choose axis with minimal displacement
                f32 minOverlap = std::min(std::min(std::abs(xOverlap), std::abs(yOverlap)), std::abs(zOverlap));

                if (minOverlap == std::abs(xOverlap)) {
                    intersection.intersectPos = Vec3<f32>(xOverlap, 0.0f, 0.0f);
                } else if (minOverlap == std::abs(yOverlap)) {
                    intersection.intersectPos = Vec3<f32>(0.0f, yOverlap, 0.0f);
                } else {
                    intersection.intersectPos = Vec3<f32>(0.0f, 0.0f, zOverlap);
                }
            }
        }

        return intersection;
    }

    Intersection getIntersection(const VoxelBlockWorld& blockWorld) {
        Vec3<f32> aMin = worldMin();
        Vec3<f32> aMax = worldMax();

        i64 startX = aMin.x >= 0 ? aMin.x : aMin.x - 1;
        i64 startY = aMin.y >= 0 ? aMin.y : aMin.y - 1;
        i64 startZ = aMin.z >= 0 ? aMin.z : aMin.z - 1;

        i64 endX = aMax.x >= 0 ? aMax.x : aMax.x - 1;
        i64 endY = aMax.y >= 0 ? aMax.y : aMax.y - 1;
        i64 endZ = aMax.z >= 0 ? aMax.z : aMax.z - 1;

        Intersection intersection(false, Vec3<f32>(0.0f), Vec3<f32>(0.0f));

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

                    AABB blockAABB(Vec3<f32>(x, y, z), Vec3<f32>(1.0f));

                    Intersection newIntersection = getIntersection(blockAABB);
                    if (!intersection.intersects || (newIntersection.intersects && newIntersection.intersectPos.abs().max() < intersection.intersectPos.abs().max())) {
                        intersection = newIntersection;
                    }
                }
            }
        }

        return intersection;
    }

    void solveCollision(const Intersection& intersection) {
        if (!intersection.intersects) {
            return;
        }

        pos = pos + intersection.intersectPos;
    }
};

#endif