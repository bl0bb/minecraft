#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <cmath>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "core/maths.h"
#include "core/camera.h"
#include "core/shader.h"

#include "voxel/voxel_mesher.h"
#include "voxel/voxel_renderer.h"

// TODO: add quad support for rendering and for obj importing??

constexpr u16 WINDOW_WIDTH = 1920;
constexpr u16 WINDOW_HEIGHT = 1080;
const bool FULLSCREEN = false;




void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

void GLAPIENTRY message_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
) {
    std::string SEVERITY = "";
    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
    SEVERITY = "LOW";
    break;
    case GL_DEBUG_SEVERITY_MEDIUM:
    SEVERITY = "MEDIUM";
    break;
    case GL_DEBUG_SEVERITY_HIGH:
    SEVERITY = "HIGH";
    break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
    SEVERITY = "NOTIFICATION";
    break;
    }
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = %s, message = %s\n",
    type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "",
    type, SEVERITY.c_str(), message);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // camera->processMouseMovement(xpos - last_x, last_y - ypos);
    // last_x = xpos;
    // last_y = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(window, true);
    } else if (key == GLFW_KEY_X && action == GLFW_RELEASE) {
        GLint lastPolyMode[2];
        glGetIntegerv(GL_POLYGON_MODE, lastPolyMode);
        if (lastPolyMode[0] == GL_FILL) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}

bool init_opengl() {
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback(message_callback, 0);

    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glClearColor(0.529f, 0.808f, 0.922f, 0.0f);

    glEnable(GL_MULTISAMPLE);

    return true;
};

GLFWwindow* init_window() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 2);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Binary Greedy Meshing V2", FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Unable to create GLFW window\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        fprintf(stderr, "Unable to initialize glad\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    return window;
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        fprintf(stderr, "Unable to initialize GLFW\n");
        return 1;
    }

    auto window = init_window();
    if (!window) {
        return 1;
    }
    glfwSetWindowPos(window, 0, 31);
    glfwSwapInterval(0);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    if (!init_opengl()) {
        fprintf(stderr, "Unable to initialize glad/opengl\n");
        return 1;
    }








    MeshData meshData;
    meshData.vertices = new std::vector<u64>(10000);

    u8* voxels = new u8[CS_P3]{0};
    memset(voxels, 0, CS_P3);

    // int r = CS_P / 2;
    // for (int x = -r; x < r; x++) {
    //     for (int y = -r; y < r; y++) {
    //         for (int z = -r; z < r; z++) {
    //             if (std::sqrt(x * x + y * y + z * z) < 30.0f) {
    //                 voxels[get_zxy_index(x + r, y + r, z + r)] = 8;
    //             }
    //         }
    //     }
    // }

    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            for (int z = 0; z < 5; z++) {
                voxels[get_zxy_index(20 + x, 20 + y, 20 + z)] = 1;
            }
        }
    }

    generate_voxel_mesh(voxels, meshData);






    Shader shader = Shader("voxel", "voxel");
    Camera camera = Camera(Vec3<f32>(0, 0, -50));
    camera.handleResolution(WINDOW_WIDTH, WINDOW_HEIGHT);
  
    float forwardMove = 0.0f;
    float rightMove = 0.0f;
    float noclipSpeed = 250.0f;
  
    float deltaTime = 0.0f;
  
    auto lastFrame = glfwGetTime();











    // rendering
    // Unit cube vertices (centered at origin)
    float cubeVertices[] = {
        -0.5f,-0.5f,-0.5f,  // 0
        0.5f,-0.5f,-0.5f,  // 1
        0.5f, 0.5f,-0.5f,  // 2
        -0.5f, 0.5f,-0.5f,  // 3
        -0.5f,-0.5f, 0.5f,  // 4
        0.5f,-0.5f, 0.5f,  // 5
        0.5f, 0.5f, 0.5f,  // 6
        -0.5f, 0.5f, 0.5f   // 7
    };

    unsigned int cubeIndices[] = {
        0,1,2, 2,3,0, // back
        4,5,6, 6,7,4, // front
        4,5,1, 1,0,4, // bottom
        7,6,2, 2,3,7, // top
        4,0,3, 3,7,4, // left
        5,1,2, 2,6,5  // right
    };

    // Setup VAO/VBO/EBO
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    // SSBO Setup
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, meshData.vertices->size() * sizeof(u64), meshData.vertices->data(), GL_STATIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);









    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) forwardMove = 1.0f;
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) forwardMove = -1.0f;
        else forwardMove = 0.0f;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) rightMove = 1.0f;
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) rightMove = -1.0f;
        else rightMove = 0.0f;
        auto wishdir = (camera.front * forwardMove) + (camera.right * rightMove);
        camera.position = camera.position + wishdir * noclipSpeed * deltaTime;

        Vec3<i64> cameraChunkPos = camera.position / CS;

        

        int numCommands = meshData.vertices->size();



        // rendering
        f32 proj_mat[16];
        camera.projection.toGLMatrix(proj_mat);

        f32 view_mat[16];
        camera.getViewMatrix().toGLMatrix(view_mat);

        shader.use();
        shader.setMat4("u_projection", proj_mat);
        shader.setMat4("u_view", view_mat);
        shader.setVec3("eye_position", camera.position);

        Vec3<i64> intCamPosition = camera.position; // TODO: floor?
        shader.setIVec3("eye_position_int", intCamPosition);

        glBindVertexArray(vao);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, numCommands);










    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }












    return 0;
}