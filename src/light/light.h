#ifndef LIGHT_H
#define LIGHT_H

#include "../core/maths.h"
#include "../../core/color.h"
#include "../voxel/light/voxel_light_world.h"

class ChunkLight {
    static void propagate(const VoxelGameWorld& voxelWorld, const VoxelChunk& thisChunk, VoxelLightChunk& lightChunk, Vec3<i64>& pos, RGBI4 color) {
        std::queue<std::tuple<int, int, int, uint8_t>> lightQueue;

        // Initialize light sources
        for (u8 x = 0; x < CS; x++) {
            for (u8 y = 0; y < CS; y++) {
                for (u8 z = 0; z < CS; z++) {
                    EmbeddedVoxel thisVoxel = thisChunk.voxels[get_zxy_index(x, y, z)];
                    BlockVoxelData thisBlock = block_voxel_datas[thisVoxel.type];
                    if (thisBlock.can_emit_light) {
                        RGBA4 lightColor = thisBlock.get
                        lightChunk.voxels[get_zxy_index(x, y, z)].light = thisChunk.voxels[get_zxy_index(x, y, z)].emission;
                        lightQueue.push({x, y, z, world[x][y][z].emission});
                    }
                }
            }
        }

        // BFS Light Propagation
        while (!lightQueue.empty()) {
            auto [x, y, z, lightLevel] = lightQueue.front();
            lightQueue.pop();

            if (lightLevel <= 1) continue;

            for (const auto& dir : directions) {
                int nx = x + dir.x;
                int ny = y + dir.y;
                int nz = z + dir.z;

                if (!inBounds(nx, ny, nz)) continue;
                if (world[nx][ny][nz].solid) continue;

                uint8_t newLevel = lightLevel - 1;
                if (world[nx][ny][nz].light < newLevel) {
                    world[nx][ny][nz].light = newLevel;
                    lightQueue.push({nx, ny, nz, newLevel});
                }
            }
        }
    }
};

#endif