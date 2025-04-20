#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include "../core/maths.h"

#include "renderer_3d.h"




char* readFileToCharArray(const char* filename, long* outSize) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    // Seek to the end to determine the file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Allocate memory for the content + null terminator
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    // Read file into buffer
    size_t bytesRead = fread(buffer, 1, size, file);
    buffer[bytesRead] = '\0'; // Null-terminate the string

    if (outSize) {
        *outSize = bytesRead;
    }

    fclose(file);
    return buffer;
}





// TO-BE external functions
// Callback function for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}








renderer_3d* renderer_3d_create(u16 w, u16 h, const char* title) {
    renderer_3d* renderer = malloc(sizeof(renderer_3d));
    *renderer = (renderer_3d) {
        .width = w,
        .height = h,

        .shader_programs_count = 0,
        .shader_programs = NULL,

        .textures_count = 0,
        .textures = NULL,

        .objects_count = 0,
        .objects = NULL,
    };

    // TODO: add dynamic array or something
    renderer->shader_programs = malloc(sizeof(GLuint) * 10);
    renderer->textures = malloc(sizeof(GLuint) * 10);
    renderer->objects = malloc(sizeof(object_3d) * 10);


    // Initialize GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return NULL;
    }

    // Set required options for GLFW: use OpenGL 3.3 and the core profile.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a GLFWwindow object
    renderer->window = glfwCreateWindow(800, 600, "Spinning Cube", NULL, NULL);
    if (renderer->window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(renderer->window);
    glfwSetFramebufferSizeCallback(renderer->window, framebuffer_size_callback);

    // Initialize GLAD before calling any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return NULL;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);





    return renderer;
}

void renderer_3d_free(renderer_3d* renderer) {
    glfwDestroyWindow(renderer->window);
    glfwTerminate();
}

bool renderer_3d_initGLFW() {
    return glfwInit();
}

bool renderer_3d_initGLAD() {
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void renderer_3d_add_object(renderer_3d* renderer, object_3d* object) {
    glGenVertexArrays(1, &object->VAO);
    glGenBuffers(1, &object->VBO);
    glGenBuffers(1, &object->EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s)
    glBindVertexArray(object->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, object->VBO);
    glBufferData(GL_ARRAY_BUFFER, object->vertices_count * sizeof(f32), object->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->indices_count * sizeof(u32), object->indices, GL_STATIC_DRAW);


    // Multiply by 8 because 8 floats per row (3 for vertex, 3 for normal, 2 for uv)
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    // TexCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    // Unbind for cleanliness
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    renderer->objects[renderer->objects_count++] = *object;
}



GLuint renderer_3d_create_texture(const u8* data, int width, int height, int nrChannels) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Texture wrapping and filtering
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

GLuint renderer_3d_compileShader(const char* vertexPath, const char* fragmentPath) {
    const char* vertexCode = readFileToCharArray(vertexPath, NULL);
    const char* fragmentCode = readFileToCharArray(fragmentPath, NULL);

    int success;
    char infoLog[512];

    
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);

    // Check for shader compile errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }



    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);

    // Check for shader compile errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }




    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    // Check for linking errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }




    // Delete the shaders as they're linked now
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

void renderer_3d_process_input(renderer_3d* renderer) {
    if (glfwGetKey(renderer->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(renderer->window, true);
    }
}

void renderer_3d_run(renderer_3d* renderer) {
    // Input handling
    renderer_3d_process_input(renderer);

    f64 start_time = glfwGetTime();

    // Clear the screen (color and depth)
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // update camera and view
    vec3 tmp_vec = {
        .x = 0.0,
        .y = 0.0,
        .z = -3.0,
    };
    // Camera/view transformation: move a bit backwards to see the cube
    mat4 view;
    mat4_identity(&view);
    mat4_translate_world_vec3(&view, &tmp_vec);

    // Projection matrix: perspective projection
    mat4 projection;
    mat4_perspective(deg_to_rad(45.0f), 800.0 / 600.0, 0.1, 100.0, &projection);




    GLuint shader_program;
    GLuint texture;

    f32 model_mat[16];
    f32 view_mat[16];
    f32 projection_mat[16];

    GLuint modelLoc;
    GLuint viewLoc;
    GLuint projLoc;
    
    for (u16 i = 0; i < renderer->objects_count; i++) {
        object_3d object = renderer->objects[i];

        shader_program = object.shader_program;
        texture = object.texture;

        // Activate shader program
        glUseProgram(shader_program);
        
        glUniform1i(glGetUniformLocation(shader_program, "ourTexture"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Transformation matrices
        // Calculate the model matrix: rotate over time
        mat4 model = object.transform;
        f64 angle = glfwGetTime() * deg_to_rad(50.0);
        mat4_rotate(&model, deg_to_rad(30), 0, 0);
        mat4_rotate(&model, 0.0 * angle, 3.0 * angle, 0.0 * angle);


        // Retrieve the uniform locations and set them
        modelLoc = glGetUniformLocation(shader_program, "model");
        viewLoc  = glGetUniformLocation(shader_program, "view");
        projLoc  = glGetUniformLocation(shader_program, "projection");

        mat4_to_gl_mat4(&model, model_mat);
        mat4_to_gl_mat4(&view, view_mat);
        mat4_to_gl_mat4(&projection, projection_mat);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model_mat);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view_mat);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection_mat);

        // Bind the VAO and draw the cube
        glBindVertexArray(object.VAO);
        // glDrawArrays(GL_TRIANGLES, 0, object.indices_count);
        glDrawElements(GL_TRIANGLES, object.indices_count, GL_UNSIGNED_INT, 0);
    }



    f64 end_time = glfwGetTime();
    printf("render %lf FPS (%lfms)\n", 1 / (end_time - start_time), (end_time - start_time) * 1000);




    // Swap buffers and poll IO events
    glfwSwapBuffers(renderer->window);
    glfwPollEvents();
}
