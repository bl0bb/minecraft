#ifndef OBJECT_3D_H
#define OBJECT_3D_H

#include "../core/types.h"
#include "../core/maths.h"

typedef struct {
    mat4 transform;
    
    u16 vertices_count;
    f32* vertices;
    
    u16 indices_count;
    u16* indices;
    
    // shader for rendering this object
    GLuint shader_program;

    // texture
    GLuint texture;

    // bindings
    GLuint VAO; // Vertex Array Object
    GLuint VBO; // Vertex Buffer Object
    GLuint EBO; // Element Buffer Object
} object_3d;

#endif