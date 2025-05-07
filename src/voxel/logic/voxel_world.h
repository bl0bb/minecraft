#ifndef VOXEL_WORLD_H
#define VOXEL_WORLD_H

#include "voxel_chunk.h"

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

    VoxelWorld(Vec3<u64> _size) :
    size(_size) {

    }

    u64 getChunkIndex(u64 x, u64 y, u64 z) const {
        return z + (x * size.z) + (y * size.x * size.z);
    }
};



namespace VoxelWorlds {

template<typename VoxelWorldType>
typename VoxelWorldType::chunk_type::voxel_type getVoxel(const VoxelWorldType& world, u64 x, u64 y, u64 z) {
    // TODO: add transform to voxel_world

    u64 chunk_pos_x = x / CS;
    u64 chunk_pos_y = y / CS;
    u64 chunk_pos_z = z / CS;

    if (chunk_pos_x < 0 || chunk_pos_y < 0 || chunk_pos_z < 0) {
        return 0;
    }
    
    if (chunk_pos_x >= world.size.x || chunk_pos_y >= world.size.y || chunk_pos_z >= world.size.z) {
        return 0;
    }

    u64 chunk_index = world.getChunkIndex(chunk_pos_x, chunk_pos_y, chunk_pos_z);

    return world.chunks[chunk_index].voxels[get_zxy_index(x % CS, y % CS, z % CS)];
}

}




#endif