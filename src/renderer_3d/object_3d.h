#ifndef OBJECT_3D_H
#define OBJECT_3D_H

#include "../core/types.h"
#include "../core/maths.h"

struct Object3D {
    Mat4<f64> transform;
    
    std::vector<f32> vertices;
    
    std::vector<u16> indices;
    
    // shader for rendering this object
    GLuint shader_program;

    // texture
    GLuint texture;

    // bindings
    GLuint VAO; // Vertex Array Object
    GLuint VBO; // Vertex Buffer Object
    GLuint EBO; // Element Buffer Object
};

#endif