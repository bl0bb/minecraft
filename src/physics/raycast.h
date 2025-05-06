#ifndef RAYCAST_H
#define RAYCAST_H

#include "../voxel/logic/voxel_game_world.h"

// cast a ray
// voxelWorld: the world to cast the ray in
// origin: the starting point of the ray
// offset: the offset position from origin
bool raycast(const VoxelGameWorld& voxelWorld, Vec3<f64>& origin, Vec3<f64>& offset) {
    Vec3<f64> pos = origin;
    Vec3<f64> dir = offset.normalized();
    f64 dist = offset.magnitude();
    f64 distTraveled = 0;
    i64 blockX = pos.x;
    i64 blockY = pos.y;
    i64 blockZ = pos.z;
    f64 offX;
    f64 offY;
    f64 offZ;
    f64 stepAmount;
    u8 face;
    while (true) {
        offX = dir.x > 0 ? blockX - pos.x : pos.x - (blockX - 1);
        offY = dir.y > 0 ? blockY - pos.y : pos.y - (blockY - 1);
        offZ = dir.z > 0 ? blockZ - pos.z : pos.z - (blockZ - 1);
        if (offX > offY && offX > offZ) {
            stepAmount = offX / abs(dir.x);
            face = pos.x > 0 ? 3 : 2;
        } else if (offY > offX && offY > offZ) {
            stepAmount = offY / abs(dir.y);
            face = pos.y > 0 ? 1 : 0;
        } else {
            stepAmount = offZ / abs(dir.z);
            face = pos.z > 0 ? 5 : 4;
        }

        // check if the next hit is farther away than the distance and break because then were too far away and didnt hit anything
        if (stepAmount + distTraveled > dist) {
            break;
        }

        if (face == 0) {
            blockX++;
        } else if (face == 1) {

        }

        distTraveled += stepAmount;
        pos = pos + dir * stepAmount;
    }
}

#endif