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
    // voxel faces
    GLuint vao, vbo, ebo;

    #if GL_API == 0
    GLuint voxel_ssbo;
    #elif GL_API == 1
    GLuint texture_ubo;

    GLuint voxel_data1_ubo;
    GLuint voxel_data2_ubo;
    GLuint voxel_data3_ubo;
    GLuint voxel_data4_ubo;
    GLuint voxel_data5_ubo;
    GLuint voxel_data6_ubo;
    GLuint voxel_data7_ubo;
    GLuint voxel_data8_ubo;
    #endif

    #elif GL_API == 2
    // TODO
    #endif

    VoxelChunkRenderer() : voxel_faces(new VoxelFace[CS_P3]()) {

    }

    void init() {
        #if GL_API == 0 || GL_API == 1
        // Define your quad vertices (positions only for simplicity)
        f32 quadVertices[] = {
            0, 0, 0,
            1, 0, 0,
            1, 1, 0,
            0, 1, 0,
        };

        // Define indices for the quad (using an IBO)
        unsigned int quadIndices[] = {
            2, 1, 0,
            0, 3, 2,
        };

        // Create VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create VBO for quad vertices
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        // Create EBO for quad indices
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

        // Setup vertex attribute for positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind VAO
        glBindVertexArray(0);

        // Create SSBO for quad data
        #if GL_API == 0
        glGenBuffers(1, &voxel_ssbo);
        #elif GL_API == 1
        auto loadData = [](GLuint ubo) {
            glGenBuffers(1, &ubo);
        };

        loadData(voxel_data1_ubo);
        loadData(voxel_data2_ubo);
        loadData(voxel_data3_ubo);
        loadData(voxel_data4_ubo);
        loadData(voxel_data5_ubo);
        loadData(voxel_data6_ubo);
        loadData(voxel_data7_ubo);
        loadData(voxel_data8_ubo);
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
        // data1,2,3,4 is 64 bits, split into 2 u32
        // u32 data[8][voxel_count];
        u32** data = (u32**)malloc(sizeof(u32*) * 8);
        for (u8 i = 0; i < 8; i++) {
            data[i] = (u32*)malloc(sizeof(u32) * voxel_count);
        }
        printf("fluh 1 %i\n", voxel_count);
        for (u32 i = 0; i < voxel_count; i++) {
            if (i == 2372) {
                printf("BREAK %i %i\n", voxel_count, voxel_count - 1);
                break;
            }
            VoxelFace face = voxel_faces[i];
            for (u8 j = 0; j < 8; j++) {
                u8 shift = (j & 1) ? 32 : 0;
                u8 data_i = j >> 1;

                u64 dataBlock;
                if (data_i == 0) dataBlock = face.data1;
                else if (data_i == 1) dataBlock = face.data2;
                else if (data_i == 2) dataBlock = face.data3;
                else dataBlock = face.data4;

                dataBlock = (dataBlock >> shift) & ((u64(1) << 32) - 1);
                data[j][i] = dataBlock;
            }
        }
        printf("fluh 2\n");

        printf("fluh 3 %i\n", sizeof(VoxelFace));
        auto loadData = [this, data](u8 i, GLuint ubo) {
            glBindBuffer(GL_UNIFORM_BUFFER, ubo);
            glBufferData(GL_UNIFORM_BUFFER, voxel_count * sizeof(VoxelFace), data[i], GL_STATIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
        };

        loadData(0, voxel_data1_ubo);
        loadData(1, voxel_data2_ubo);
        loadData(2, voxel_data3_ubo);
        loadData(3, voxel_data4_ubo);
        loadData(4, voxel_data5_ubo);
        loadData(5, voxel_data6_ubo);
        loadData(6, voxel_data7_ubo);
        loadData(7, voxel_data8_ubo);
        printf("fluh 4\n");

        for (u8 i = 0; i < 8; i++) {
            free(data[i]);
        }
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
        // Bind VAO and draw
        glBindVertexArray(vao);

        #if GL_API == 0
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, voxel_ssbo);
        #elif GL_API == 1
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, voxel_data1_ubo);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, voxel_data2_ubo);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, voxel_data3_ubo);
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, voxel_data4_ubo);
        glBindBufferBase(GL_UNIFORM_BUFFER, 4, voxel_data5_ubo);
        glBindBufferBase(GL_UNIFORM_BUFFER, 5, voxel_data6_ubo);
        glBindBufferBase(GL_UNIFORM_BUFFER, 6, voxel_data7_ubo);
        glBindBufferBase(GL_UNIFORM_BUFFER, 7, voxel_data8_ubo);
        #endif

        shaderProgram.setIVec3("chunk_pos", chunk->pos);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, voxel_count);
        glBindVertexArray(0);
        #elif GL_API == 2
        // TODO
        #endif
    }
};

#endif