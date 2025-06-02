#ifndef BLOCK_OUTLINE_RENDERER_H
#define BLOCK_OUTLINE_RENDERER_H

class BlockOutlineRenderer {
public:
    Vec3<i64> pos;

    GLuint vao, vbo;

    Shader shader;

    BlockOutlineRenderer() {};

    void init() {
        #if GL_API == 0 || GL_API == 1
        // mesh
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        f32 vertices[] = {
            // positions          // texture coords

            // right face
             1.0f,  1.0f,  1.0f,   0.0f, 0.0f,
             1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
             1.0f,  1.0f,  0.0f,   1.0f, 0.0f,
             1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
             1.0f,  1.0f,  1.0f,   0.0f, 0.0f,
             1.0f,  0.0f,  1.0f,   0.0f, 1.0f,

            // left face
             0.0f,  1.0f,  1.0f,   1.0f, 0.0f,
             0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
             0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
             0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
             0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
             0.0f,  1.0f,  1.0f,   1.0f, 0.0f,

            // top face
             0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
             0.0f,  1.0f,  1.0f,   0.0f, 0.0f,
             1.0f,  1.0f,  1.0f,   1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,   1.0f, 0.0f,
             1.0f,  1.0f,  0.0f,   1.0f, 1.0f,
             0.0f,  1.0f,  0.0f,   0.0f, 1.0f,

            // bottom face
             0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
             1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
             1.0f,  0.0f,  1.0f,   1.0f, 0.0f,
             1.0f,  0.0f,  1.0f,   1.0f, 0.0f,
             0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
             0.0f,  0.0f,  0.0f,   0.0f, 1.0f,

            // back face
             0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
             0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
             1.0f,  1.0f,  0.0f,   0.0f, 1.0f,
             1.0f,  1.0f,  0.0f,   0.0f, 1.0f,
             1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
             0.0f,  0.0f,  0.0f,   1.0f, 0.0f,

            // front face
             0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
             1.0f,  0.0f,  1.0f,   1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,   1.0f, 1.0f,
             1.0f,  1.0f,  1.0f,   1.0f, 1.0f,
             0.0f,  1.0f,  1.0f,   0.0f, 1.0f,
             0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
        };

        f32 padding = 0.01f;
        for (u16 i = 0; i < array_size(vertices); i += 5) {
            for (u8 j = 0; j < 3; j++) {
                f32 vertex = vertices[i + j];
                vertices[i + j] = vertex * (1 + (padding * 2)) - padding;
            }
        }

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
        glEnableVertexAttribArray(1);

        #elif GL_API == 2
        // TODO
        #endif
    }

    void render(f32 proj_mat[16], f32 view_mat[16]) {
        #if GL_API == 0 || GL_API == 1
        shader.use();

        shader.setMat4("uProjection", proj_mat);
        shader.setMat4("uView", view_mat);
        shader.setVec3("uPos", pos);

        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        #elif GL_API == 2
        // TODO
        #endif
    }
};

#endif