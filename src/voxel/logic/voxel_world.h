#ifndef VOXEL_WORLD_H
#define VOXEL_WORLD_H

#include "voxel_chunk.h"

#include "../../blocks/blocks.h"

template<typename ChunkType>
class VoxelWorld {
public:
    using chunk_type = ChunkType;

    ChunkType* chunks;
    Vec3<u64> size;

    VoxelWorld() {

    }

    ~VoxelWorld() {

    }

    VoxelWorld(Vec3<u64>& _size) :
    size(_size),
    chunks((ChunkType*)malloc(sizeof(ChunkType) * _size.volume())) {

    }

    u64 getChunkIndex(u64 x, u64 y, u64 z) const {
        return z + (x * size.z) + (y * size.x * size.z);
    }

    u64 chunkPosToChunkIndex(u64 x, u64 y, u64 z) const {
        i64 chunk_pos_x = (i64(size.x) / 2) + x;
        i64 chunk_pos_y = (i64(size.y) / 2) + y;
        i64 chunk_pos_z = (i64(size.z) / 2) + z;
        return chunk_pos_z + (chunk_pos_x * size.z) + (chunk_pos_y * size.x * size.z);
    }
};



namespace VoxelWorlds {

template<typename VoxelWorldType, typename VoxelType = VoxelWorldType::chunk_type::voxel_type, typename ChunkType = VoxelWorldType::chunk_type>
bool getVoxel(const VoxelWorldType& world, i64 x, i64 y, i64 z, VoxelType** voxel_ptr) {
    i64 chunk_pos_x = (i64(world.size.x) / 2) + floor(f64(x) / f64(CS));
    i64 chunk_pos_y = (i64(world.size.y) / 2) + floor(f64(y) / f64(CS));
    i64 chunk_pos_z = (i64(world.size.z) / 2) + floor(f64(z) / f64(CS));

    if (chunk_pos_x < 0 || chunk_pos_y < 0 || chunk_pos_z < 0) {
        return false;
    }
    
    if (chunk_pos_x >= world.size.x || chunk_pos_y >= world.size.y || chunk_pos_z >= world.size.z) {
        return false;
    }

    u64 chunk_index = world.getChunkIndex(chunk_pos_x, chunk_pos_y, chunk_pos_z);
    
    *voxel_ptr = &world.chunks[chunk_index].voxels[get_zxy_index(((x % CS) + CS) % CS, ((y % CS) + CS) % CS, ((z % CS) + CS) % CS)];

    return true;
}

template<typename VoxelWorldType, typename VoxelType = VoxelWorldType::chunk_type::voxel_type>
VoxelType* getVoxelUnsafe(const VoxelWorldType& world, i64 x, i64 y, i64 z) {
    VoxelType* voxel;
    if (!getVoxel(world, x, y, z, &voxel)) {
        throw std::runtime_error("getVoxelUnsafe: voxel not found");
    }

    return voxel;
}

template<typename VoxelWorldType, typename VoxelType = VoxelWorldType::chunk_type::voxel_type>
bool placeVoxel(const VoxelWorldType& world, i64 x, i64 y, i64 z, VoxelType voxelToPlace) {
    VoxelType* voxel;
    if (!getVoxel(world, x, y, z, &voxel)) {
        return false;
    }

    *voxel = voxelToPlace;
    
    return true;
}

template<typename VoxelChunkType, typename VoxelType = VoxelChunkType::voxel_type>
constexpr void calculateHeightmap(const VoxelChunkType& chunk, u8* heightmap) {
    bool found_block;
    for (u8 x = 0; x < CS; x++) {
        for (u8 z = 0; z < CS; z++) {
            found_block = false;
            heightmap[x + (z * CS)] = CS;
            for (u8 y = CS - 1; y >= 0; y--) {
                if (BLOCK_VOXEL_DATA[chunk.voxels[get_zxy_index(x, y, z)].type].transparent == true) {
                    heightmap[x + (z * CS)] = y;
                    break;
                }
            }
            if (!found_block) {
                heightmap[x + (z * CS)] = 0;
            }
        }
    }
}

inline u8 heightAt(const u8* heightmap, u8 x, u8 z) {
    return heightmap[x + (z * CS)];
}

template<typename VoxelWorldType>
inline u8 heightAtWorld(const VoxelWorldType& world, u8 x, u8 z) {
    return world.heightmap[x + (z * CS)];
}

}




#endif