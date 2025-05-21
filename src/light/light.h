#ifndef LIGHT_H
#define LIGHT_H

#include <queue>

#include "../core/maths.h"
#include "../core/color.h"
#include "../voxel/block/voxel_block_world.h"
#include "../voxel/light/voxel_light_world.h"

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

typedef LightQueue<RGBIS4> BlockLightQueue;
typedef LightQueue<RGBIS4> AllLightQueue;

namespace ChunkLight {
    enum LightPropagationType : u8 {
        DEFAULT_LIGHT,
        SUN_LIGHT,
    };

    static void add_propagate(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, AllLightQueue& lightQueue, u32 mask, u32 offset, LightPropagationType type) {
        while (!lightQueue.empty()) {
            auto [pos, _] = lightQueue.front();
            lightQueue.pop();

            RGBIS4* light;
            if (!VoxelWorlds::getVoxel(voxelLightWorld, pos.x, pos.y, pos.z, &light)) {
                continue;
            }
            u8 val = (*light & mask) >> offset;

            // propagate in reverse of enum Direction order so DOWN is first
            // this will improve sunlight propagation speed
            for (u8 i = 0; i < 6; i++) {
                Vec3<i64> newPos = pos + directions[i];

                RGBIS4* newLight;
                if (!VoxelWorlds::getVoxel(voxelLightWorld, newPos.x, newPos.y, newPos.z, &newLight)) {
                    continue;
                }

                EmbeddedVoxel* voxel;
                if (!VoxelWorlds::getVoxel(voxelWorld, newPos.x, newPos.y, newPos.z, &voxel)) {
                    continue;
                }

                BlockVoxelData newBlock = BLOCK_VOXEL_DATA[voxel->type];

                bool sunlight_down = type == SUN_LIGHT && i == 0;

                if ((newLight != 0 || newBlock.transparent) && ((sunlight_down && *newLight < *light) || (*newLight + 1 < val))) {
                    // sunlight does not get dimmer as it propagates down
                    i8 delta = sunlight_down ? 0 : -1;

                    *newLight = (*newLight & ~mask) | ((((*light & mask) >> offset) + delta) << offset);
                    lightQueue.push({newPos, 0});
                }
            }
        }
    }

    static void remove_propagate(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, AllLightQueue& lightQueue, AllLightQueue& propQueue, u32 mask, u32 offset, LightPropagationType type) {
        while (!lightQueue.empty()) {
            auto [pos, value] = lightQueue.front();
            lightQueue.pop();

            for (u8 i = 0; i < 6; i++) {
                Vec3<i64> newPos = pos + directions[i];

                RGBIS4* newLight;
                if (!VoxelWorlds::getVoxel(voxelLightWorld, newPos.x, newPos.y, newPos.z, &newLight)) {
                    continue;
                }
                u8 newValue = (*newLight & mask) >> offset;

                bool sunlight_down = type == SUN_LIGHT && i == 0;

                if ((*newLight & mask) != 0 && (newValue < value || sunlight_down)) {
                    // sunlight does not get dimmer as it propagates down
                    i8 delta = sunlight_down ? 0 : -1;

                    *newLight = *newLight & ~mask;
                    lightQueue.push({newPos, newValue});
                } else if (newValue >= value) {
                    propQueue.push({newPos, 0});
                }
            }
        }
    }

    static void add_channel(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, Vec3<i64>& pos, u8 value, u32 mask, u32 offset, LightPropagationType type) {
        AllLightQueue lightQueue;

        RGBIS4* light;
        if (!VoxelWorlds::getVoxel(voxelLightWorld, pos.x, pos.y, pos.z, &light)) {
            return;
        }
        *light = (*light & ~mask) | (((u32)value) << offset);

        lightQueue.push({pos, 0});
        add_propagate(voxelWorld, voxelLightWorld, lightQueue, mask, offset, type);
    }

    static void remove_channel(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, Vec3<i64>& pos, u32 mask, u32 offset, LightPropagationType type) {
        AllLightQueue lightQueue;
        AllLightQueue propQueue;

        RGBIS4* light;
        if (!VoxelWorlds::getVoxel(voxelLightWorld, pos.x, pos.y, pos.z, &light)) {
            return;
        }
        RGBIS4 oldLight = *light;
        *light = (oldLight & ~mask);

        lightQueue.push({pos, (oldLight & mask) >> offset});
        remove_propagate(voxelWorld, voxelLightWorld, lightQueue, propQueue, mask, offset, type);
        add_propagate(voxelWorld, voxelLightWorld, lightQueue, mask, offset, type);
    }

    static void add_light(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, Vec3<i64>& pos, RGBIS4 light) {
        for (u8 i = 0; i < 4; i++) {
            u8 offset = i * 4;
            u32 mask = 0xF << offset;
            add_channel(voxelWorld, voxelLightWorld, pos, (light & mask) >> offset, mask, offset, DEFAULT_LIGHT);
        }
    }

    static void remove_light(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, Vec3<i64>& pos, RGBIS4 light) {
        for (u8 i = 0; i < 4; i++) {
            u8 offset = i * 4;
            u32 mask = 0xF << offset;
            remove_channel(voxelWorld, voxelLightWorld, pos, mask, offset, DEFAULT_LIGHT);
        }
    }

    static void update_light(const VoxelBlockWorld& voxelWorld, VoxelLightWorld& voxelLightWorld, Vec3<i64>& pos) {
        AllLightQueue lightQueue;

        // 0..4 for each channel
        // 0 = R
        // 1 = G
        // 2 = B
        // 3 = I
        // 4 = SUN
        for (u8 i = 0; i < 5; i++) {
            u8 offset = i * 4;
            u32 mask = 0xF << offset;

            bool sunlight = i == 4;
            
            for (u8 d = 0; d < 6; d++) {
                Vec3<i64> newPos = pos + directions[d];

                EmbeddedVoxel* voxel;
                if (!VoxelWorlds::getVoxel(voxelWorld, newPos.x, newPos.y, newPos.z, &voxel)) {
                    continue;
                }

                lightQueue.push({newPos, 0});
            }

            if (sunlight && pos.y > VoxelWorlds::heightAtWorld(voxelLightWorld, pos.x, pos.z)) {
                RGBIS4* light;
                if (VoxelWorlds::getVoxel(voxelLightWorld, pos.x, pos.y, pos.z, &light)) {
                    *light = Colors::COLOR4_MAX;
                    lightQueue.push({pos, 0});
                }
            }
        }
    }
};

#endif