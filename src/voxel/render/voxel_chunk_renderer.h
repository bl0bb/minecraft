#ifndef VOXEL_CHUNK_RENDERER_H
#define VOXEL_CHUNK_RENDERER_H

#include <glad/glad.h>

#include <cstring>

#include "../../core/types.h"
#include "../../core/shader.h"

#include "../logic/voxel_world.h"
#include "../logic/voxel_chunk.h"
#include "voxel_chunk_mesh_common.h"
#include "voxel_mesher.h"



struct ChunkMeshSortFace {
    f64 dist;
    VoxelFace face;

    ChunkMeshSortFace() {}

    ChunkMeshSortFace(f64 _dist, VoxelFace _face) : dist(_dist), face(_face) {}
};

class VoxelChunkRenderer {
public:
    VoxelBlockChunk* chunk;

    VoxelFace* voxel_faces;
    u32 voxel_count;

    #if GL_API == 0 || GL_API == 1

    #if GL_API == 0
    GLuint voxel_ssbo;
    #elif GL_API == 1
    GLuint voxel_data_vao;
    GLuint voxel_data_vbo;
    #endif

    #elif GL_API == 2
    // TODO
    #endif

    VoxelChunkRenderer() : voxel_faces(new VoxelFace[CS_P3]()) {

    }

    void init() {
        #if GL_API == 0 || GL_API == 1

        // Create SSBO for quad data
        #if GL_API == 0
        glGenBuffers(1, &voxel_ssbo);
        #elif GL_API == 1
        glGenVertexArrays(1, &voxel_data_vao);
        glGenBuffers(1, &voxel_data_vbo);
        #endif

        #elif GL_API == 2
        // TODO
        #endif
    }

    void generateMesh(const VoxelBlockWorld& voxelWorld, const VoxelBlockStateWorld& voxelBlockStateWorld, const VoxelLightWorld& voxelLightWorld) {
        std::memset(voxel_faces, 0, CS_P3 * sizeof(VoxelFace));
        voxel_count = generate_voxel_mesh(voxelWorld, voxelBlockStateWorld, voxelLightWorld, chunk->pos, voxel_faces);
        updateMesh();
    }

    void updateMesh() const {
        #if GL_API == 0
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, voxel_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, CS_P3 * sizeof(VoxelFace), voxel_faces, GL_STATIC_DRAW);
        #elif GL_API == 1
        glBindVertexArray(voxel_data_vao);
        glBindBuffer(GL_ARRAY_BUFFER, voxel_data_vbo);

        VoxelFace* data = new VoxelFace[voxel_count * 6];
        for (u32 i = 0; i < voxel_count; i++) {
            for (u32 j = 0; j < 6; j++) {
                data[i * 6 + j] = voxel_faces[i];
            }
        }

        glBufferData(GL_ARRAY_BUFFER, voxel_count * 6 * 8 * sizeof(u32), data, GL_STATIC_DRAW);

        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 8 * sizeof(u32), (void*)(0 * sizeof(u32)));
        glEnableVertexAttribArray(0);
        glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 8 * sizeof(u32), (void*)(1 * sizeof(u32)));
        glEnableVertexAttribArray(1);
        glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 8 * sizeof(u32), (void*)(2 * sizeof(u32)));
        glEnableVertexAttribArray(2);
        glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, 8 * sizeof(u32), (void*)(3 * sizeof(u32)));
        glEnableVertexAttribArray(3);
        glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, 8 * sizeof(u32), (void*)(4 * sizeof(u32)));
        glEnableVertexAttribArray(4);
        glVertexAttribIPointer(5, 1, GL_UNSIGNED_INT, 8 * sizeof(u32), (void*)(5 * sizeof(u32)));
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(6, 1, GL_UNSIGNED_INT, 8 * sizeof(u32), (void*)(6 * sizeof(u32)));
        glEnableVertexAttribArray(6);
        glVertexAttribIPointer(7, 1, GL_UNSIGNED_INT, 8 * sizeof(u32), (void*)(7 * sizeof(u32)));
        glEnableVertexAttribArray(7);

        free(data);
        #elif GL_API == 2
        // TODO
        #endif
    }

    void sortWithin(const Vec3<i8>& pos) {
        ChunkMeshSortFace* faces = new ChunkMeshSortFace[voxel_count];

        for (u32 i = 0; i < voxel_count; i++) {
            VoxelFace face = voxel_faces[i];
            f64 dist = (Vec3<i64>(pos) - Vec3<i64>(face.getX(), face.getY(), face.getZ())).magnitude();
            faces[i] = ChunkMeshSortFace(dist, face);
        }

        std::sort(faces, faces + voxel_count, [&pos](const ChunkMeshSortFace& a, const ChunkMeshSortFace& b) {
            return a.dist > b.dist;
        });

        for (u32 i = 0; i < voxel_count; i++) {
            voxel_faces[i] = faces[i].face;
        }

        free(faces);

        updateMesh();
    }

    // TODO
    // void sortNeighbor(u8 xSort, u8 ySort, u8 zSort) {
    //     std::sort(voxel_faces, voxel_faces + voxel_count, [dir](const VoxelFace& a, const VoxelFace& b) {
    //         u8 aX = a.getX();
    //         u8 aY = a.getY();
    //         u8 aZ = a.getZ();

    //         u8 bX = b.getX();
    //         u8 bY = b.getY();
    //         u8 bZ = b.getZ();


    //     });
    // }

    void render(Shader& shaderProgram) const {
        #if GL_API == 0 || GL_API == 1
        // draw
        #if GL_API == 0
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, voxel_ssbo);
        #elif GL_API == 1
        glBindVertexArray(voxel_data_vao);
        glBindBuffer(GL_ARRAY_BUFFER, voxel_data_vbo);
        #endif

        shaderProgram.setIVec3("chunk_pos", chunk->pos);

        glDrawArrays(GL_TRIANGLES, 0, voxel_count * 6);
        #elif GL_API == 2
        // TODO
        #endif
    }
};

#endif