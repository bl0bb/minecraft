#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <chrono>
#include <vector>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "core/maths.h"
#include "core/camera.h"
#include "core/shader.h"

#include "light/light.h"

#include "voxel/block/voxel_block_world.h"
#include "voxel/blockstate/voxel_blockstate_world.h"
#include "voxel/height/voxel_height_world.h"
#include "voxel/light/voxel_light_world.h"

#include "voxel/render/voxel_chunk_renderer.h"
#include "voxel/render/voxel_world_renderer.h"

#include "core/array.h"
#include "blocks/block.h"

#include "physics/raycast/raycast.h"
#include "physics/aabb/aabb.h"

#include "terrain_gen/terrain_gen.h"

#include "gui/text/text_renderer.h"

#include "gui_elements/crosshair/crosshair.h"
#include "gui_elements/fps_counter/fps_counter.h"
#include "gui_elements/hotbar/hotbar.h"

#include "renderers/block_outline/block_outline_renderer.h"

#include "nbt/nbt.h"

#include "file_parsers/nbt_parser.h"
#include "file_parsers/mca_parser.h"

// TODO: add quad support for rendering and for obj importing??

constexpr u16 WINDOW_WIDTH = 1920;
constexpr u16 WINDOW_HEIGHT = 1080;
const bool FULLSCREEN = false;


Shader* shader;
Camera* camera;


// mouse
f32 lastMouseX = 0.0f;
f32 lastMouseY = 0.0f;

std::unordered_map<int, bool> currentMouseButtonStates;
std::unordered_map<int, bool> lastMouseButtonStates;

bool wasMouseButtonStateChanged(int button) {
    return currentMouseButtonStates[button] != lastMouseButtonStates[button];
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    (void)window;
    camera->processMouseMovement(xpos - lastMouseX, lastMouseY - ypos);
    lastMouseX = xpos;
    lastMouseY = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    (void)window;
    (void)mods;
    if (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            currentMouseButtonStates[button] = true;
        } else if (action == GLFW_RELEASE) {
            currentMouseButtonStates[button] = false;
        }
    } else {
        std::cout << "Unknown mouse button: " << button << "\n";
    }
}

// Call this once per frame to update last frame key states
void updateLastMouseButtonStates() {
    lastMouseButtonStates = currentMouseButtonStates;
}

// keys
std::unordered_map<int, bool> currentKeyStates;
std::unordered_map<int, bool> lastKeyStates;

bool wasKeyStateChanged(int key) {
    return currentKeyStates[key] != lastKeyStates[key];
}

// Callback to track key events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key != GLFW_KEY_UNKNOWN) {
        if (action == GLFW_PRESS) {
            currentKeyStates[key] = true;
        } else if (action == GLFW_RELEASE) {
            currentKeyStates[key] = false;
        }
    } else {
        std::cout << "Unknown key: " << key << "\n";
    }
}

// Call this once per frame to update last frame key states
void updateLastKeyStates() {
    lastKeyStates = currentKeyStates;
}





// debugging
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
    (void)source;
    (void)id;
    (void)userParam;
    (void)length;
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

#if GL_API == 0 || GL_API == 1
bool init_opengl() {
    // this not working on mac
    // or is it?
    #if GL_API == 0
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback(message_callback, 0);
    #endif
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glClearColor(0.529f, 0.808f, 0.922f, 0.0f);

    glEnable(GL_MULTISAMPLE);

    // transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}
#elif GL_API == 2
// TODO
void createVkInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Cube";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

bool init_vulkan() {
    createVkInstance();
    return true;
}
#endif

GLFWwindow* init_window() {
    #if GL_API == 0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #elif GL_API == 1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on macOS
    #endif
    #elif GL_API == 2
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    #endif

    glfwWindowHint(GLFW_SAMPLES, 4);

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

    {
        // Load the icon image
        int width, height, channels;
        u8* imageData = stbi_load("assets/logo.jpg", &width, &height, &channels, 4); // Force RGBA
        if (imageData) {
            GLFWimage images[1];
            images[0].width = width;
            images[0].height = height;
            images[0].pixels = imageData;
            glfwSetWindowIcon(window, 1, images);
            stbi_image_free(imageData);
        } else {
            fprintf(stderr, "Failed to load icon image\n");
        }
    }

    #if GL_API == 0 || GL_API == 1
    if (!gladLoadGL()) {
        fprintf(stderr, "Unable to initialize glad\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }
    #elif GL_API == 2
    // TODO
    #endif

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

    #if GL_API == 0 || GL_API == 1
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

    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY, 0, // target, level
        0, 0, texIdx, // x offset, y offset, z offset
        texWidth, texHeight, 1, // width, height, depth
        format, GL_UNSIGNED_BYTE, data // format, type, pixels
    );

    #elif GL_API == 2
    // TODO
    #endif

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

    // mouse input
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // keyboard input
    glfwSetKeyCallback(window, key_callback);

    #if GL_API == 0 || GL_API == 1
    if (!init_opengl()) {
        fprintf(stderr, "Unable to initialize glad/opengl\n");
        return 1;
    }
    #elif GL_API == 2
    // TODO
    #endif

    #if GL_API == 0 || GL_API == 1
    const char *version = (const char*)glGetString(GL_VERSION);
    printf("OpenGL version: %s\n", version);
    #elif GL_API == 2
    // TODO: get Vulkan version
    const char *version = "???";
    printf("Vulkan version: %s\n", version);
    #endif



    #if GL_API == 0
    int msaaSamples = 0;
    glGetIntegerv(GL_SAMPLES, &msaaSamples);
    std::cout << "MSAA Samples: " << msaaSamples << "\n";
    #elif GL_API == 1
    // TODO
    #elif GL_API == 2
    // TODO
    int msaaSamples = 6969;
    std::cout << "MSAA Samples: " << msaaSamples << "\n";
    #endif

    // Load blocks
    loadBlocks();

    // load block meshes
    loadBlocksMeshes();





    // world size in chunks
    Vec3<u64> world_size = {4, 2, 4};
    Vec3<i64> world_chunk_center = world_size / 2;

    Vec2<u64> height_size = {world_size.x, world_size.z};
    Vec2<i64> world_chunk_height_center = height_size / 2;

    VoxelBlockWorld voxelBlockWorld = VoxelBlockWorld(world_size);
    VoxelBlockStateWorld voxelBlockStateWorld = VoxelBlockStateWorld(world_size);
    VoxelLightWorld voxelLightWorld = VoxelLightWorld(world_size);
    VoxelHeightWorld voxelHeightWorld = VoxelHeightWorld(height_size);
    VoxelWorldRenderer voxelWorldRenderer = VoxelWorldRenderer(world_size);
    
    // setup
    for (u64 y = 0; y < world_size.y; y++) {
        for (u64 x = 0; x < world_size.x; x++) {
            for (u64 z = 0; z < world_size.z; z++) {
                // game
                VoxelBlockChunk chunk = VoxelBlockChunk();
                chunk.pos = Vec3<i64>(x, y, z) - world_chunk_center;
                voxelBlockWorld.chunks[voxelBlockWorld.getChunkIndex(x, y, z)] = chunk;

                // block state
                BlockStateVoxelChunk blockStateChunk = BlockStateVoxelChunk();
                blockStateChunk.pos = Vec3<i64>(x, y, z) - world_chunk_center;
                voxelBlockStateWorld.chunks[voxelBlockStateWorld.getChunkIndex(x, y, z)] = blockStateChunk;

                // light
                voxelLightWorld.chunks[voxelLightWorld.getChunkIndex(x, y, z)] = VoxelLightChunk();

                // render
                VoxelChunkRenderer chunkRenderer = VoxelChunkRenderer();
                chunkRenderer.chunk = &voxelBlockWorld.chunks[voxelBlockWorld.getChunkIndex(x, y, z)];
                chunkRenderer.init();

                voxelWorldRenderer.chunks[voxelBlockWorld.getChunkIndex(x, y, z)] = chunkRenderer;
            }
        }
    }
    for (i64 x = 0; x < world_size.x; x++) {
        for (i64 z = 0; z < world_size.z; z++) {
            // height
            voxelHeightWorld.chunks[voxelHeightWorld.getChunkIndex(x, z)] = VoxelHeightChunk();
        }
    }

    // terrain generation
    // Noise noise = Noise();
    // noise.setSeed(646);
    // noise.updateNoise();
    // for (i64 y = 0; y < world_size.y; y++) {
    //     for (i64 x = 0; x < world_size.x; x++) {
    //         for (i64 z = 0; z < world_size.z; z++) {
    //             // game
    //             VoxelBlockChunk& chunk = voxelBlockWorld.chunks[voxelBlockWorld.getChunkIndex(x, y, z)];

    //             auto start = std::chrono::high_resolution_clock::now();
    //             noise.GenerateFullTerrain(chunk.voxels, x, y, z);
    //             auto end = std::chrono::high_resolution_clock::now();
    //             std::chrono::duration<double, std::milli> elapsed = end - start;
    //             std::cout << "Terrain gen: " << elapsed.count() << " ms\n";
    //         }
    //     }
    // }



    // flat grass
    for (i64 cx = -i64(world_size.x) / 2; cx < i64(world_size.x) / 2; cx++) {
        for (i64 cz = -i64(world_size.z) / 2; cz < i64(world_size.z) / 2; cz++) {
            if (cx == 0 && cz == 0) {
                VoxelWorlds::placeVoxel(voxelBlockWorld, cx * CS, 5, cz * CS, EmbeddedVoxel(BlockTypes::SNOW));
            } else if (cx == -2 && cz == 1) {
                VoxelWorlds::placeVoxel(voxelBlockWorld, cx * CS, 5, cz * CS, EmbeddedVoxel(BlockTypes::COBBLESTONE));
            } else {
                VoxelWorlds::placeVoxel(voxelBlockWorld, cx * CS, 5, cz * CS, EmbeddedVoxel(BlockTypes::OAK_PLANKS));
            }

            // game
            VoxelBlockChunk& chunk = voxelBlockWorld.chunks[voxelBlockWorld.chunkPosToChunkIndex(cx, 0, cz)];

            for (u8 x = 0; x < CS; x++) {
                for (u8 z = 0; z < CS; z++) {
                    chunk.voxels[get_zxy_index(x, 4, z)] = EmbeddedVoxel(BlockTypes::GRASS);
                    for (u8 y = 1; y < 4; y++) {
                        chunk.voxels[get_zxy_index(x, y, z)] = EmbeddedVoxel(BlockTypes::DIRT);
                    }
                    chunk.voxels[get_zxy_index(x, 0, z)] = EmbeddedVoxel(BlockTypes::STONE);
                }
            }
        }
    }

    // {
    //     // BlockStateStruct* state = new BlockStateStruct();
    //     // *state = StairBlockState();
        
    //     BlockStateStruct* state = new BlockStateStruct(StairBlockState());

    //     printf("%i\n", std::get<StairBlockState*>(*state)->direction);
    // }

    // TODO: free already existing block state. OR can you just override the value set there?



    {
        i64 start_x = -63;
        i64 start_z = 6;
        
        // oak planks
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 0, 5,  start_z, EmbeddedVoxel(BlockTypes::OAK_PLANKS));

        // block, slab and stair
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 2, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_PLANKS));

        // slabs
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 4, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_SLAB));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 4, 6,  start_z, BlockStateVoxel(new BlockStateStruct(SlabBlockState(0))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 8, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_SLAB));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 8, 6,  start_z, BlockStateVoxel(new BlockStateStruct(SlabBlockState(1))));

        // stairs
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 12, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 12, 6,  start_z, BlockStateVoxel(new BlockStateStruct(StairBlockState(0))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 16, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 16, 6,  start_z, BlockStateVoxel(new BlockStateStruct(StairBlockState(1))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 18, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 18, 6,  start_z, BlockStateVoxel(new BlockStateStruct(StairBlockState(2))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 20, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 20, 6,  start_z, BlockStateVoxel(new BlockStateStruct(StairBlockState(3))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 22, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 22, 6,  start_z, BlockStateVoxel(new BlockStateStruct(StairBlockState(4))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 24, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 24, 6,  start_z, BlockStateVoxel(new BlockStateStruct(StairBlockState(5))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 28, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 28, 6,  start_z, BlockStateVoxel(new BlockStateStruct(StairBlockState(6))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 30, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 30, 6,  start_z, BlockStateVoxel(new BlockStateStruct(StairBlockState(7))));

        // block that torches attaches to
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 32, 6,  0, EmbeddedVoxel(BlockTypes::OAK_PLANKS));

        // torch
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 34, 7,  start_z, EmbeddedVoxel(BlockTypes::TORCH));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 34, 7,  start_z, BlockStateVoxel(new BlockStateStruct(TorchBlockState(0))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 36, 6,  start_z, EmbeddedVoxel(BlockTypes::TORCH));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 36, 6,  start_z, BlockStateVoxel(new BlockStateStruct(TorchBlockState(1))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 38, 6,  start_z, EmbeddedVoxel(BlockTypes::TORCH));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 38, 6,  start_z, BlockStateVoxel(new BlockStateStruct(TorchBlockState(2))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 40, 6,  start_z + 1, EmbeddedVoxel(BlockTypes::TORCH));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 40, 6,  start_z + 1, BlockStateVoxel(new BlockStateStruct(TorchBlockState(3))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 42, 6,  start_z - 1, EmbeddedVoxel(BlockTypes::TORCH));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 42, 6,  start_z - 1, BlockStateVoxel(new BlockStateStruct(TorchBlockState(4))));

        // oak log
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 44, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_LOG));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 44, 6,  start_z, BlockStateVoxel(new BlockStateStruct(LogBlockState(0))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 46, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_LOG));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 46, 6,  start_z, BlockStateVoxel(new BlockStateStruct(LogBlockState(1))));
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 48, 6,  start_z, EmbeddedVoxel(BlockTypes::OAK_LOG));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 48, 6,  start_z, BlockStateVoxel(new BlockStateStruct(LogBlockState(2))));

        // glass panes
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 50, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 50, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b0000)))); // none

        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 52, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 52, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b0001)))); // right
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 54, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 54, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b0010)))); // left
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 56, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 56, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b0100)))); // back
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 58, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 58, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b1000)))); // front
        
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 60, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 60, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b0011)))); // right, left
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 62, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 62, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b1100)))); // back, front
        
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 64, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 64, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b0101)))); // right, back
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 66, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 66, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b0110)))); // back, left
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 68, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 68, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b1010)))); // left, front
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 70, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 70, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b1001)))); // front, right
        
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 72, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 72, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b0111)))); // right, back, left
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 74, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 74, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b1110)))); // back, left, front
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 76, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 76, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b1011)))); // left, front, right
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 78, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 78, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b1101)))); // front, right, back

        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 80, 6,  start_z, EmbeddedVoxel(BlockTypes::GLASS_PANE));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 80, 6,  start_z, BlockStateVoxel(new BlockStateStruct(GlassPaneBlockState(0b1111)))); // all

        // poppy
        VoxelWorlds::placeVoxel(voxelBlockWorld,      start_x + 84, 6,  start_z, EmbeddedVoxel(BlockTypes::POPPY));
        VoxelWorlds::placeVoxel(voxelBlockStateWorld, start_x + 84, 6,  start_z, BlockStateVoxel(new BlockStateStruct(BlockBlockState())));
    }



    {
        for (u64 x = 0; x < 16; x++) {
            for (u64 z = 0; z < 16; z++) {
                VoxelWorlds::placeVoxel(voxelBlockWorld, 20 + x, 10, -40 + z, EmbeddedVoxel(BlockTypes::OAK_PLANKS));
                VoxelWorlds::placeVoxel(voxelBlockWorld, -20 + x, 7, -40 + z, EmbeddedVoxel(BlockTypes::OAK_PLANKS));
            }
        }
    }






    {
        auto placeNBT = [&voxelBlockWorld, &voxelBlockStateWorld, &voxelLightWorld](std::string filePath, i32 placeX, i32 placeY, i32 placeZ) {
            // load house nbt
            std::ifstream file(filePath, std::ios::binary);
            if (!file) {
                std::cerr << "Failed to open file.\n";
                return 1;
            }

            std::vector<char> fileVec;
            if (isGzipped(file)) {
                fileVec = decompressGzipFile(file);
            } else {
                fileVec = streamToString(file);
            }

            NBTReader nbtReader(fileVec);
            NBT* houseNbt = nbtReader.parse();

            if (!houseNbt || houseNbt->tagType != TAG_Compound) {
                std::cerr << "Invalid or missing root compound tag.\n";
                return 1;
            }

            auto blocksTag = houseNbt->getCompoundTag("blocks");

            if (!blocksTag) {
                std::cerr << "Missing 'blocks' tag.\n";
                return 1;
            }

            auto paletteTag = houseNbt->getCompoundTag("palette");
            if (!paletteTag) {
                std::cerr << "Missing 'palette' tag.\n";
                return 1;
            }

            const auto& blocks = std::get<std::vector<NBT*>>(blocksTag->value);
            const auto& palette = std::get<std::vector<NBT*>>(paletteTag->value);

            for (const auto& block : blocks) {
                auto compound = std::get<std::map<std::string, NBT*>>(block->value);

                // Get block position
                const auto& pos = std::get<std::vector<NBT*>>(compound.at("pos")->value);
                u64 stateIndex = std::get<i32>(compound.at("state")->value);

                if (stateIndex >= 0 && stateIndex < palette.size()) {
                    auto stateTag = palette[stateIndex];
                    const auto& stateCompound = std::get<std::map<std::string, NBT*>>(stateTag->value);
                    std::string name = std::get<std::string>(stateCompound.at("Name")->value);

                    bool hasProperties = stateCompound.find("Properties") != stateCompound.end();
                    std::map<std::string, NBT*>* properties;
                    if (hasProperties) {
                        properties = &std::get<std::map<std::string, NBT*>>(stateCompound.at("Properties")->value);
                    }

                    if (name == "minecraft:air" || name == "minecraft:jigsaw" || name == "minecraft:oak_door" || name == "minecraft:white_bed" || name == "minecraft:yellow_bed" || name == "minecraft:chest" || name == "minecraft:oak_fence" || name == "minecraft:oak_pressure_plate") {
                        continue;
                    }

                    BlockType blockType;
                    if (name == "minecraft:grass_block") {
                        blockType = BlockTypes::GRASS;
                    } else if (name == "minecraft:dirt") {
                        blockType = BlockTypes::DIRT;
                    } else if (name == "minecraft:cobblestone") {
                        blockType = BlockTypes::COBBLESTONE;
                    } else if (name == "minecraft:oak_log") {
                        blockType = BlockTypes::OAK_LOG;
                    } else if (name == "minecraft:oak_planks") {
                        blockType = BlockTypes::OAK_PLANKS;
                    } else if (name == "minecraft:oak_slab") {
                        blockType = BlockTypes::OAK_SLAB;
                    } else if (name == "minecraft:oak_stairs") {
                        blockType = BlockTypes::OAK_STAIRS;
                    } else if (name == "minecraft:dirt_path") {
                        blockType = BlockTypes::DIRT;
                    } else if (name == "minecraft:oak_door") {
                        blockType = BlockTypes::OAK_PLANKS;
                    } else if (name == "minecraft:cobblestone_stairs") {
                        blockType = BlockTypes::COBBLESTONE_STAIRS;
                    } else if (name == "minecraft:wall_torch") {
                        blockType = BlockTypes::TORCH;
                    } else if (name == "minecraft:stripped_oak_log") {
                        blockType = BlockTypes::STRIPPED_OAK_LOG;
                    } else if (name == "minecraft:glass_pane") {
                        blockType = BlockTypes::GLASS_PANE;
                    } else {
                        std::cerr << "Unsupported block type: " << name << "\n";
                        return 1;
                    }

                    Vec3<i64> worldPos(
                        std::get<i32>(pos[0]->value) + placeX,
                        std::get<i32>(pos[1]->value) + placeY,
                        std::get<i32>(pos[2]->value) + placeZ
                    );

                    BlockVoxelData blockTemplate = BLOCK_VOXEL_DATA[blockType];

                    BlockStateStruct* newState = new BlockStateStruct();
                    if (blockTemplate.stateType == BlockStateTypes::BLOCK) {
                        *newState = BlockBlockState();
                    } else if (blockTemplate.stateType == BlockStateTypes::SLAB) {
                        if (hasProperties) {
                            std::string type = std::get<std::string>(properties->at("type")->value);
                            if (type == "double") {
                                // slab to full block
                                *newState = BlockBlockState();
                                blockType -= 1;
                            } else {
                                *newState = SlabBlockState();
                                std::get<SlabBlockState>(*newState).placement = type == "top" ? 1 : 0;
                            }
                        } else {
                            *newState = SlabBlockState();
                        }
                    } else if (blockTemplate.stateType == BlockStateTypes::STAIR) {
                        *newState = StairBlockState();
                        if (hasProperties) {
                            std::string half = std::get<std::string>(properties->at("half")->value);
                            std::string facing = std::get<std::string>(properties->at("facing")->value);
                            // NBT stairs are facing the "wrong" way
                            std::get<StairBlockState>(*newState).direction = (facing == "east" ? 1 : facing == "west" ? 0 : facing == "north" ? 2 : 3) + (half == "bottom" ? 0 : 4);
                        }
                    } else if (blockTemplate.stateType == BlockStateTypes::LOG) {
                        *newState = LogBlockState();
                        if (hasProperties) {
                            std::string axis = std::get<std::string>(properties->at("axis")->value);
                            std::get<LogBlockState>(*newState).direction = axis[0] - 'x';
                        }
                    } else if (blockTemplate.stateType == BlockStateTypes::TORCH) {
                        *newState = TorchBlockState();
                        if (hasProperties) {
                            std::string facing = std::get<std::string>(properties->at("facing")->value);
                            std::get<TorchBlockState>(*newState).direction = facing[0] - 'x';
                        }
                    } else if (blockTemplate.stateType == BlockStateTypes::GLASS_PANE) {
                        *newState = GlassPaneBlockState();
                        if (hasProperties) {
                            std::string east = std::get<std::string>(properties->at("east")->value);
                            std::string west = std::get<std::string>(properties->at("west")->value);
                            std::string south = std::get<std::string>(properties->at("south")->value);
                            std::string north = std::get<std::string>(properties->at("north")->value);
                            std::get<GlassPaneBlockState>(*newState).connections = (
                                (east == "true" ? 1 : 0) << 0 |
                                (west == "true" ? 1 : 0) << 1 |
                                (south == "true" ? 1 : 0) << 2 |
                                (north == "true" ? 1 : 0) << 3
                            );
                        }
                    }

                    VoxelWorlds::placeVoxel(voxelBlockWorld,      worldPos.x, worldPos.y, worldPos.z, EmbeddedVoxel(blockType));
                    VoxelWorlds::placeVoxel(voxelBlockStateWorld, worldPos.x, worldPos.y, worldPos.z, BlockStateVoxel(newState));
                }
            }

            return 0;
        };

        if (placeNBT("assets/structures/plains_big_house_1.nbt", -20, 5, 10)) return 1;
        if (placeNBT("assets/structures/plains_medium_house_1.nbt", -10, 5, 10)) return 1;
        if (placeNBT("assets/structures/plains_medium_house_2.nbt", 10, 5, 10)) return 1;
    }
    


    {
        // Add a small tree
        i64 x = -20;
        i64 y = 5;
        i64 z = -20;

        VoxelWorlds::placeVoxel(voxelBlockWorld,           x, y + 1, z, EmbeddedVoxel(BlockTypes::OAK_LOG));
        *VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, x, y + 1, z)->state = LogBlockState();
        VoxelWorlds::placeVoxel(voxelBlockWorld,           x, y + 2, z, EmbeddedVoxel(BlockTypes::OAK_LOG));
        *VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, x, y + 2, z)->state = LogBlockState();
        VoxelWorlds::placeVoxel(voxelBlockWorld,           x, y + 3, z, EmbeddedVoxel(BlockTypes::OAK_LOG));
        *VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, x, y + 3, z)->state = LogBlockState();
        VoxelWorlds::placeVoxel(voxelBlockWorld,           x, y + 4, z, EmbeddedVoxel(BlockTypes::OAK_LOG));
        *VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, x, y + 4, z)->state = LogBlockState();

        for (i8 cy = 0; cy < 2; cy++) {
            for (i8 cx = -2; cx < 3; cx++) {
                for (i8 cz = -2; cz < 3; cz++) {
                    if (cx == 0 && cz == 0) {
                        continue;
                    }
                    VoxelWorlds::placeVoxel(voxelBlockWorld,           x + cx, y + 1 + cy, z + cz, EmbeddedVoxel(BlockTypes::OAK_LEAVES));
                    *VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, x + cx, y + 1 + cy, z + cz)->state = BlockBlockState();
                }
            }
        }
        for (i8 cy = 0; cy < 2; cy++) {
            for (i8 cx = -1; cx < 2; cx++) {
                for (i8 cz = -1; cz < 2; cz++) {
                    if (cx == 0 && cz == 0) {
                        continue;
                    }
                    VoxelWorlds::placeVoxel(voxelBlockWorld,           x + cx, y + 3 + cy, z + cz, EmbeddedVoxel(BlockTypes::OAK_LEAVES));
                    *VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, x + cx, y + 3 + cy, z + cz)->state = BlockBlockState();
                }
            }
        }
        VoxelWorlds::placeVoxel(voxelBlockWorld,           x, y + 3 + 2, z, EmbeddedVoxel(BlockTypes::OAK_LEAVES));
        *VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, x, y + 3 + 2, z)->state = BlockBlockState();

        VoxelWorlds::placeVoxel(voxelBlockWorld,           x + -1, y + 3 + 1, z + -1, EmbeddedVoxel(BlockTypes::AIR));
        *VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, x + -1, y + 3 + 1, z + -1)->state = BlockBlockState();
        VoxelWorlds::placeVoxel(voxelBlockWorld,           x + -1, y + 3 + 1, z +  1, EmbeddedVoxel(BlockTypes::AIR));
        *VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, x + -1, y + 3 + 1, z +  1)->state = BlockBlockState();
        VoxelWorlds::placeVoxel(voxelBlockWorld,           x +  1, y + 3 + 1, z + -1, EmbeddedVoxel(BlockTypes::AIR));
        *VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, x +  1, y + 3 + 1, z + -1)->state = BlockBlockState();
        VoxelWorlds::placeVoxel(voxelBlockWorld,           x +  1, y + 3 + 1, z +  1, EmbeddedVoxel(BlockTypes::AIR));
        *VoxelWorlds::getVoxelUnsafe(voxelBlockStateWorld, x +  1, y + 3 + 1, z +  1)->state = BlockBlockState();
    }
    

    {
        VoxelWorlds::placeVoxel(voxelBlockWorld, 13, 5, -10, EmbeddedVoxel(BlockTypes::OAK_PLANKS));
        VoxelWorlds::placeVoxel(voxelBlockWorld, 11, 5, -10, EmbeddedVoxel(BlockTypes::OAK_PLANKS));
        VoxelWorlds::placeVoxel(voxelBlockWorld, 12, 5, -11, EmbeddedVoxel(BlockTypes::OAK_PLANKS));
        VoxelWorlds::placeVoxel(voxelBlockWorld, 12, 5,  -9, EmbeddedVoxel(BlockTypes::OAK_PLANKS));
        VoxelWorlds::placeVoxel(voxelBlockWorld, 12, 6, -10, EmbeddedVoxel(BlockTypes::OAK_PLANKS));
        auto pos = Vec3<i64>(12, 5, -10);
        ChunkLight::add_light(voxelBlockWorld, voxelLightWorld, pos, Colors::createRGBIS4(15, 15, 15, 0, 0));
    }

    {
        // parse .mca
        // parseMCA("assets/maps/notre_dame_medieval_city/region/r.1.0.mca");
    }

    // light
    for (u64 x = 0; x < world_size.x; x++) {
        for (u64 z = 0; z < world_size.z; z++) {
            VoxelHeightChunk& heightChunk = voxelHeightWorld.chunks[voxelHeightWorld.getChunkIndex(x, z)];
            heightChunk.pos = Vec2<i64>(x, z) - world_chunk_height_center;


            auto start = std::chrono::high_resolution_clock::now();

            heightChunk.calculateHeightmap(voxelBlockWorld, voxelBlockStateWorld);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;
            std::cout << "Heightmap gen: " << elapsed.count() << " ms\n";
        }
    }

    // light
    for (u64 x = 0; x < world_size.x; x++) {
        for (u64 z = 0; z < world_size.z; z++) {
            VoxelHeightChunk& heightChunk = voxelHeightWorld.chunks[voxelHeightWorld.getChunkIndex(x, z)];
            for (u64 y = 0; y < world_size.y; y++) {
                VoxelBlockChunk& blockChunk = voxelBlockWorld.chunks[voxelBlockWorld.getChunkIndex(x, y, z)];
                VoxelLightChunk& lightChunk = voxelLightWorld.chunks[voxelLightWorld.getChunkIndex(x, y, z)];

                auto start = std::chrono::high_resolution_clock::now();

                ChunkLight::apply_light(voxelBlockWorld, voxelHeightWorld, voxelLightWorld, blockChunk, heightChunk, lightChunk);

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> elapsed = end - start;
                std::cout << "Light gen: " << elapsed.count() << " ms\n";
            }
        }
    }


    // mesh
    for (u64 x = 0; x < world_size.x; x++) {
        for (u64 y = 0; y < world_size.y; y++) {
            for (u64 z = 0; z < world_size.z; z++) {
                VoxelChunkRenderer& chunk = voxelWorldRenderer.chunks[voxelWorldRenderer.getChunkIndex(x, y, z)];

                auto start = std::chrono::high_resolution_clock::now();

                chunk.generateMesh(voxelBlockWorld, voxelBlockStateWorld, voxelLightWorld);
            
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> elapsed = end - start;
                std::cout << "Mesh gen: " << elapsed.count() << " ms\n";
            }
        }
    }
    







    auto getBlockAndStateAtPos = [&voxelBlockWorld, &voxelBlockStateWorld](const Vec3<i64>& pos, EmbeddedVoxel** voxel, BlockStateVoxel** blockStateVoxel) {
        if (!VoxelWorlds::getVoxel(voxelBlockWorld, pos.x, pos.y, pos.z, voxel)) {
            printf("NO\n");
            return false;
        }
        if (!VoxelWorlds::getVoxel(voxelBlockStateWorld, pos.x, pos.y, pos.z, blockStateVoxel)) {
            printf("ERROR: BLOCKSTATE NOT FOUND WHEN BLOCK EXISTS\n");
            return false;
        }
        return true;
    };

    auto blockChangedAtPos = [&voxelBlockWorld, &voxelBlockStateWorld, &voxelLightWorld, &voxelWorldRenderer, &voxelHeightWorld](const Vec3<i64>& modifyPos, const EmbeddedVoxel* voxel, const BlockStateVoxel* blockStateVoxel) {
        voxelHeightWorld.updateHeightAtPos(modifyPos, *voxel, *blockStateVoxel);

        ChunkLight::remove_light(voxelBlockWorld, voxelLightWorld, modifyPos);
        
        Vec3<i64> blockChunkPos = voxelWorldRenderer.worldToChunkPos(modifyPos);
        VoxelChunkRenderer& blockChunk = voxelWorldRenderer.chunks[voxelWorldRenderer.chunkPosToChunkIndex(blockChunkPos)];
        blockChunk.generateMesh(voxelBlockWorld, voxelBlockStateWorld, voxelLightWorld);
    };







    camera = new Camera(Vec3<f32>(0, 0, 0));
    camera->handleResolution(WINDOW_WIDTH, WINDOW_HEIGHT);
  
    float forwardMove = 0.0f;
    float rightMove = 0.0f;
    float noclipSpeed = 10.0f;
  
    float deltaTime = 0.0f;
  
    auto lastFrame = glfwGetTime();









    









    #if GL_API == 0 || GL_API == 1
    // textures
    GLuint textureArray;
    glGenTextures(1, &textureArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

    // Allocate storage
    u16 numTextures = 100; // big number
    u8 texWidth = 16;
    u8 texHeight = 16;

    // skip first
    u16 texIdx = 1;

    u8 mipLevels = std::floor(std::log2(std::max(texWidth, texHeight))) + 1;
    #if GL_API == 0
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevels, GL_RGBA8, texWidth, texHeight, numTextures);
    #elif GL_API == 1
    for (int level = 0; level < mipLevels; ++level) {
        int w = texWidth >> level;
        int h = texHeight >> level;
        if (w < 1) w = 1;
        if (h < 1) h = 1;

        glTexImage3D(
            GL_TEXTURE_2D_ARRAY,
            level,              // mipmap level
            GL_RGBA8,           // internal format
            w,                  // width at this mip level
            h,                  // height at this mip level
            numTextures,        // number of layers in the array
            0,                  // border (must be 0)
            GL_RGBA,            // format of pixel data (set NULL, so format doesn't matter much here)
            GL_UNSIGNED_BYTE,   // data type of pixel data
            NULL                // no initial data, just allocate
        );
    }
    #endif
    

    // repeat, only matters on greedy meshing i think (but we dont use that anymore)
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // turn off texture smoothing
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



    // LOAD TEXTURES
    u32* block_textures_data = new u32[array_size(block_textures)]{0};

    for (u16 i = 1; i < array_size(block_textures); i++) {
        std::string path = "assets/textures/block/";
        path += block_textures[i];
        path += ".png";
        i32 nrChannels;
        load_texture(path.c_str(), texIdx++, texWidth, texHeight, nrChannels);
        block_textures_data[i] = nrChannels;
    }

    // what does mipmaps do? mipmaps does some anti aliasing stuff bluhhh
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);



    #if GL_API == 0
    // Create SSBO for texture metadata
    GLuint texture_ssbo;

    glGenBuffers(1, &texture_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, texture_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, array_size(block_textures) * sizeof(u32), block_textures_data, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, texture_ssbo);
    #elif GL_API == 1
    // Create array buffer for texture metadata
    GLuint texture_ubo;

    glGenBuffers(1, &texture_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, texture_ubo);
    glBufferData(GL_UNIFORM_BUFFER, array_size(block_textures) * sizeof(u32), block_textures_data, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, texture_ubo);
    #endif






    Shader geometryShader = Shader("voxel/main.vert", "voxel/main.frag");



    // block outline
    Shader blockOutlineShader = Shader("block_outline/main.vert", "block_outline/main.frag");

    BlockOutlineRenderer blockOutline;
    blockOutline.shader = blockOutlineShader;
    blockOutline.init();




    // ui

    // image shader
    Shader imageShader = Shader("image/main.vert", "image/main.frag");

    // crosshair
    CrosshairUI crosshairUI = CrosshairUI(40);


    // fps
    FpsCounterUI fpsUI;
    {
        u8* charSizes = new u8[256]{
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 3, 5, 5, 5, 5, 1, 3, 3, 3, 5, 1, 5, 1, 5,
            5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 4, 5, 4, 5,
            6, 5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5,
            5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 3, 5, 5,
            2, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 4, 2, 5, 5, 5,
            5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 3, 1, 3, 6, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5,
            0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 5, 0, 0, 0, 6, 6,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 5, 0,
            6, 5, 5, 5, 4, 4, 5, 6, 4, 2, 0, 6, 4, 4, 5, 0
        };

        TextRenderer textRenderer = TextRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 16, 8, 8, 16 * 8, 16 * 8, charSizes, Shader("text/main.vert", "text/main.frag"));

        int width, height, nrChannels;
        u8* data = stbi_load("assets/fonts/ascii.png", &width, &height, &nrChannels, 0);
        textRenderer.loadFont(data);
        stbi_image_free(data);

        fpsUI = FpsCounterUI(60, textRenderer);
    }



    // hotbar
    HotbarUI hotbarUI(Vec2<f32>(0.0f, WINDOW_HEIGHT / 2 - 20.0f), 0, 40.0f, imageShader);


    #elif GL_API == 2
    // TODO
    #endif

    bool hasLookBlock = false;
    Vec3<i64> lookBlockPos;
    u8 lookBlockDir;

    Vec3<f32> playerCamOffset(0.0f, 0.6f, 0.0f);

    AABB playerAABB(Vec3<f32>(0.0f, 10.0f, 0.0f), Vec3<f32>(0.6f, 1.8f, 0.6f));
    Vec3<f32> playerVel(0.0f, 0.0f, 0.0f);

    // 0 = creative (flying)
    // 1 = survival (moving with physics)
    u8 gamemode = 0;

    bool isInChat = false;


    Vec3<i64> lastCamBlockPos = camera->position;
    Vec3<i64> lastCamChunkPos = voxelBlockWorld.worldToChunkPos(lastCamBlockPos);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        if (deltaTime >= (f32)1 / 60) {
            lastFrame = currentFrame;

            // look block
            {
                RaycastResult raycastResult = raycast(voxelBlockWorld, camera->position, camera->front * 16);
                hasLookBlock = raycastResult.success;
                if (hasLookBlock) {
                    lookBlockPos = raycastResult.blockPos;
                    blockOutline.pos = lookBlockPos;
                    lookBlockDir = raycastResult.dir;
                }
            }

            // debugging / actions
            if (currentKeyStates[GLFW_KEY_ESCAPE] && wasKeyStateChanged(GLFW_KEY_ESCAPE)) {
                glfwSetWindowShouldClose(window, true);
            } else if (currentKeyStates[GLFW_KEY_X] && wasKeyStateChanged(GLFW_KEY_X)) {
                GLint lastPolyMode;
                glGetIntegerv(GL_POLYGON_MODE, &lastPolyMode);
                if (lastPolyMode == GL_FILL) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                } else if (lastPolyMode == GL_LINE) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                } else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }



            if (currentMouseButtonStates[GLFW_MOUSE_BUTTON_LEFT] && wasMouseButtonStateChanged(GLFW_MOUSE_BUTTON_LEFT)) {
                // break block
                if (hasLookBlock) {
                    EmbeddedVoxel* voxel;
                    BlockStateVoxel* blockStateVoxel;
                    if (getBlockAndStateAtPos(lookBlockPos, &voxel, &blockStateVoxel)) {
                        *voxel = EmbeddedVoxel(BlockTypes::AIR);
                        *blockStateVoxel->state = BlockBlockState();
                        
                        blockChangedAtPos(lookBlockPos, voxel, blockStateVoxel);
                    }
                }
            }
            if (currentMouseButtonStates[GLFW_MOUSE_BUTTON_RIGHT] && wasMouseButtonStateChanged(GLFW_MOUSE_BUTTON_RIGHT)) {
                // place block
                if (hasLookBlock) {
                    Vec3<i64> modifyPos = VoxelWorlds::addDirToVec(lookBlockPos, lookBlockDir);

                    EmbeddedVoxel* voxel;
                    BlockStateVoxel* blockStateVoxel;
                    if (getBlockAndStateAtPos(modifyPos, &voxel, &blockStateVoxel)) {
                        RGBIS4 light = *VoxelWorlds::getVoxelUnsafe(voxelLightWorld, modifyPos.x, modifyPos.y, modifyPos.z);

                        *voxel = EmbeddedVoxel(BlockTypes::COBBLESTONE);
                        *blockStateVoxel->state = BlockBlockState();

                        blockChangedAtPos(modifyPos, voxel, blockStateVoxel);
                    }
                }
            }

            if (currentKeyStates[GLFW_KEY_T] && wasKeyStateChanged(GLFW_KEY_T)) {
                isInChat = true;
            }

            if (currentKeyStates[GLFW_KEY_F] && wasKeyStateChanged(GLFW_KEY_F)) {
                gamemode = (gamemode + 1) % 2;
                if (gamemode == 1) {
                    playerAABB.pos = camera->position - playerCamOffset;
                    playerVel = Vec3<f32>(0.0f);
                }
            }
            
            if (currentKeyStates[GLFW_KEY_W]) forwardMove = 1.0f;
            else if (currentKeyStates[GLFW_KEY_S]) forwardMove = -1.0f;
            else forwardMove = 0.0f;

            if (currentKeyStates[GLFW_KEY_D]) rightMove = 1.0f;
            else if (currentKeyStates[GLFW_KEY_A]) rightMove = -1.0f;
            else rightMove = 0.0f;

            if (gamemode == 0) {
                auto wishdir = (camera->front * forwardMove) + (camera->right * rightMove);

                camera->position = camera->position + wishdir * noclipSpeed * deltaTime;
            } else {
                auto wishdir = (camera->front * forwardMove) + (camera->right * rightMove);
                wishdir.y = 0.0f;
                wishdir = wishdir.normalized();

                if (currentKeyStates[GLFW_KEY_SPACE] && wasKeyStateChanged(GLFW_KEY_SPACE)) {
                    playerVel.y += 12.0f;
                }

                playerVel = playerVel + Vec3<f32>(0.0f, -32.656f, 0.0f) * deltaTime;
                playerAABB.pos = playerAABB.pos + ((wishdir * 10.0f) + playerVel) * deltaTime;

                Intersection intersection = playerAABB.getIntersection(voxelBlockWorld);
                playerAABB.solveCollision(intersection);

                if (intersection.intersects) {
                    playerVel = Vec3<f32>(0.0f);
                }

                camera->position = playerAABB.pos + playerCamOffset;
            }
            

            // printf("(%f %f %f) (%f %f %f)\n", camera->front.x, camera->front.y, camera->front.z, camera->position.x, camera->position.y, camera->position.z);



            if (currentKeyStates[GLFW_KEY_1] && wasKeyStateChanged(GLFW_KEY_1)) hotbarUI.setSlot(0);
            else if (currentKeyStates[GLFW_KEY_2] && wasKeyStateChanged(GLFW_KEY_2)) hotbarUI.setSlot(1);
            else if (currentKeyStates[GLFW_KEY_3] && wasKeyStateChanged(GLFW_KEY_3)) hotbarUI.setSlot(2);
            else if (currentKeyStates[GLFW_KEY_4] && wasKeyStateChanged(GLFW_KEY_4)) hotbarUI.setSlot(3);
            else if (currentKeyStates[GLFW_KEY_5] && wasKeyStateChanged(GLFW_KEY_5)) hotbarUI.setSlot(4);
            else if (currentKeyStates[GLFW_KEY_6] && wasKeyStateChanged(GLFW_KEY_6)) hotbarUI.setSlot(5);
            else if (currentKeyStates[GLFW_KEY_7] && wasKeyStateChanged(GLFW_KEY_7)) hotbarUI.setSlot(6);
            else if (currentKeyStates[GLFW_KEY_8] && wasKeyStateChanged(GLFW_KEY_8)) hotbarUI.setSlot(7);
            else if (currentKeyStates[GLFW_KEY_9] && wasKeyStateChanged(GLFW_KEY_9)) hotbarUI.setSlot(8);






            // block / chunk sorting
            Vec3<i64> camBlockPos = camera->position;
            Vec3<i64> camChunkPos = voxelBlockWorld.worldToChunkPos(camBlockPos);

            if (camBlockPos != lastCamBlockPos) {
                // printf("sort (%i %i %i)\n", camBlockPos.x, camBlockPos.y, camBlockPos.z);

                for (u64 i = 0; i < voxelWorldRenderer.size.volume(); i++) {
                    VoxelChunkRenderer& chunk = voxelWorldRenderer.chunks[i];
                    if (camChunkPos == chunk.chunk->pos) {
                        Vec3<i64> pos = camBlockPos - (camChunkPos * CS);
                        chunk.sortWithin(pos);
                        break;
                    }
                }

                if (camChunkPos != lastCamChunkPos) {
                    // printf("sort chunks\n");
                    voxelWorldRenderer.sortChunks(camChunkPos);

                    for (u64 i = 0; i < voxelWorldRenderer.size.volume(); i++) {
                        VoxelChunkRenderer& chunk = voxelWorldRenderer.chunks[i];

                        // only rerender chunks that are NOT the new chunk AND are directly next to the new chunk
                        if (camChunkPos != chunk.chunk->pos && (chunk.chunk->pos - camChunkPos).abs().max() == 1) {
                            Vec3<i64> pos = camBlockPos - (chunk.chunk->pos * CS);
                            chunk.sortWithin(pos);

                            // Vec3<i64> pos = camChunkPos - chunk.chunk->pos;
                            // Vec3<i64> sortPos = Vec3<i64>(pos.x < 0 ? 0 : CS - 1, pos.y < 0 ? 0 : CS - 1, pos.z < 0 ? 0 : CS - 1);
                            // chunk.sortWithin(sortPos);

                            // TODO
                            // Vec3<i64> pos = camChunkPos - chunk.chunk->pos;
                            // chunk.sortNeighbor(sign(pos.x), sign(pos.y), sign(pos.z));
                        }
                    }
                }
            }

            lastCamBlockPos = camBlockPos;
            lastCamChunkPos = camChunkPos;






            


            auto renderStart = glfwGetTime();
            // --------------------------------------
            // GEOMETRY PASS
            #if GL_API == 0 || GL_API == 1
            // glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_DEPTH_TEST);
            #elif GL_API == 2
            // TODO
            #endif

            // rendering
            f32 proj_mat[16];
            camera->projection.toGLMatrix(proj_mat);

            f32 view_mat[16];
            camera->getViewMatrix().toGLMatrix(view_mat);

            #if GL_API == 0 || GL_API == 1
            geometryShader.use();
            #elif GL_API == 2
            // TODO
            #endif

            #if GL_API == 0 || GL_API == 1
            geometryShader.setMat4("u_projection", proj_mat);
            geometryShader.setMat4("u_view", view_mat);
            geometryShader.setVec3("eye_position", camera->position);
            #elif GL_API == 2
            // TODO
            #endif


            #if GL_API == 0 || GL_API == 1
            geometryShader.setIVec3("eye_position_int", camChunkPos);
            #elif GL_API == 2
            // TODO
            #endif

            #if GL_API == 0 || GL_API == 1
            // bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
            geometryShader.setInt("texArray", 0);

            // ao
            geometryShader.setInt("gPosition", 1);
            geometryShader.setInt("gNormal", 2);
            geometryShader.setInt("texNoise", 3);
            #elif GL_API == 2
            // TODO
            #endif

            #if GL_API == 0 || GL_API == 1
            voxelWorldRenderer.render(geometryShader);

            if (hasLookBlock) {
                blockOutline.render(proj_mat, view_mat);
            }

            // glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // --------------------------------------
            #elif GL_API == 2
            // TODO
            #endif


            auto renderEnd = glfwGetTime();


            // ui
            glDisable(GL_DEPTH_TEST);



            // crosshair
            crosshairUI.render(WINDOW_WIDTH, WINDOW_HEIGHT);
            

            // text
            
            // fps
            fpsUI.addFrame(renderEnd - renderStart);
            fpsUI.render(WINDOW_WIDTH, WINDOW_HEIGHT);



            // hotbar
            hotbarUI.render(WINDOW_WIDTH, WINDOW_HEIGHT);







        
            glfwSwapBuffers(window);

            updateLastMouseButtonStates();
            updateLastKeyStates();
        }
        
        glfwPollEvents();
    }












    return 0;
}