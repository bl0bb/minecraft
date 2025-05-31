#ifndef TEXT_RENDERER_UI_H
#define TEXT_RENDERER_UI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../core/types.h"

class TextRenderer {
public:
    u16 windowWidth;
    u16 windowHeight;

    u8 charsPerRow;

    u8 charWidth;
    u8 charHeight;

    u8 width;
    u8 height;

    u8* charSizes;

    Shader shader;

    // private
    GLuint VBO, VAO;
    GLuint textureID;
    
    TextRenderer() {};

    TextRenderer(u16 _windowWidth, u16 _windowHeight, u8 _charsPerRow, u8 _charWidth, u8 _charHeight, u8 _width, u8 _height, u8* _charSizes, Shader _shader) :
        windowWidth(_windowWidth),
        windowHeight(_windowHeight),

        charsPerRow(_charsPerRow),

        charWidth(_charHeight),
        charHeight(_charHeight),

        width(_width),
        height(_height),

        charSizes(_charSizes),

        shader(_shader)
    {
        f32 quadVertices[] = {
            // pos      // tex
            0.0f, 1.0f,  0.0f, 1.0f,
            1.0f, 0.0f,  1.0f, 0.0f,
            0.0f, 0.0f,  0.0f, 0.0f,
        
            0.0f, 1.0f,  0.0f, 1.0f,
            1.0f, 1.0f,  1.0f, 1.0f,
            1.0f, 0.0f,  1.0f, 0.0f,

            
            // 1.0f, 0.0f,  1.0f, 0.0f,
            // 1.0f, 1.0f,  1.0f, 1.0f,
            // 0.0f, 1.0f,  0.0f, 1.0f,
            
            // 0.0f, 0.0f,  0.0f, 0.0f,
            // 1.0f, 0.0f,  1.0f, 0.0f,
            // 0.0f, 1.0f,  0.0f, 1.0f,
        
        };
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        
        // position
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)0);
        glEnableVertexAttribArray(0);
        
        // texcoord
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)(2 * sizeof(f32)));
        glEnableVertexAttribArray(1);
    }

    ~TextRenderer() {

    }

    void loadFont(const u8* imageData) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Upload the texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void renderChar(char c, f32 x, f32 y, f32 scale) {
        // int asciiIndex = c - 32;
        int asciiIndex = c;
        int row = asciiIndex / charsPerRow;
        int col = asciiIndex % charsPerRow;

        f32 deltaWidth = charWidth / (f32)width;
        f32 deltaHeight = charHeight / (f32)height;

        f32 u0 = col * deltaWidth;
        f32 v0 = row * deltaHeight;
        // f32 u1 = (col + 1) * deltaWidth;
        // f32 v1 = (row + 1) * deltaHeight;
    

        // Set uniforms
        shader.setVec2("uOffset", x, y);
        shader.setVec2("uScale", scale, scale);

        shader.setVec2("uTexOffset", u0, v0);
        shader.setVec2("uTexSize", deltaWidth, deltaHeight);


        // Set texture transform using a texture matrix or use texcoord manipulation in shader if needed

    
        // Bind and draw
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void renderText(const char* str, f32 x, f32 y, f32 scale, RGB4 color) {
        shader.use();

        // color
        shader.setInt("uColor", color);

        // projection
        Mat4<f32> ortho = Mat4<f32>::ortho(-(float)windowWidth / 2, (float)windowWidth / 2, (float)windowHeight / 2, -(float)windowHeight / 2);
        f32 proj_mat[16];
        ortho.toGLMatrix(proj_mat);
        shader.setMat4("uProjection", proj_mat);

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("fontTexture", 0);

        f32 origX = x;
        while (*str) {
            renderChar(*str, x, y, scale);
            x += scale * ((charSizes[*str] + 1) / (f32)charWidth);
            str++;
        }
    }
};

#endif