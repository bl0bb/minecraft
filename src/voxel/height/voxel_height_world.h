#ifndef VOXEL_HEIGHT_WORLD_H
#define VOXEL_HEIGHT_WORLD_H

#include "../../core/types.h"
#include "../../blocks/mesh/block_mesh.h"
#include "../logic/voxel_chunk.h"
#include "../block/voxel_block_world.h"
#include "../blockstate/voxel_blockstate_world.h"

class VoxelHeightChunk {
public:
    // static
    static inline u16 getXZIndex(u8 x, u8 z) {
        return x + (z * CS);
    }

    // class
    Vec2<i64> pos;
    i64* heightmap;

    VoxelHeightChunk() : heightmap((i64*)malloc(sizeof(i64) * CS_2)) {

    }

    inline i64 heightAt(u8 x, u8 z) const {
        return heightmap[getXZIndex(x, z)];
    }

    inline void setHeightAt(u8 x, u8 z, i64 height) const {
        heightmap[getXZIndex(x, z)] = height;
    }

    void calculateHeightmap(const VoxelBlockWorld& world, const VoxelBlockStateWorld& stateWorld) {
        bool found_block;
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                found_block = false;
                heightmap[getXZIndex(x, z)] = world.size.y / 2 * CS - 1;

                for (i8 chunk_y = world.size.y - 1; chunk_y >= 0; chunk_y--) {
                    i8 chunk_y_world = world.size.y / 2 - 1;
                    const auto& chunk = world.chunks[world.chunkPosToChunkIndex(pos.x, chunk_y_world, pos.y)];
                    const auto& stateChunk = stateWorld.chunks[stateWorld.chunkPosToChunkIndex(pos.x, chunk_y_world, pos.y)];

                    for (i16 y = CS - 1; y >= 0; y--) {
                        BlockType blockType = chunk.voxels[get_zxy_index(x, y, z)].type;
                        BlockVoxelData blockData = BLOCK_VOXEL_DATA[blockType];
                        if (blockData.transparent) {
                            continue;
                        }

                        BlockStateStruct* state = stateChunk.voxels[get_zxy_index(x, y, z)].state;
                        BlockMesh blockMesh = BLOCK_MESHES[blockData.meshType](*state);

                        if (blockType != BlockTypes::AIR) { // blockMesh.culls(2) || blockMesh.culls(3)
                            found_block = true;
                            printf("fluh!!\n");
                            heightmap[getXZIndex(x, z)] = CS * chunk_y_world + y;
                            break;
                        }
                    }
                    if (found_block) {
                        break;
                    }
                }

                if (!found_block) {
                    heightmap[getXZIndex(x, z)] = -world.size.y / 2 * CS;
                }
            }
        }
    }
};

class VoxelHeightWorld {
public:
    Vec2<i64> size;

    VoxelHeightChunk* chunks;

    VoxelHeightWorld(Vec2<u64>& _size) :
    size(_size),
    chunks((VoxelHeightChunk*)malloc(sizeof(VoxelHeightChunk) * _size.volume())) {

    }

    inline u64 getChunkIndex(u64 x, u64 z) const {
        return z + (x * size.y);
    }

    inline Vec2<u64> chunkPosToChunkPosIndex(i64 x, i64 z) const {
        u64 chunk_pos_x = (size.x / 2) + x;
        u64 chunk_pos_z = (size.y / 2) + z;
        return {chunk_pos_x, chunk_pos_z};
    }

    inline u64 chunkPosToChunkIndex(u64 x, u64 z) const {
        Vec2<u64> pos = chunkPosToChunkPosIndex(x, z);
        return getChunkIndex(pos.x, pos.y);
    }

    inline Vec2<i64> worldToChunkPosIndex(i64 x, i64 z) const {
        Vec2<i64> chunkPos = worldToChunkPos(x, z);
        return chunkPosToChunkPosIndex(chunkPos.x, chunkPos.y);
    }

    inline Vec2<i64> worldToChunkPos(i64 x, i64 z) const {
        Vec2<i64> result = Vec2<i64>(x, z) / CS;
        if (x < 0) result.x--;
        if (z < 0) result.y--;
        return result;
    }

    inline u64 worldToChunkIndex(i64 x, i64 z) const {
        Vec2<i64> chunkPos = worldToChunkPosIndex(x, z);
        return getChunkIndex(chunkPos.x, chunkPos.y);
    }

    inline i64 heightAt(i64 x, i64 z) const {
        Vec2<i64> chunkPos = worldToChunkPosIndex(x, z);
        return chunks[getChunkIndex(chunkPos.x, chunkPos.y)].heightAt(((x % CS) + CS) % CS, ((z % CS) + CS) % CS);
    }

    inline bool isChunkPosIndexInWorld(const Vec2<i64>& pos) const {
        return (pos.x >= 0 && pos.x < size.x) && (pos.y >= 0 && pos.y < size.y);
    }

    inline bool isChunkPosInWorld(const Vec2<i64>& pos) const {
        return isChunkPosIndexInWorld(chunkPosToChunkPosIndex(pos.x, pos.y));
    }

    void updateHeightAtPos(const Vec3<i64>& modifyPos, const EmbeddedVoxel& block, const BlockStateVoxel& stateVoxel) {
        (void)stateVoxel;
        Vec2<i64> chunkPos = worldToChunkPos(modifyPos.x, modifyPos.z);
        Vec2<i64> chunkPosIndex = chunkPosToChunkPosIndex(chunkPos.x, chunkPos.y);
        if (!isChunkPosIndexInWorld(chunkPosIndex)) {
            return;
        }

        Vec2<i64> chunkBlockPos = Vec2<i64>(modifyPos.x, modifyPos.z) - chunkPos * CS;

        const auto& chunk = chunks[getChunkIndex(chunkPosIndex.x, chunkPosIndex.y)];

        i64 height = chunk.heightAt(chunkBlockPos.x, chunkBlockPos.y);
        if (modifyPos.y < height) {
            return;
        }

        // check if block blocks light
        BlockVoxelData blockData = BLOCK_VOXEL_DATA[block.type];
        if (blockData.transparent) {
            return;
        }

        chunk.setHeightAt(chunkBlockPos.x, chunkBlockPos.y, modifyPos.y);
        
        // BlockMesh blockMesh = BLOCK_MESHES[blockData.meshType](*stateVoxel.state);
        // if (blockMesh.culls(2) || blockMesh.culls(3)) {
        //     chunk.setHeightAt(chunkBlockPos.x, chunkBlockPos.y, modifyPos.y);
        // }
    }
};

#endif