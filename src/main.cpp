#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "core/maths.h"
#include "core/camera.h"
#include "core/shader.h"

#include "voxel/voxel.h"
#include "voxel/voxel_mesher.h"
#include "voxel/voxel_renderer.h"
#include "voxel/voxel_world.h"

#include "core/array.h"
#include "blocks.h"

#include "shading/ambient_occlusion.h"
#include "quad.h"

#include "terrain_gen/terrain_gen.h"

#include "text/text_renderer.h"

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
    glfwWindowHint(GLFW_SAMPLES, 1);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "bing bong bing bong bing bong bing bong bing bong bing bong bing bong bing bong bing bong bing bong bing bong", FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr, nullptr);
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

int load_texture(const char* path, u16 texIdx, u8 texWidth, u8 texHeight, i32& nrChannels) {
    // Load and create a texture
    int width, height;
    u8* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (width != texWidth) {
        printf("LOADED IMAGE HAS INCORRECT WIDTH: %i %i\n", width, texWidth);
        return 1;
    }
    if (height != texHeight) {
        printf("LOADED IMAGE HAS INCORRECT HEIGHT: %i %i\n", height, texHeight);
        return 1;
    }

    GLuint format;
    switch (nrChannels) {
        case 1:
            format = GL_RED;
            break;
        case 2:
            // TODO: this is useless?? lol??
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            break;
    }

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texIdx, texWidth, texHeight, 1, format, GL_UNSIGNED_BYTE, data);

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









    Noise noise = Noise();

    // world size in chunks
    u16 world_size = 1; // 8;
    u16 world_height = 1; // 4;
    u16 world_total_chunks = world_size * world_size * world_height;

    VoxelWorld voxelWorld = VoxelWorld();
    voxelWorld.chunks = std::vector<VoxelChunk>(world_total_chunks);
    for (int x = 0; x < world_size; x++) {
        for (int y = 0; y < world_height; y++) {
            for (int z = 0; z < world_size; z++) {
                VoxelChunk chunk = VoxelChunk();
                chunk.init();
                chunk.pos = Vec3<u64>(x, y, z);

                noise.generateTerrain(chunk.voxels, x, y, z, 69);

                VoxelFace* voxel_faces = new VoxelFace[CS_P3]{0};
                chunk.voxel_count = generate_voxel_mesh(chunk.voxels, voxel_faces);
                chunk.updateMesh(voxel_faces);

                voxelWorld.chunks[z + (x * world_size) + (y * world_size * world_size)] = chunk;
            }
        }
    }


    /*
    // // sphere
    // int r = CS_P / 2;
    // for (int x = -r; x < r; x++) {
    //     for (int y = -r; y < r; y++) {
    //         for (int z = -r; z < r; z++) {
    //             if (std::sqrt(x * x + y * y + z * z) < 15.0f) {
    //                 voxels[get_zxy_index(x + r, y + r, z + r)] = EmbeddedVoxels::create(1);
    //             }
    //         }
    //     }
    // }

    // // layers
    // u8 testRegionSize = 4;
    // for (int x = 0; x < testRegionSize; x++) {
    //     for (int z = 0; z < testRegionSize; z++) {
    //         voxels[get_zxy_index(x, 0, z)] = EmbeddedVoxels::create(3);
    //     }
    // }
    // for (int x = 0; x < testRegionSize; x++) {
    //     for (int z = 0; z < testRegionSize; z++) {
    //         voxels[get_zxy_index(x, 2, z)] = EmbeddedVoxels::create(2);
    //     }
    // }
    // for (int x = 0; x < testRegionSize; x++) {
    //     for (int z = 0; z < testRegionSize; z++) {
    //         voxels[get_zxy_index(x, 4, z)] = EmbeddedVoxels::create(1);
    //     }
    // }
    // for (int x = 0; x < testRegionSize; x++) {
    //     for (int z = 0; z < testRegionSize; z++) {
    //         voxels[get_zxy_index(x, 6, z)] = EmbeddedVoxels::create(4);
    //     }
    // }


    // // house
    // // ground
    // u8 platform_size = 32;
    // for (int x = 0; x < platform_size; x++) {
    //     for (int z = 0; z < platform_size; z++) {
    //         voxels[get_zxy_index(x, 0, z)] = EmbeddedVoxels::create(BlockType::COBBLESTONE + 1);
    //         voxels[get_zxy_index(x, 1, z)] = EmbeddedVoxels::create(BlockType::DIRT + 1);
    //         voxels[get_zxy_index(x, 2, z)] = EmbeddedVoxels::create(BlockType::GRASS + 1);
    //     }
    // }
    // u8 house_size = 8;
    // u8 house_height = 6;
    // Vec2<u8> house_center(
    //     platform_size / 2 - house_size / 2,
    //     platform_size / 2 - house_size / 2
    // );
    // // walls
    // for (int i = 0; i < house_size; i++) {
    //     for (int y = 0; y < house_height; y++) {
    //         voxels[get_zxy_index(house_center.x,                      3 + y, house_center.y + i)] = EmbeddedVoxels::create(BlockType::OAK_PLANKS + 1);
    //         voxels[get_zxy_index(house_center.x + i,                  3 + y, house_center.y + house_size - 1)] = EmbeddedVoxels::create(BlockType::OAK_PLANKS + 1);
    //         voxels[get_zxy_index(house_center.x + house_size - 1,     3 + y, house_center.y + house_size - 1 - i)] = EmbeddedVoxels::create(BlockType::OAK_PLANKS + 1);
    //         voxels[get_zxy_index(house_center.x + house_size - 1 - i, 3 + y, house_center.y)] = EmbeddedVoxels::create(BlockType::OAK_PLANKS + 1);
    //     }
    // }
    // // door
    // voxels[get_zxy_index(house_center.x + 2, 4, house_center.y)] = 0;
    // voxels[get_zxy_index(house_center.x + 2, 5, house_center.y)] = 0;
    // // floor and ceiling
    // for (int x = 0; x < house_size - 2; x++) {
    //     for (int z = 0; z < house_size - 2; z++) {
    //         voxels[get_zxy_index(house_center.x + 1 + x, 3, house_center.y + 1 + z)] = EmbeddedVoxels::create(BlockType::OAK_PLANKS + 1);
    //         voxels[get_zxy_index(house_center.x + 1 + x, 3 + house_height - 1, house_center.y + 1 + z)] = EmbeddedVoxels::create(BlockType::OAK_PLANKS + 1);
    //     }
    // }
    // // roof
    // for (int i = 0; i < house_size / 2 + 1; i++) {
    //     for (int z = 0; z < house_size + 2; z++) {
    //         voxels[get_zxy_index(
    //             house_center.x - 1 + i,
    //             3 + house_height - 1 + i,
    //             house_center.y - 1 + z
    //         )] = EmbeddedVoxels::create(BlockType::OAK_PLANKS + 1);
    //         voxels[get_zxy_index(
    //             house_center.x + house_size - i,
    //             3 + house_height - 1 + i,
    //             house_center.y - 1 + z
    //         )] = EmbeddedVoxels::create(BlockType::OAK_PLANKS + 1);
    //     }
    // }
    */







    camera = new Camera(Vec3<f32>(40, 40, 40));
    camera->handleResolution(WINDOW_WIDTH, WINDOW_HEIGHT);
  
    float forwardMove = 0.0f;
    float rightMove = 0.0f;
    float noclipSpeed = 50.0f;
  
    float deltaTime = 0.0f;
  
    auto lastFrame = glfwGetTime();








    









    // texture ssbo
    GLuint texture_ssbo;

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
    u32* block_textures_data = new u32[array_size(block_textures)]{0};

    for (u16 i = 0; i < array_size(block_textures); i++) {
        std::string path = "assets/textures/";
        path += block_textures[i];
        path += ".png";
        i32 nrChannels;
        load_texture(path.c_str(), texIdx++, texWidth, texHeight, nrChannels);
        block_textures_data[i] = nrChannels;
    }


    // Create SSBO for texture metadata
    glGenBuffers(1, &texture_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, texture_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, array_size(block_textures) * sizeof(u32), block_textures_data, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, texture_ssbo);






    Shader geometryShader = Shader("voxel/main.vert", "voxel/main.frag");
    Shader edgeShader = Shader("edge/edge.vert", "edge/edge.frag");


    /*
    // --------------------
    // AO
    // Shaders
    Shader geometryShader("voxel/main.vert", "voxel/main.frag");
    Shader ssaoShader("ambient_occlusion/ssao.vert", "ambient_occlusion/ssao.frag");
    Shader blurShader("ambient_occlusion/ssao.vert", "ambient_occlusion/blur.frag");
    Shader finalShader("ambient_occlusion/ssao.vert", "ambient_occlusion/final.frag");

    // G-Buffer
    GLuint gBuffer;
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    GLuint gPosition, gNormal;
    glGenTextures(1, &gPosition);
    glGenTextures(1, &gNormal);

    // Position color buffer
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // Normal color buffer
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // Tell OpenGL which color attachments we'll use
    GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    // Depth buffer
    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "G-Buffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // SSAO framebuffer
    GLuint ssaoFBO, ssaoBlurFBO;
    glGenFramebuffers(1, &ssaoFBO);
    glGenFramebuffers(1, &ssaoBlurFBO);

    GLuint ssaoColorBuffer, ssaoColorBufferBlur;
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

    glGenTextures(1, &ssaoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create SSAO kernel + noise
    std::vector<Vec3<f32>> ssaoKernel;
    std::vector<Vec3<f32>> ssaoNoise;
    GLuint ssao_noise_texture = generateSSAOTexture(ssaoKernel, ssaoNoise);
    // --------------------
    */





    // GLuint quadVao = createQuad();






    // TextRenderer textRenderer = TextRenderer(16, 8, 8, 16 * 8, 16 * 8);
    TextRenderer textRenderer(16, 8, 8, 16 * 8, 16 * 8);

    Shader textShader = Shader("text/main.vert", "text/main.frag");

    for (int i = 0; i < 1; i++) {
        int width, height, nrChannels;
        u8* data = stbi_load("assets/fonts/ascii.png", &width, &height, &nrChannels, 0);
        
        textRenderer.loadFont(data);

        stbi_image_free(data);
    }



    u32 shaderType = 0;



    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        if (deltaTime >= (f32)1 / 60) {
            lastFrame = currentFrame;

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) forwardMove = 1.0f;
            else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) forwardMove = -1.0f;
            else forwardMove = 0.0f;

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) rightMove = 1.0f;
            else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) rightMove = -1.0f;
            else rightMove = 0.0f;
            auto wishdir = (camera->front * forwardMove) + (camera->right * rightMove);
            camera->position = camera->position + wishdir * noclipSpeed * deltaTime;

            if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) shaderType = 0;
            else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) shaderType = 1;
            else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) shaderType = 2;
            else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) shaderType = 3;
            else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) shaderType = 4;
            else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) shaderType = 5;
            else if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) shaderType = 6;
            else if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) shaderType = 7;
            else if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) shaderType = 8;
            else if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) shaderType = 9;

            if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            

            // printf("(%f %f %f) (%f %f %f)\n", camera->front.x, camera->front.y, camera->front.z, camera->position.x, camera->position.y, camera->position.z);

            


            // --------------------------------------
            // GEOMETRY PASS
            // glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Vec3<i64> cameraChunkPos = camera->position / CS;

            // rendering
            f32 proj_mat[16];
            camera->projection.toGLMatrix(proj_mat);

            f32 view_mat[16];
            camera->getViewMatrix().toGLMatrix(view_mat);

            Shader* activeShader;
            if (shaderType == 0 || true) {
                activeShader = &geometryShader;
            } else if (shaderType == 1) {
                activeShader = &edgeShader;
            }

            activeShader->use();

            activeShader->setMat4("u_projection", proj_mat);
            activeShader->setMat4("u_view", view_mat);
            activeShader->setVec3("eye_position", camera->position);

            Vec3<i64> intCamPosition = camera->position;
            activeShader->setIVec3("eye_position_int", intCamPosition);

            if (shaderType == 0 || true) {
                // bind textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
                geometryShader.setInt("texArray", 0);

                // ao
                geometryShader.setInt("gPosition", 1);
                geometryShader.setInt("gNormal", 2);
                geometryShader.setInt("texNoise", 3);
            } else if (shaderType == 1) {
                edgeShader.setFloat("texelWidth", 1.0f / WINDOW_WIDTH);
                edgeShader.setFloat("texelHeight", 1.0f / WINDOW_HEIGHT);
            }

            for (int i = 0; i < world_total_chunks; i++) {
                voxelWorld.chunks[i].render(*activeShader);
            }

            // glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // --------------------------------------





            // // --------------------------------------
            // // 2. SSAO Pass: generate SSAO texture
            // glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
            // glClear(GL_COLOR_BUFFER_BIT);
            // ssaoShader.use();
            // for (unsigned int i = 0; i < 64; ++i)
            //     ssaoShader.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
            // ssaoShader.setMat4("projection", proj_mat);

            // glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, gPosition);
            // glActiveTexture(GL_TEXTURE1);
            // glBindTexture(GL_TEXTURE_2D, gNormal);
            // glActiveTexture(GL_TEXTURE2);
            // glBindTexture(GL_TEXTURE_2D, ssao_noise_texture);

            // ssaoShader.setInt("gPosition", 0);
            // ssaoShader.setInt("gNormal", 1);
            // ssaoShader.setInt("texNoise", 2);

            // glBindVertexArray(quadVao);
            // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            // glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // // --------------------------------------

            // // --------------------------------------
            // // 3. Blur Pass: blur SSAO texture
            // glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
            // glClear(GL_COLOR_BUFFER_BIT);
            // blurShader.use();

            // glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
            // blurShader.setInt("ssaoInput", 0);

            // glBindVertexArray(quadVao);
            // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            // glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // // --------------------------------------

            // // --------------------------------------
            // // 4. Lighting Pass: render final scene
            // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // finalShader.use();

            // glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, gPosition);
            // glActiveTexture(GL_TEXTURE1);
            // glBindTexture(GL_TEXTURE_2D, gNormal);
            // glActiveTexture(GL_TEXTURE2);
            // glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);

            // finalShader.setInt("gPosition", 0);
            // finalShader.setInt("gNormal", 1);
            // finalShader.setInt("ssao", 2);

            // glBindVertexArray(quadVao);
            // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            // // --------------------------------------




            // text
            // textRenderer.renderText("linganguliguliguli gwata lingangu lingangu", 50, 50, 100, textShader.ID);





        
            glfwSwapBuffers(window);
        }
        
        glfwPollEvents();
    }












    return 0;
}