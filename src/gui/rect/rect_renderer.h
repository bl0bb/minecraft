#ifndef RECT_RENDERER_UI_H
#define RECT_RENDERER_UI_H

#include "../../core/maths.h"

class RectRendererUI {
public:
    Vec2<f32> pos;
    Vec2<f32> size;
    RGBI8 color;

    GLuint rectVao, rectVbo;

    Shader shader;

    // empty constructor
    // TODO: turn this into an "init" function instead
    static RectRendererUI empty() {
        RectRendererUI rectRenderer;
        
        // mesh
        glGenVertexArrays(1, &rectRenderer.rectVao);
        glGenBuffers(1, &rectRenderer.rectVbo);
        glBindVertexArray(rectRenderer.rectVao);

        glBindBuffer(GL_ARRAY_BUFFER, rectRenderer.rectVbo);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)(2 * sizeof(f32)));
        glEnableVertexAttribArray(1);

        return rectRenderer;
    }

    RectRendererUI() {}

    // call this when changing size, position or color
    void update() {
        #if GL_API == 0 || GL_API == 1
        glBindVertexArray(rectVao);
        glBindBuffer(GL_ARRAY_BUFFER, rectVbo);
        f32 imageVertices[] = {
             1.0f * (size.x / 2) + pos.x, -1.0f * (size.y / 2) + pos.y, 0.0f, 0.0f,
            -1.0f * (size.x / 2) + pos.x, -1.0f * (size.y / 2) + pos.y, 1.0f, 0.0f,
             1.0f * (size.x / 2) + pos.x,  1.0f * (size.y / 2) + pos.y, 0.0f, 1.0f,
            -1.0f * (size.x / 2) + pos.x,  1.0f * (size.y / 2) + pos.y, 1.0f, 1.0f,
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(imageVertices), imageVertices, GL_STATIC_DRAW);
        #elif GL_API == 2
        // TODO
        #endif
    }

    void render(u16 WINDOW_WIDTH, u16 WINDOW_HEIGHT) {
        #if GL_API == 0 || GL_API == 1
        shader.use();

        Mat4<f32> ortho = Mat4<f32>::ortho(-(f32)WINDOW_WIDTH / 2, (f32)WINDOW_WIDTH / 2, (f32)WINDOW_HEIGHT / 2, -(f32)WINDOW_HEIGHT / 2);
        f32 proj_mat[16];
        ortho.toGLMatrix(proj_mat);
        shader.setMat4("uProjection", proj_mat);

        glBindVertexArray(rectVao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, imageTexture);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Unbind
        glBindTexture(GL_TEXTURE_2D, 0);
        #elif GL_API == 2
        // TODO
        #endif
    }
};

#endif