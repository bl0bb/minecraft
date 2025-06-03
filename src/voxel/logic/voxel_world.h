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

    constexpr u64 getChunkIndex(u64 x, u64 y, u64 z) const {
        return z + (x * size.z) + (y * size.x * size.z);
    }

    constexpr u64 chunkPosToChunkIndex(i64 x, i64 y, i64 z) const {
        i64 chunk_pos_x = (size.x / 2) + x;
        i64 chunk_pos_y = (size.y / 2) + y;
        i64 chunk_pos_z = (size.z / 2) + z;
        return chunk_pos_z + (chunk_pos_x * size.z) + (chunk_pos_y * size.x * size.z);
    }

    constexpr u64 chunkPosToChunkIndex(const Vec3<i64>& pos) const {
        return chunkPosToChunkIndex(pos.x, pos.y, pos.z);
    }

    inline Vec3<i64> worldToChunkPos(const Vec3<i64>& pos) const {
        Vec3<i64> result = pos / CS;
        if (pos.x < 0) result.x--;
        if (pos.y < 0) result.y--;
        if (pos.z < 0) result.z--;
        return result;
    }

    constexpr bool isChunkPosIndexInWorld(const Vec3<i64>& pos) {
        return (pos.x >= 0 && pos.x < size.x) && (pos.y >= 0 && pos.y < size.y) && (pos.z >= 0 && pos.z < size.z);
    }

    constexpr bool isChunkPosInWorld(const Vec3<i64>& pos) {
        return isChunkPosIndexInWorld(chunkPosToChunkIndex(pos.x, pos.y, pos.z));
    }
};



namespace VoxelWorlds {

template<typename T>
constexpr bool isInChunkBounds(const Vec3<T>& pos) {
    return (pos.x >= 0 && pos.x < CS) && (pos.y >= 0 && pos.y < CS) && (pos.z >= 0 && pos.z < CS);
}

template<typename VoxelWorldType, typename VoxelType = VoxelWorldType::chunk_type::voxel_type, typename ChunkType = VoxelWorldType::chunk_type>
constexpr bool getVoxel(const VoxelWorldType& world, i64 x, i64 y, i64 z, VoxelType** voxel_ptr) {
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
constexpr VoxelType* getVoxelUnsafe(const VoxelWorldType& world, i64 x, i64 y, i64 z) {
    VoxelType* voxel;
    if (!getVoxel(world, x, y, z, &voxel)) {
        throw std::runtime_error("getVoxelUnsafe: voxel not found");
    }

    return voxel;
}

template<typename VoxelWorldType, typename VoxelType = VoxelWorldType::chunk_type::voxel_type>
constexpr bool placeVoxel(const VoxelWorldType& world, i64 x, i64 y, i64 z, VoxelType voxelToPlace) {
    VoxelType* voxel;
    if (!getVoxel(world, x, y, z, &voxel)) {
        return false;
    }

    *voxel = voxelToPlace;
    
    return true;
}

template<typename T>
constexpr Vec3<T> addDirToVec(Vec3<T> vec, u8 dir) {
    Vec3<T> newVec = vec;
    if (dir == 0) newVec.x++;
    else if (dir == 1) newVec.x--;
    else if (dir == 2) newVec.y++;
    else if (dir == 3) newVec.y--;
    else if (dir == 4) newVec.z++;
    else if (dir == 5) newVec.z--;
    return newVec;
}

}




#endif