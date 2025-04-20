#ifndef RENDERER_3D_H
#define RENDERER_3D_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdbool.h>

#include "../core/types.h"
#include "object_3d.h"

typedef struct {
    GLFWwindow* window;
    uint16_t width;
    uint16_t height;

    // for rendering objects

    // shaders
    u16 shader_programs_count;
    GLuint* shader_programs;

    // textures
    u16 textures_count;
    GLuint* textures;

    // objects
    u16 objects_count;
    object_3d* objects;
} renderer_3d;






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








renderer_3d* renderer_3d_create(u16 w, u16 h, const char* title);

void renderer_3d_free(renderer_3d* renderer);

bool renderer_3d_initGLFW();

bool renderer_3d_initGLAD();

void renderer_3d_add_object(renderer_3d* renderer, object_3d* object);



GLuint renderer_3d_create_texture(const u8* data, int width, int height, int nrChannels);

GLuint renderer_3d_compileShader(const char* vertexPath, const char* fragmentPath);

void renderer_3d_process_input(renderer_3d* renderer);

void renderer_3d_run(renderer_3d* renderer);












#endif