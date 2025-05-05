#ifndef VOXEL_WORLD_RENDERER_H
#define VOXEL_WORLD_RENDERER_H

#include "../logic/voxel_world.h"
#include "voxel_chunk_renderer.h"

class VoxelWorldRenderer : public VoxelWorld<VoxelChunkRenderer> {
public:
    using VoxelWorld<VoxelChunkRenderer>::VoxelWorld;

    void render(Shader& shaderProgram) {
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