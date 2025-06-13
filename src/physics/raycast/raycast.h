#ifndef RAYCAST_H
#define RAYCAST_H

#include "../../voxel/block/voxel_block_world.h"

// cast a ray
// voxelWorld: the world to cast the ray in
// origin: the starting point of the ray
// offset: the offset position from origin
class RaycastResult {
public:
    bool success;

    EmbeddedVoxel block;
    Vec3<i64> blockPos;
    Vec3<f32> pos;
    u8 dir;
};

RaycastResult raycast(const VoxelBlockWorld& voxelWorld, const Vec3<f64>& origin, const Vec3<f64>& offset) {
    Vec3<f64> pos = origin;
    Vec3<f64> dir = offset.normalized();
    f64 dist = offset.magnitude();
    f64 distTraveled = 0;
    i64 blockX = pos.x >= 0 ? pos.x : pos.x - 1;
    i64 blockY = pos.y >= 0 ? pos.y : pos.y - 1;
    i64 blockZ = pos.z >= 0 ? pos.z : pos.z - 1;

    RaycastResult raycastResult;

    while (true) {
        // get distance from each edge (abs)
        f64 offX = dir.x > 0 ? (blockX + 1) - pos.x : pos.x - blockX;
        f64 offY = dir.y > 0 ? (blockY + 1) - pos.y : pos.y - blockY;
        f64 offZ = dir.z > 0 ? (blockZ + 1) - pos.z : pos.z - blockZ;

        f64 distX = dir.x ? offX / abs(dir.x) : std::numeric_limits<f64>::infinity();
        f64 distY = dir.y ? offY / abs(dir.y) : std::numeric_limits<f64>::infinity();
        f64 distZ = dir.z ? offZ / abs(dir.z) : std::numeric_limits<f64>::infinity();

        f64 stepAmount;
        u8 face;
        if (distX < distY && distX < distZ) {
            stepAmount = distX;
            face = dir.x < 0 ? 0 : 1;
        } else if (distY < distX && distY < distZ) {
            stepAmount = distY;
            face = dir.y < 0 ? 2 : 3;
        } else {
            stepAmount = distZ;
            face = dir.z < 0 ? 4 : 5;
        }

        distTraveled += stepAmount;
        pos = pos + dir * stepAmount;

        // check if the next hit is farther away than the distance and break because then were too far away and didnt hit anything
        if (distTraveled >= dist) {
            break;
        }

        if (face == 0) {
            blockX--;
        } else if (face == 1) {
            blockX++;
        } else if (face == 2) {
            blockY--;
        } else if (face == 3) {
            blockY++;
        } else if (face == 4) {
            blockZ--;
        } else {
            blockZ++;
        }

        EmbeddedVoxel* voxel;
        if (VoxelWorlds::getVoxel(voxelWorld, blockX, blockY, blockZ, &voxel)) {
            if (voxel->type != BlockTypes::AIR) {
                raycastResult.success = true;
                raycastResult.block = *voxel;
                raycastResult.pos = pos;
                raycastResult.blockPos = Vec3<i64>(blockX, blockY, blockZ);
                raycastResult.dir = face;
                return raycastResult;
            }
        }
    }

    raycastResult.success = false;
    return raycastResult;
}

#endif