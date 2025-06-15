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

RaycastResult raycast(const VoxelBlockWorld& voxelWorld, const Vec3<f64>& origin, const Vec3<f64>& offset);

#endif