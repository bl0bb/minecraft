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

#include "voxel/voxel.h"
#include "voxel/voxel_mesher.h"
#include "voxel/voxel_renderer.h"

// TODO: add quad support for rendering and for obj importing??

constexpr u16 WINDOW_WIDTH = 1920;
constexpr u16 WINDOW_HEIGHT = 1080;
const bool FULLSCREEN = false;


Shader* shader;
Camera* camera;

float last_x = 0.0f;
float last_y = 0.0f;



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
    camera->processMouseMovement(xpos - last_x, last_y - ypos);
    last_x = xpos;
    last_y = ypos;
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

    // no cursor thing
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        fprintf(stderr, "Unable to initialize glad\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    return window;
}

int load_texture(const char* path, u16 texIdx, u8 texWidth, u8 texHeight) {
    // Load and create a texture
    int width, height, nrChannels;
    u8* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (width != texWidth) {
        printf("LOADED IMAGE HAS INCORRECT WIDTH: %i %i\n", width, texWidth);
        return 1;
    }
    if (height != texHeight) {
        printf("LOADED IMAGE HAS INCORRECT HEIGHT: %i %i\n", height, texHeight);
        return 1;
    }

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texIdx, texWidth, texHeight, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    return 0;
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

    EmbeddedVoxel* voxels = new EmbeddedVoxel[CS_P3]{0};
    memset(voxels, 0, CS_P3);

    int r = CS_P / 2;
    for (int x = -r; x < r; x++) {
        for (int y = -r; y < r; y++) {
            for (int z = -r; z < r; z++) {
                if (std::sqrt(x * x + y * y + z * z) < 15.0f) {
                    voxels[get_zxy_index(x + r, y + r, z + r)] = EmbeddedVoxels::create(1);
                }
            }
        }
    }

    // u8 testRegionSize = 4;
    // for (int x = 0; x < testRegionSize; x++) {
    //     for (int z = 0; z < testRegionSize; z++) {
    //         voxels[get_zxy_index(x, 0, z)] = EmbeddedVoxels::create(3);
    //     }
    // }
    // for (int x = 0; x < testRegionSize; x++) {
    //     for (int z = 0; z < testRegionSize; z++) {
    //         voxels[get_zxy_index(x, 1, z)] = EmbeddedVoxels::create(2);
    //     }
    // }
    // for (int x = 0; x < testRegionSize; x++) {
    //     for (int z = 0; z < testRegionSize; z++) {
    //         voxels[get_zxy_index(x, 2, z)] = EmbeddedVoxels::create(1);
    //     }
    // }
    // for (int x = 0; x < testRegionSize; x++) {
    //     for (int z = 0; z < testRegionSize; z++) {
    //         voxels[get_zxy_index(x, 3, z)] = EmbeddedVoxels::create(4);
    //     }
    // }

    generate_voxel_mesh(voxels, meshData);






    shader = new Shader("voxel", "voxel");
    camera = new Camera(Vec3<f32>(0, 0, 0));
    camera->handleResolution(WINDOW_WIDTH, WINDOW_HEIGHT);
  
    float forwardMove = 0.0f;
    float rightMove = 0.0f;
    float noclipSpeed = 10.0f;
  
    float deltaTime = 0.0f;
  
    auto lastFrame = glfwGetTime();











    // rendering
    // Define your cube vertices (positions only for simplicity)
    float cubeVertices[] = {
        0, 0, 0,
        1, 0, 0,
        1, 1, 0,
        0, 1, 0,

        // -0.5f, -0.5f, -0.5f,
        // 0.5f, -0.5f, -0.5f,
        // 0.5f,  0.5f, -0.5f,
        // -0.5f,  0.5f, -0.5f,
        // -0.5f, -0.5f,  0.5f,
        // 0.5f, -0.5f,  0.5f,
        // 0.5f,  0.5f,  0.5f,
        // -0.5f,  0.5f,  0.5f
    };

    // Define indices for the cube (using an IBO)
    unsigned int cubeIndices[] = {
        2, 1, 0, 0, 3, 2,

        // 2, 1, 0, 0, 3, 2,   // right face
        // 4, 5, 6, 6, 7, 4,   // left face
        // 1, 5, 4, 4, 0, 1,   // bottom face
        // 3, 7, 6, 6, 2, 3,   // top face
        // 4, 7, 3, 3, 0, 4,   // front face
        // 2, 6, 5, 5, 1, 2    // back face
    };

    GLuint vao, vbo, ebo, ssbo;
    // Create VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create VBO for cube vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Create EBO for cube indices
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // Setup vertex attribute for positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);

    // Create SSBO for instance positions
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, meshData.vertices->size() * sizeof(u64), meshData.vertices->data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    // Unbind VAO
    glBindVertexArray(0);



    // textures
    GLuint textureArray;
    glGenTextures(1, &textureArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

    // Allocate storage
    u16 numTextures = 32; // big number
    u8 texWidth = 16;
    u8 texHeight = 16;

    u16 texIdx = 0;

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, texWidth, texHeight, numTextures);

    // set to default settings
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // turn off texture smoothing
    glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // LOAD TEXTURES
    load_texture("assets/textures/grass_block_side.png", texIdx++, texWidth, texHeight);
    load_texture("assets/textures/dirt.png", texIdx++, texWidth, texHeight);
    load_texture("assets/textures/cobblestone.png", texIdx++, texWidth, texHeight);
    load_texture("assets/textures/oak_planks.png", texIdx++, texWidth, texHeight);








    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        if (deltaTime >= (f32)1 / 60) {
            lastFrame = currentFrame;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) forwardMove = 1.0f;
            else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) forwardMove = -1.0f;
            else forwardMove = 0.0f;

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) rightMove = 1.0f;
            else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) rightMove = -1.0f;
            else rightMove = 0.0f;
            auto wishdir = (camera->front * forwardMove) + (camera->right * rightMove);
            camera->position = camera->position + wishdir * noclipSpeed * deltaTime;

            // printf("(%f %f %f) (%f %f %f)\n", camera->front.x, camera->front.y, camera->front.z, camera->position.x, camera->position.y, camera->position.z);

            Vec3<i64> cameraChunkPos = camera->position / CS;

            

            int numCommands = meshData.vertices->size();


            // rendering
            f32 proj_mat[16];
            camera->projection.toGLMatrix(proj_mat);

            f32 view_mat[16];
            camera->getViewMatrix().toGLMatrix(view_mat);

            shader->use();
            shader->setMat4("u_projection", proj_mat);
            shader->setMat4("u_view", view_mat);
            shader->setVec3("eye_position", camera->position);

            Vec3<i64> intCamPosition = camera->position;
            shader->setIVec3("eye_position_int", intCamPosition);


            // bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
            shader->setInt("texArray", 0);



            // Bind VAO and draw
            glBindVertexArray(vao);
            glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, meshData.vertices->size());
            glBindVertexArray(0);





        
            glfwSwapBuffers(window);
        }
        
        glfwPollEvents();
    }












    return 0;
}