#ifndef CROSSHAIR_UI_H
#define CROSSHAIR_UI_H

#include "../../core/shader.h"

class CrosshairUI {
public:
    u16 crosshairSize;

    Shader crosshairShader;
    GLuint crosshairVao, crosshairVbo;
    GLuint crosshairTexture;

    CrosshairUI(u16 _crosshairSize) : crosshairSize(_crosshairSize) {
        // shader
        crosshairShader = Shader("crosshair/main.vert", "crosshair/main.frag");

        // mesh
        #if GL_API == 0 || GL_API == 1
        f32 crosshairVertices[] = {
            1.0f * (crosshairSize / 2), -1.0f * (crosshairSize / 2), 0.0f, 0.0f,
            -1.0f * (crosshairSize / 2), -1.0f * (crosshairSize / 2), 1.0f, 0.0f,
            1.0f * (crosshairSize / 2),  1.0f * (crosshairSize / 2), 0.0f, 1.0f,
            -1.0f * (crosshairSize / 2),  1.0f * (crosshairSize / 2), 1.0f, 1.0f,
        };

        glGenVertexArrays(1, &crosshairVao);
        glGenBuffers(1, &crosshairVbo);
        glBindVertexArray(crosshairVao);

        glBindBuffer(GL_ARRAY_BUFFER, crosshairVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)(2 * sizeof(f32)));
        glEnableVertexAttribArray(1);



        // texture
        glGenTextures(1, &crosshairTexture);
        glBindTexture(GL_TEXTURE_2D, crosshairTexture);

        // Set wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width, height, nrChannels;
        u8* data = stbi_load("assets/textures/gui/crosshair.png", &width, &height, &nrChannels, 0);

        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        #elif GL_API == 2
        // TODO
        #endif
    }

    void render(u16 WINDOW_WIDTH, u16 WINDOW_HEIGHT) {
        #if GL_API == 0 || GL_API == 1
        crosshairShader.use();

        Mat4<f32> ortho = Mat4<f32>::ortho(-(f32)WINDOW_WIDTH / 2, (f32)WINDOW_WIDTH / 2, (f32)WINDOW_HEIGHT / 2, -(f32)WINDOW_HEIGHT / 2);
        f32 proj_mat[16];
        ortho.toGLMatrix(proj_mat);
        crosshairShader.setMat4("uProjection", proj_mat);

        glBindVertexArray(crosshairVao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, crosshairTexture);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        #elif GL_API == 2
        // TODO
        #endif
    }
};

#endif