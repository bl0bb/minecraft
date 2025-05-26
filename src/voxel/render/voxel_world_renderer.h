#ifndef VOXEL_WORLD_RENDERER_H
#define VOXEL_WORLD_RENDERER_H

#include "../logic/voxel_world.h"
#include "voxel_chunk_renderer.h"

class VoxelWorldRenderer : public VoxelWorld<VoxelChunkRenderer> {
public:
    u64* chunkOrder;

    VoxelWorldRenderer(Vec3<u64>& _size) : VoxelWorld<VoxelChunkRenderer>(_size), chunkOrder((u64*)malloc(sizeof(u64) * size.volume())) {
        for (u64 i = 0; i < size.volume(); i++) {
            chunkOrder[i] = i;
        }
    }

    void sortChunks(const Vec3<i64>& pos) const {
        std::sort(chunkOrder, chunkOrder + size.volume(), [this, &pos](const u64& aIdx, const u64& bIdx) {
            const VoxelChunkRenderer& a = chunks[aIdx];
            const VoxelChunkRenderer& b = chunks[bIdx];
            f64 aDist = (pos - a.chunk->pos).magnitude();
            f64 bDist = (pos - b.chunk->pos).magnitude();
            return aDist > bDist;

            // Vec3<i64> aPos = pos - a.chunk->pos;
            // Vec3<i64> bPos = pos - b.chunk->pos;
            // u8 aDist = std::min(std::abs(aPos.x), std::min(std::abs(aPos.y), std::abs(aPos.z)));
            // u8 bDist = std::min(std::abs(bPos.x), std::min(std::abs(bPos.y), std::abs(bPos.z)));
            // return aDist > bDist;
        });
    }

    void render(Shader& shaderProgram) const {
        for (u64 i = 0; i < size.volume(); i++) {
            u64 idx = chunkOrder[i];
            chunks[idx].render(shaderProgram);
        }
        // for (i64 x = 0; x < size.x; x++) {
        //     for (i64 y = 0; y < size.y; y++) {
        //         for (i64 z = 0; z < size.z; z++) {
        //             VoxelChunkRenderer& chunk = chunks[getChunkIndex(x, y, z)];
                    
        //             chunk.render(shaderProgram);
        //         }
        //     }
        // }
    }
};

#endif