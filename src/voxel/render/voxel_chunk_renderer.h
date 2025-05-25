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

    #if GL_API == 0
    // voxel faces
    GLuint vao, vbo, ebo;
    GLuint voxel_ssbo;
    #elif GL_API == 1
    // TODO
    #endif

    VoxelChunkRenderer() : voxel_faces(new VoxelFace[CS_P3]()) {

    }

    void init() {
        #if GL_API == 0
        // Define your cube vertices (positions only for simplicity)
        f32 cubeVertices[] = {
            0, 0, 0,
            1, 0, 0,
            1, 1, 0,
            0, 1, 0,
        };

        // Define indices for the cube (using an IBO)
        unsigned int cubeIndices[] = {
            2, 1, 0,
            0, 3, 2,
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
        #elif GL_API == 1
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
        // TODO
        #endif
    }

    void sortWithin(const Vec3<u8>& pos) {
        ChunkMeshSortFace* faces = new ChunkMeshSortFace[voxel_count];

        for (u32 i = 0; i < voxel_count; i++) {
            VoxelFace face = voxel_faces[i];

            f64 dist = (Vec3<i64>(pos) - Vec3<i64>(face.getX(), face.getY(), face.getZ())).magnitude();

            faces[i] = ChunkMeshSortFace(dist, face);
        }

        std::sort(faces, faces + voxel_count, [&pos](const ChunkMeshSortFace& a, const ChunkMeshSortFace& b) {
            // f64 aDist = (pos - Vec3<u8>(a.getX(), a.getY(), a.getZ())).magnitude();
            // f64 bDist = (pos - Vec3<u8>(b.getX(), b.getY(), b.getZ())).magnitude();
            // return aDist > bDist;

            // Vec3<u8> aPos = pos - Vec3<u8>(a.getX(), a.getY(), a.getZ());
            // Vec3<u8> bPos = pos - Vec3<u8>(b.getX(), b.getY(), b.getZ());
            // u8 aDist = std::min(std::abs(aPos.x), std::min(std::abs(aPos.y), std::abs(aPos.z)));
            // u8 bDist = std::min(std::abs(bPos.x), std::min(std::abs(bPos.y), std::abs(bPos.z)));
            // return aDist > bDist;

            // Vec3<u8> aPos = (pos - Vec3<u8>(a.getX(), a.getY(), a.getZ())).abs();
            // Vec3<u8> bPos = (pos - Vec3<u8>(b.getX(), b.getY(), b.getZ())).abs();
            // return bPos.hasSmallerDim(aPos);

            return a.dist > b.dist;
        });

        for (u32 i = 0; i < voxel_count; i++) {
            voxel_faces[i] = faces[i].face;
        }

        updateMesh();
    }

    void sortNeighbor(u8 dir) {
        std::sort(voxel_faces, voxel_faces + voxel_count, [dir](const VoxelFace& a, const VoxelFace& b) {
            u8 aPos;
            u8 bPos;
            if (dir & 0b100) {
                // z
                aPos = a.getZ();
                bPos = b.getZ();
            } else if (dir & 0b010) {
                // y
                aPos = a.getY();
                bPos = b.getY();
            } else {
                // x
                aPos = a.getX();
                bPos = b.getX();
            }
            return aPos < bPos == (dir & 1);
            // if (dir & 1) {
            //     return bPos < aPos;
            // }
            // return aPos < bPos;
        });
    }

    void render(Shader& shaderProgram) const {
        #if GL_API == 0
        // Bind VAO and draw
        glBindVertexArray(vao);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, voxel_ssbo);

        shaderProgram.setIVec3("chunk_pos", chunk->pos);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, voxel_count);
        glBindVertexArray(0);
        #elif GL_API == 1
        // TODO
        #endif
    }
};

#endif