#ifndef IMAGE_RENDERER_UI_H
#define IMAGE_RENDERER_UI_H

#include "../../core/maths.h"
#include "../../gui/text/text_renderer.h"

class ImageRendererUI {
public:
    Vec2<f32> pos;
    Vec2<f32> size;

    GLuint imageVao, imageVbo;
    GLuint imageTexture;

    Shader shader;

    // empty constructor
    // TODO: turn this into an "init" function instead
    static ImageRendererUI empty() {
        ImageRendererUI imageRenderer;
        
        // mesh
        glGenVertexArrays(1, &imageRenderer.imageVao);
        glGenBuffers(1, &imageRenderer.imageVbo);
        glBindVertexArray(imageRenderer.imageVao);

        glBindBuffer(GL_ARRAY_BUFFER, imageRenderer.imageVbo);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)(2 * sizeof(f32)));
        glEnableVertexAttribArray(1);

        // texture
        glGenTextures(1, &imageRenderer.imageTexture);
        glBindTexture(GL_TEXTURE_2D, imageRenderer.imageTexture);

        // Set wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Unbind
        glBindTexture(GL_TEXTURE_2D, 0);

        return imageRenderer;
    }

    ImageRendererUI() {}

    // call this when changing size or position
    void update() {
        #if GL_API == 0 || GL_API == 1
        glBindVertexArray(imageVao);
        glBindBuffer(GL_ARRAY_BUFFER, imageVbo);
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
    
    void loadImage(const char* path) {
        int width, height, nrChannels;
        u8* data = stbi_load(path, &width, &height, &nrChannels, 0);

        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, imageTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        // Unbind
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void render(u16 WINDOW_WIDTH, u16 WINDOW_HEIGHT) {
        #if GL_API == 0 || GL_API == 1
        shader.use();

        Mat4<f32> ortho = Mat4<f32>::ortho(-(f32)WINDOW_WIDTH / 2, (f32)WINDOW_WIDTH / 2, (f32)WINDOW_HEIGHT / 2, -(f32)WINDOW_HEIGHT / 2);
        f32 proj_mat[16];
        ortho.toGLMatrix(proj_mat);
        shader.setMat4("uProjection", proj_mat);

        glBindVertexArray(imageVao);

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