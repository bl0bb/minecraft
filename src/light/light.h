#ifndef LIGHT_H
#define LIGHT_H

#include <queue>

#include "../core/maths.h"
#include "../../core/color.h"
#include "../voxel/logic/voxel_game_world.h"
#include "../voxel/light/voxel_light_world.h"

// Directions
const Vec3<i8> directions[6] = {
    { 1,  0,  0},
    {-1,  0,  0},
    { 0,  1,  0},
    { 0, -1,  0},
    { 0,  0,  1},
    { 0,  0, -1},
};

typedef std::queue<std::tuple<Vec3<i64>, RGBI4>> LightQueue;

class ChunkLight {
    enum LightPropagationType : u8 {
        DEFAULT_LIGHT,
        SUN_LIGHT,
    };

    static void propagate(const VoxelGameWorld& voxelWorld, const VoxelLightWorld& voxelLightWorld, LightQueue& lightQueue) {
        // BFS Light Propagation
        while (!lightQueue.empty()) {
            auto [x, y, z, lightColor] = lightQueue.front();
            lightQueue.pop();
    
            for (const auto& dir : directions) {
                i64 nx = x + dir.x;
                i64 ny = y + dir.y;
                i64 nz = z + dir.z;
    
                VoxelGameWorld::chunk_type::voxel_type* worldVoxel;
                if (!VoxelWorlds::getVoxel(voxelWorld, x, y, z, &worldVoxel)) {
                    continue;
                }
    
                RGBI4 newLightColor = Colors::reduceOne(lightColor);
    
                // Only update if this is stronger than current light
                VoxelLightWorld::chunk_type::voxel_type* lightVoxel;
                if (VoxelWorlds::getVoxel(voxelLightWorld, nx, ny, nz, &lightVoxel) < newLightColor) {
                    *lightVoxel = newLightColor;
                    lightQueue.push({nx, ny, nz, newLightColor});
                }
            }
        }
    }

    static void calculate_light(const VoxelGameWorld& voxelWorld, ) {
        LightQueue lightQueue;

        // check each block for light sources
        for (u8 x = 0; x < CS; x++) {
            for (u8 y = 0; y < CS; y++) {
                for (u8 z = 0; z < CS; z++) {
                    BlockVoxelData thisBlock = block_voxel_datas[thisChunk.voxels[get_zxy_index(x, y, z)].type];
                    if (thisBlock.can_emit_light) {
                        lightQueue.push({x, y, z, thisBlock.get_light()});
                    }
                }
            }
        }
    }

    static void add_propagate(const VoxelGameWorld& voxelWorld, LightQueue& lightQueue, u16 mask, u32 offset, LightPropagationType type) {
        while (!lightQueue.empty()) {
            auto [pos, lightColor] = lightQueue.front();
            lightQueue.pop();

            RGBI4 light = world_get_light(voxelWorld, pos);
            u8 val = (light & mask) >> offset;

            // propagate in reverse of enum Direction order so DOWN is first
            // this will improve sunlight propagation speed
            for ()
        }
    }

    static void add_channel(const VoxelGameWorld& voxelWorld, Vec3<i64>& pos, u8 value, u16 mask, u32 offset, LightPropagationType type) {
        LightQueue lightQueue;
        world_set_light(voxelWorld, pos, (world_get_light(world, pos) & ~mask) | (((u32)value) << offset));
        lightQueue.push({pos});
        add_propagate(world, lightQueue, mask, offset, type);
    }

    static void add_light(const VoxelGameWorld& voxelWorld, Vec3<i64>& pos, RGBI4 light) {
        for (u8 i = 0; i < 4; i++) {
            u8 offset = i * 4;
            u16 mask = 0xF << offset;
            add_channel(world, pos, (light & mask) >> offset, mask, offset, DEFAULT_LIGHT)
        }
    }
};

#endif