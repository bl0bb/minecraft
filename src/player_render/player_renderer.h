#ifndef PLAYER_RENDERER_H
#define PLAYER_RENDERER_H

#include <glad/glad.h>

#include "../core/types.h"
#include "../core/shader.h"

class PlayerRenderer {
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


    void updateMesh() const {
        #if GL_API == 0
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, voxel_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, CS_P3 * sizeof(VoxelFace), voxel_faces, GL_STATIC_DRAW);
        #elif GL_API == 1
        glBindVertexArray(voxel_data_vao);
        glBindBuffer(GL_ARRAY_BUFFER, voxel_data_vbo);

        VoxelFace data[voxel_count * 6];
        for (u32 i = 0; i < voxel_count; i++) {
            for (u32 j = 0; j < 6; j++) {
                data[i * 6 + j] = voxel_faces[i];
            }
        }

        glBufferData(GL_ARRAY_BUFFER, voxel_count * 6 * sizeof(VoxelFace), data, GL_STATIC_DRAW);

        u64 dataPtr = 0;
        u16 data_row_size = (3 * sizeof(u32)) + (3 * sizeof(f32)) + (2 * sizeof(f32)) + (3 * sizeof(f32)) + (1 * sizeof(u32)) + (2 * sizeof(u32)) + (2 * sizeof(u32)) + (1 * sizeof(u32)) + (1 * sizeof(u32)) + (3 * sizeof(u32)) + (3 * sizeof(u32)) + (3 * sizeof(u32)) + (1 * sizeof(u32));

        // data_pos
        glVertexAttribIPointer(0, 3, GL_UNSIGNED_INT, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(0);
        dataPtr += (3 * sizeof(u32));

        // data_face_pos
        glVertexAttribPointer(1, 3, GL_FLOAT, false, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(1);
        dataPtr += (3 * sizeof(f32));

        // data_face_size
        glVertexAttribPointer(2, 2, GL_FLOAT, false, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(2);
        dataPtr += (2 * sizeof(f32));

        // data_face_rot
        glVertexAttribPointer(3, 3, GL_FLOAT, false, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(3);
        dataPtr += (3 * sizeof(f32));

        // data_uv_rot
        glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(4);
        dataPtr += (1 * sizeof(u32));

        // data_uv_pos
        glVertexAttribIPointer(5, 2, GL_UNSIGNED_INT, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(5);
        dataPtr += (2 * sizeof(u32));

        // data_uv_size
        glVertexAttribIPointer(6, 2, GL_UNSIGNED_INT, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(6);
        dataPtr += (2 * sizeof(u32));

        // data_dir
        glVertexAttribIPointer(7, 1, GL_UNSIGNED_INT, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(7);
        dataPtr += (1 * sizeof(u32));

        // data_type
        glVertexAttribIPointer(8, 1, GL_UNSIGNED_INT, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(8);
        dataPtr += (1 * sizeof(u32));

        // data_light_0_1_2
        glVertexAttribIPointer(9, 3, GL_UNSIGNED_INT, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(9);
        dataPtr += (3 * sizeof(u32));

        // data_light_3_4_5
        glVertexAttribIPointer(10, 3, GL_UNSIGNED_INT, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(10);
        dataPtr += (3 * sizeof(u32));

        // data_light_6_7_8
        glVertexAttribIPointer(11, 3, GL_UNSIGNED_INT, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(11);
        dataPtr += (3 * sizeof(u32));

        // data_ao
        glVertexAttribIPointer(12, 1, GL_UNSIGNED_INT, data_row_size, (void*)dataPtr);
        glEnableVertexAttribArray(12);
        dataPtr += (1 * sizeof(u32));
        #elif GL_API == 2
        // TODO
        #endif
    }

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