#ifndef LIGHT_H
#define LIGHT_H

#include <queue>

#include "../core/maths.h"
#include "../core/color.h"
#include "../voxel/block/voxel_block_world.h"
#include "../voxel/height/voxel_height_world.h"
#include "../voxel/light/voxel_light_world.h"






template <typename T>
T getElementAt(std::queue<T> q, size_t index);



// Directions
// Start with DOWN because it makes sunlight propagate faster
const Vec3<i8> directions[6] = {
    { 0, -1,  0},
    { 0,  1,  0},
    { 1,  0,  0},
    {-1,  0,  0},
    { 0,  0,  1},
    { 0,  0, -1},

    // { 1,  0,  0},
    // {-1,  0,  0},
    // { 0,  1,  0},
    // { 0, -1,  0},
    // { 0,  0,  1},
    // { 0,  0, -1},
};

template<typename LightType>
using LightQueue = std::queue<std::tuple<Vec3<i64>, LightType>>;

typedef LightQueue<RGBIS4> AllLightQueue;

namespace ChunkLight {
    enum LightPropagationType : u8 {
        DEFAULT_LIGHT,
        SUN_LIGHT,
    };

    static void add_propagate(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, AllLightQueue& lightQueue, u32 mask, u32 offset, LightPropagationType type);

    static void remove_propagate(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, AllLightQueue& lightQueue, AllLightQueue& propQueue, u32 mask, u32 offset, LightPropagationType type);

    static void add_channel(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, const Vec3<i64>& pos, u8 value, u32 mask, u32 offset, LightPropagationType type);

    static void remove_channel(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, const Vec3<i64>& pos, u32 mask, u32 offset, LightPropagationType type);

    static void add_light(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, const Vec3<i64>& pos, RGBIS4 light);

    static void remove_light(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, const Vec3<i64>& pos);

    static void update_light(const VoxelBlockWorld& voxelWorld, const VoxelHeightWorld& heightWorld, VoxelLightWorld& voxelLightWorld, const Vec3<i64>& pos);

    static void apply_light(const VoxelBlockWorld& voxelWorld, const VoxelHeightWorld& heightWorld, VoxelLightWorld& lightWorld, const VoxelBlockChunk& chunk, const VoxelHeightChunk& heightChunk, VoxelLightChunk& voxelLightChunk);
};

#endif