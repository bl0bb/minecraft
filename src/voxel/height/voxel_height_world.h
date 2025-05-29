#ifndef VOXEL_HEIGHT_WORLD_H
#define VOXEL_HEIGHT_WORLD_H

#include "../../core/types.h"
#include "../../blocks/mesh/block_mesh.h"
#include "../logic/voxel_chunk.h"
#include "../blockstate/voxel_blockstate_world.h"

class VoxelHeightChunk {
public:
    // static
    static inline u16 getXZIndex(u8 x, u8 z) {
        return x + (z * CS);
    }

    // class
    Vec2<i64> pos;
    u8* heightmap;

    VoxelHeightChunk() : heightmap((u8*)malloc(sizeof(u8) * CS_2)) {

    }

    inline u8 heightAt(u8 x, u8 z) const {
        return heightmap[getXZIndex(x, z)];
    }

    void calculateHeightmap(const VoxelBlockWorld& world, const VoxelBlockStateWorld& stateWorld) {
        const auto& chunk = world.chunks[world.chunkPosToChunkIndex(pos.x, 0, pos.y)];
        const auto& stateChunk = stateWorld.chunks[stateWorld.chunkPosToChunkIndex(pos.x, 0, pos.y)];

        bool found_block;
        for (u8 x = 0; x < CS; x++) {
            for (u8 z = 0; z < CS; z++) {
                found_block = false;
                heightmap[getXZIndex(x, z)] = CS;
                for (u8 y = CS - 1; y >= 0; y--) {
                    BlockVoxelData blockData = BLOCK_VOXEL_DATA[chunk.voxels[get_zxy_index(x, y, z)].type];
                    if (blockData.transparent) {
                        continue;
                    }

                    BlockStateStruct* state = stateChunk.voxels[get_zxy_index(x, y, z)].state;
                    BlockMesh blockMesh = BLOCK_MESHES[blockData.meshType](*state);

                    if (blockMesh.culls(2) || blockMesh.culls(3)) {
                        found_block = true;
                        heightmap[getXZIndex(x, z)] = y;
                        break;
                    }
                }
                if (!found_block) {
                    heightmap[getXZIndex(x, z)] = 0;
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

    inline Vec2<i64> worldToChunkPos(i64 x, i64 z) const {
        return chunkPosToChunkPosIndex(floor(f64(x) / f64(CS)), floor(f64(z) / f64(CS)));
    }

    inline u64 worldToChunkIndex(i64 x, i64 z) const {
        Vec2<i64> chunkPos = worldToChunkPos(x, z);
        return getChunkIndex(chunkPos.x, chunkPos.y);
    }

    inline i64 heightAt(i64 x, i64 z) const {
        Vec2<i64> chunkPos = worldToChunkPos(x, z);
        return chunks[getChunkIndex(chunkPos.x, chunkPos.y)].heightAt(((x % CS) + CS) % CS, ((z % CS) + CS) % CS);
    }

    inline bool isChunkPosIndexInWorld(const Vec2<i64>& pos) const {
        return (pos.x >= 0 && pos.x < size.x) && (pos.y >= 0 && pos.y < size.y);
    }

    inline bool isChunkPosInWorld(const Vec2<i64>& pos) const {
        return isChunkPosIndexInWorld(chunkPosToChunkPosIndex(pos.x, pos.y));
    }
};

#endif