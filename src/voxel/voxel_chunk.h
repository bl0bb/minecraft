#ifndef VOXEL_CHUNK_H
#define VOXEL_CHUNK_H

#include <glad/glad.h>

#include <cstring>

#include "../core/types.h"
#include "../core/shader.h"

#include "voxel.h"


// CS = chunk size
// CS_P = chunk size padded
static constexpr u8 CS = 32;
static constexpr u16 CS_2 = CS * CS;
static constexpr u16 CS_3 = CS * CS * CS;
static constexpr u8 CS_P = CS + 2;
static constexpr u16 CS_P2 = CS_P * CS_P;
static constexpr u16 CS_P3 = CS_P * CS_P * CS_P;


// x = 0-4 (5) (32)
// y = 5-9 (5) (32)
// z = 10-14 (5) (32)
// w = 15-19 (5) (32)
// h = 20-24 (5) (32)
// dir = 25-37 (3) (8)
// type = 28-31 (4) (16)
typedef u64 VoxelFace;
constexpr inline VoxelFace getQuad(u64 x, u64 y, u64 z, u64 w, u64 h, u64 dir, u64 type) {
    return (type << 28) | (dir << 25) | (h << 20) | (w << 15) | (z << 10) | (y << 5) | x;
}



// helpers
constexpr inline int get_zxy_index_p(int x, int y, int z) {
    return z + (x * CS_P) + (y * CS_P2);
}
constexpr inline int get_zxy_index(int x, int y, int z) {
    return z + (x * CS) + (y * CS_2);
}

constexpr inline int getAxisIndex(const int axis, const int a, const int b, const int c) {
    if (axis == 0) return b + (a * CS_P) + (c * CS_P2);
    else if (axis == 1) return b + (c * CS_P) + (a * CS_P2);
    else return c + (a * CS_P) + (b * CS_P2);
}



class VoxelChunk {
public:
    Vec3<u64> pos;
    EmbeddedVoxel* voxels;
    u32 voxel_count;

    // voxel faces
    GLuint vao, vbo, ebo;
    GLuint voxel_ssbo;

    VoxelChunk() {
        voxels = new EmbeddedVoxel[CS_3]{0};
        std::memset(voxels, 0, CS_3);
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

    void updateMesh(VoxelFace* data) const {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, voxel_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, CS_P3 * sizeof(VoxelFace), data, GL_STATIC_DRAW);
    }

    void render(Shader& shaderProgram) const {
        // Bind VAO and draw
        glBindVertexArray(vao);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, voxel_ssbo);

        shaderProgram.setIVec3("chunk_pos", pos);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, voxel_count);
        glBindVertexArray(0);
    }
};

#endif