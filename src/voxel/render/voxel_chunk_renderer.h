#ifndef VOXEL_CHUNK_RENDERER_H
#define VOXEL_CHUNK_RENDERER_H

#include <glad/glad.h>

#include <cstring>

#include "../../core/types.h"
#include "../../core/shader.h"

#include "../logic/voxel_world.h"
#include "../logic/voxel_chunk.h"
#include "../logic/voxel.h"
#include "voxel_chunk_mesh_common.h"
#include "voxel_mesher.h"



class VoxelChunkRenderer {
public:
    VoxelChunk* chunk;
    
    u32 voxel_count;

    // voxel faces
    GLuint vao, vbo, ebo;
    GLuint voxel_ssbo;

    VoxelChunkRenderer() {

    }

    void init() {
        // Define your cube vertices (positions only for simplicity)
        float cubeVertices[] = {
            0, 0, 0,
            1, 0, 0,
            1, 1, 0,
            0, 1, 0,
        };

        // Define indices for the cube (using an IBO)
        unsigned int cubeIndices[] = {
            2, 1, 0, 0, 3, 2,
        };

        // Create VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create VBO for cube vertices
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        // Create EBO for cube indices
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

        // Setup vertex attribute for positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind VAO
        glBindVertexArray(0);

        // Create SSBO for instance positions
        glGenBuffers(1, &voxel_ssbo);
    }

    void generateMesh(const VoxelGameWorld& voxelWorld) {
        VoxelFace voxel_faces[CS_P3] = {0};
        voxel_count = generate_voxel_mesh(voxelWorld, *chunk, voxel_faces);
        updateMesh(voxel_faces);
    }

    void updateMesh(VoxelFace* data) const {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, voxel_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, CS_P3 * sizeof(VoxelFace), data, GL_STATIC_DRAW);
    }

    void render(Shader& shaderProgram) const {
        // Bind VAO and draw
        glBindVertexArray(vao);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, voxel_ssbo);

        shaderProgram.setIVec3("chunk_pos", chunk->pos);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, voxel_count);
        glBindVertexArray(0);
    }
};

#endif