#ifndef RENDERER_3D_H
#define RENDERER_3D_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <stdbool.h>

#include "../core/types.h"
#include "object_3d.h"

struct Renderer3D {
    GLFWwindow* window;
    u16 width;
    u16 height;

    // for rendering objects
    // shaders
    std::vector<GLuint> shader_programs;
    // textures
    std::vector<GLuint> textures;
    // objects
    std::vector<Object3D> objects;

    Renderer3D(u16 w, u16 h, const char* title);

    void free();

    bool initGLFW();

    bool initGLAD();

    void add_object(Object3D& object);

    void process_input();

    void run();

    static GLuint create_texture(const u8* data, int width, int height, int nrChannels);

    static GLuint compile_shader(const char* vertexPath, const char* fragmentPath);
};






#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include "../core/maths.h"

#include "renderer_3d.h"




char* readFileToCharArray(const char* filename, long* outSize);





// TO-BE external functions
// Callback function for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);












#endif