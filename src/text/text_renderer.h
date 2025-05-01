#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../core/types.h"

class TextRenderer {
public:
    u8 charsPerRow;
    u8 charWidth;
    u8 charHeight;
    u8 width;
    u8 height;

    GLuint VBO, VAO;
    GLuint textureID;
        
    TextRenderer(u8 _charsPerRow, u8 _charWidth, u8 _charHeight, u8 _width, u8 _height) :
        charsPerRow(_charsPerRow),
        charWidth(_charHeight),
        charHeight(_charHeight),
        width(_width),
        height(_height)
    {
        f32 quadVertices[] = {
            // pos      // tex
            0.0f, 1.0f,  0.0f, 1.0f,
            1.0f, 0.0f,  1.0f, 0.0f,
            0.0f, 0.0f,  0.0f, 0.0f,
        
            0.0f, 1.0f,  0.0f, 1.0f,
            1.0f, 1.0f,  1.0f, 1.0f,
            1.0f, 0.0f,  1.0f, 0.0f,
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, imageData);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void renderChar(char c, f32 x, f32 y, f32 scale, GLuint shaderProgram) {
        // int asciiIndex = c - 32;
        int asciiIndex = c;
        int row = asciiIndex / charsPerRow;
        int col = asciiIndex % charsPerRow;

        f32 u0 = col * (charWidth / (f32)width);
        f32 v0 = row * (charHeight / (f32)height);
        // f32 u1 = (col + 1) * (charWidth / (f32)width);
        // f32 v1 = (row + 1) * (charHeight / (f32)height);
    
        // f32 w = charWidth * scale;
        // f32 h = charHeight * scale;
    
        // Set uniforms
        glUseProgram(shaderProgram);
        glUniform2f(glGetUniformLocation(shaderProgram, "uOffset"), x, y);
        glUniform2f(glGetUniformLocation(shaderProgram, "uScale"), scale, scale);
    
        // Set texture transform using a texture matrix or use texcoord manipulation in shader if needed
    
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(glGetUniformLocation(shaderProgram, "fontTexture"), 0);
    
        // Bind and draw
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void renderText(const char* str, f32 x, f32 y, f32 scale, GLuint shaderProgram) {
        f32 origX = x;
        while (*str) {
            renderChar(*str, x, y, scale, shaderProgram);
            x += scale;
            str++;
        }
    }
};

#endif