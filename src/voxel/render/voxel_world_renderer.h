#ifndef VOXEL_WORLD_RENDERER_H
#define VOXEL_WORLD_RENDERER_H

#include "../logic/voxel_world.h"
#include "voxel_chunk_renderer.h"

class VoxelWorldRenderer : public VoxelWorld<VoxelChunkRenderer> {
public:
    void sortChunks(const Vec3<i64>& pos) const {
        std::sort(chunks, chunks + size.volume(), [&pos](const VoxelChunkRenderer& a, const VoxelChunkRenderer& b) {
            f64 aDist = (pos - a.chunk->pos).abs().magnitude();
            f64 bDist = (pos - b.chunk->pos).abs().magnitude();
            return aDist > bDist;

            // Vec3<i64> aPos = pos - a.chunk->pos;
            // Vec3<i64> bPos = pos - b.chunk->pos;
            // u8 aDist = std::min(std::abs(aPos.x), std::min(std::abs(aPos.y), std::abs(aPos.z)));
            // u8 bDist = std::min(std::abs(bPos.x), std::min(std::abs(bPos.y), std::abs(bPos.z)));
            // return aDist > bDist;
        });
    }

    void render(Shader& shaderProgram) const {
        for (i64 x = 0; x < size.x; x++) {
            for (i64 y = 0; y < size.y; y++) {
                for (i64 z = 0; z < size.z; z++) {
                    VoxelChunkRenderer& chunk = chunks[getChunkIndex(x, y, z)];
                    
                    chunk.render(shaderProgram);
                }
            }
        }
    }
};

#endif