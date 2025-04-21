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








Renderer3D::Renderer3D(u16 w, u16 h, const char* title) :
width(w),
height(h) {
    // Initialize GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return;
    }

    // Set required options for GLFW: use OpenGL 3.3 and the core profile.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a GLFWwindow object
    this->window = glfwCreateWindow(800, 600, "Spinning Cube", NULL, NULL);
    if (this->window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(this->window);
    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);

    // Initialize GLAD before calling any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
}

void Renderer3D::free() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

bool Renderer3D::initGLFW() {
    return glfwInit();
}

bool Renderer3D::initGLAD() {
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void Renderer3D::add_object(Object3D& object) {
    glGenVertexArrays(1, &object.VAO);
    glGenBuffers(1, &object.VBO);
    glGenBuffers(1, &object.EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s)
    glBindVertexArray(object.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, object.VBO);
    glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(f32), object.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.indices.size() * sizeof(u32), object.indices.data(), GL_STATIC_DRAW);


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

    this->objects.push_back(object);
}



GLuint Renderer3D::create_texture(const u8* data, int width, int height, int nrChannels) {
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

GLuint Renderer3D::compile_shader(const char* vertexPath, const char* fragmentPath) {
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

void Renderer3D::process_input() {
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->window, true);
    }
}

void Renderer3D::run() {
    // Input handling
    this->process_input();

    f64 start_time = glfwGetTime();

    // Clear the screen (color and depth)
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // update camera and view
    Vec3<f64> tmp_vec(0.0, 0.0, -3.0);
    // Camera/view transformation: move a bit backwards to see the cube
    Mat4<f64> view;
    view.identity();
    view.translateWorld(tmp_vec);

    // Projection matrix: perspective projection
    Mat4<f64> projection = Mat4<f64>::perspective(deg_to_rad(45.0f), 800.0 / 600.0, 0.1, 100.0);




    GLuint shader_program;
    GLuint texture;

    f32 model_mat[16];
    f32 view_mat[16];
    f32 projection_mat[16];

    GLuint modelLoc;
    GLuint viewLoc;
    GLuint projLoc;
    
    for (u16 i = 0; i < this->objects.size(); i++) {
        Object3D object = this->objects[i];

        shader_program = object.shader_program;
        texture = object.texture;



        
        // Activate shader program
        glUseProgram(shader_program);
        
        glUniform1i(glGetUniformLocation(shader_program, "ourTexture"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);




        // Transformation matrices
        // Calculate the model matrix: rotate over time
        Mat4<f64> model = object.transform;
        f64 angle = glfwGetTime() * deg_to_rad(50.0);
        model.rotate(deg_to_rad(30), 0, 0);
        model.rotate(0.0 * angle, 3.0 * angle, 0.0 * angle);



        // Retrieve the uniform locations and set them
        modelLoc = glGetUniformLocation(shader_program, "model");
        viewLoc  = glGetUniformLocation(shader_program, "view");
        projLoc  = glGetUniformLocation(shader_program, "projection");

        model.toGLMatrix(model_mat);
        view.toGLMatrix(view_mat);
        projection.toGLMatrix(projection_mat);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model_mat);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view_mat);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection_mat);





        // Bind the VAO and draw the cube
        glBindVertexArray(object.VAO);
        // glDrawArrays(GL_TRIANGLES, 0, object.indices_count);
        glDrawElements(GL_TRIANGLES, object.indices.size(), GL_UNSIGNED_INT, 0);
    }



    f64 end_time = glfwGetTime();
    printf("render %lf FPS (%lfms)\n", 1 / (end_time - start_time), (end_time - start_time) * 1000);




    // Swap buffers and poll IO events
    glfwSwapBuffers(this->window);
    glfwPollEvents();
}
