#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H

#include <glad/glad.h>

#include "../../core/types.h"
#include "../../core/shader.h"
#include "../../entity/entity_model.h"
#include "entity_mesh.h"

class EntityRenderer {
public:
    #if GL_API == 0 || GL_API == 1

    #if GL_API == 0
    GLuint element_ssbo;
    GLuint face_ssbo;
    #elif GL_API == 1
    GLuint voxel_data_vao;
    GLuint voxel_data_vbo;
    #endif

    #elif GL_API == 2
    // TODO
    #endif

    EntityModel *entityModel;

    u32 faceCount;

    EntityRenderer() : faceCount(0), entityModel(nullptr) {

    }

    void init() {
        #if GL_API == 0 || GL_API == 1

        // Create SSBO for quad data
        #if GL_API == 0
        glGenBuffers(1, &element_ssbo);
        glGenBuffers(1, &face_ssbo);
        #elif GL_API == 1
        glGenVertexArrays(1, &voxel_data_vao);
        glGenBuffers(1, &voxel_data_vbo);
        #endif

        #elif GL_API == 2
        // TODO
        #endif
    }

    void updateMesh() {
        #if GL_API == 0
        faceCount = 0;
        for (u32 i = 0; i < entityModel->elementCount; i++) {
            faceCount += entityModel->elements[i].faceCount;
        }

        EntityMeshElement *elements = (EntityMeshElement *)malloc(entityModel->elementCount * sizeof(EntityMeshElement));
        EntityMeshFace *faces = (EntityMeshFace *)malloc(faceCount * sizeof(EntityMeshFace));
        u32 faceIdx = 0;
        for (u32 i = 0; i < entityModel->elementCount; i++) {
            EntityElement& element = entityModel->elements[i];
            // TODO
            elements[i] = EntityMeshElement(
                // pos
                i * 1, 10, 0,

                // rot
                0, 0, 0,

                // size
                element.size.x, element.size.y, element.size.z
            );
            for (u32 j = 0; j < element.faceCount; j++) {
                EntityElementFace& face = element.faces[j];
                faces[faceIdx++] = EntityMeshFace(
                    // element
                    i,

                    // dir
                    face.dir,

                    // uv
                    0,

                    face.uvFrom.x, face.uvFrom.y,
                    face.uvTo.x - face.uvFrom.x, face.uvTo.y - face.uvFrom.y
                );
            }
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, element_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, entityModel->elementCount * sizeof(EntityMeshElement), elements, GL_STATIC_DRAW);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, face_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, faceCount * sizeof(EntityMeshFace), faces, GL_STATIC_DRAW);

        free(elements);
        free(faces);
        #elif GL_API == 1
        // TODO: rework this, its just a copy paste with some parts removed
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
        u16 data_row_size = (3 * sizeof(u32)) + (3 * sizeof(f32)) + (2 * sizeof(f32)) + (3 * sizeof(f32)) + (1 * sizeof(u32)) + (2 * sizeof(u32)) + (2 * sizeof(u32));

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
        #elif GL_API == 2
        // TODO
        #endif
    }

    void render(Shader& shaderProgram) const {
        #if GL_API == 0 || GL_API == 1
        // draw
        #if GL_API == 0
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, element_ssbo);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, face_ssbo);
        #elif GL_API == 1
        glBindVertexArray(voxel_data_vao);
        glBindBuffer(GL_ARRAY_BUFFER, voxel_data_vbo);
        #endif

        glDrawArrays(GL_TRIANGLES, 0, faceCount * 6);
        #elif GL_API == 2
        // TODO
        #endif
    }
};

#endif