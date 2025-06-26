#ifndef BLOCK_OUTLINE_RENDERER_H
#define BLOCK_OUTLINE_RENDERER_H

#include "../../core/maths.h"
#include "../../core/array.h"
#include "../../core/shader.h"
#include <glad/glad.h>

class BlockOutlineRenderer {
public:
    Vec3<i64> pos;

    GLuint vao, vbo;

    Shader shader;

    BlockOutlineRenderer() {};

    void init();

    void render(f32 proj_mat[16], f32 view_mat[16]);
};

#endif