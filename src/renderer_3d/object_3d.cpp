#include "object_3d.h"

void object_3d_free(const object_3d* object) {
    glDeleteVertexArrays(1, &object->VAO);
    glDeleteBuffers(1, &object->VBO);
}