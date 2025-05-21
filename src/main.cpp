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
#include "voxel/light/voxel_light_world.h"

#include "voxel/render/voxel_chunk_renderer.h"
#include "voxel/render/voxel_world_renderer.h"

#include "core/array.h"
#include "blocks/block.h"

#include "shading/ambient_occlusion.h"
#include "quad.h"

#include "terrain_gen/terrain_gen.h"

#include "text/text_renderer.h"

#include "file_parsers/nbt_parser.h"
#include "nbt/nbt.h"

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
}

#if GL_API == 0
bool init_opengl() {
    glEnable(GL_DEBUG_OUTPUT);

    // this not working on mac
    // or is it?
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback(message_callback, 0);

    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glClearColor(0.529f, 0.808f, 0.922f, 0.0f);

    glEnable(GL_MULTISAMPLE);

    return true;
}
#elif GL_API == 1
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

    #if GL_API == 0
    if (!gladLoadGL()) {
        fprintf(stderr, "Unable to initialize glad\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }
    #elif GL_API == 1
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

    #if GL_API == 0
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
    #elif GL_API == 1
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

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    #if GL_API == 0
    if (!init_opengl()) {
        fprintf(stderr, "Unable to initialize glad/opengl\n");
        return 1;
    }
    #elif GL_API == 1
    // TODO
    #endif

    #if GL_API == 0
    const char *version = (const char*)glGetString(GL_VERSION);
    printf("OpenGL version: %s\n", version);
    #elif GL_API == 1
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
    int msaaSamples = 6969;
    std::cout << "MSAA Samples: " << msaaSamples << "\n";
    #endif

    // Load blocks
    loadBlocks();

    // load block meshes
    loadBlocksMeshes();





    // world size in chunks
    Vec3<u64> world_size = {2, 2, 2};
    Vec3<i64> world_chunk_center = world_size / 2;

    VoxelBlockWorld voxelBlockWorld = VoxelBlockWorld(world_size);
    VoxelBlockStateWorld voxelBlockStateWorld = VoxelBlockStateWorld(world_size);
    VoxelLightWorld voxelLightWorld = VoxelLightWorld(world_size);
    VoxelWorldRenderer voxelWorldRenderer = VoxelWorldRenderer(world_size);
    
    // setup
    for (i64 y = 0; y < world_size.y; y++) {
        for (i64 x = 0; x < world_size.x; x++) {
            for (i64 z = 0; z < world_size.z; z++) {
                // game
                VoxelBlockChunk chunk = VoxelBlockChunk();
                chunk.pos = Vec3<i64>(x, y, z) - world_chunk_center;
                voxelBlockWorld.chunks[voxelBlockWorld.getChunkIndex(x, y, z)] = chunk;

                // block state
                BlockStateVoxelChunk blockStateChunk = BlockStateVoxelChunk();
                blockStateChunk.pos = Vec3<i64>(x, y, z) - world_chunk_center;
                voxelBlockStateWorld.chunks[voxelBlockStateWorld.getChunkIndex(x, y, z)] = blockStateChunk;

                // render
                VoxelChunkRenderer chunkRenderer = VoxelChunkRenderer();
                chunkRenderer.init();
                chunkRenderer.chunk = &voxelBlockWorld.chunks[voxelBlockWorld.getChunkIndex(x, y, z)];

                voxelWorldRenderer.chunks[voxelBlockWorld.getChunkIndex(x, y, z)] = chunkRenderer;

                // light
                voxelLightWorld.chunks[voxelLightWorld.getChunkIndex(x, y, z)] = VoxelLightChunk();
            }
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

    // block, slab and stair
    VoxelWorlds::placeVoxel(voxelBlockWorld,      -12, 6,  0, EmbeddedVoxel(BlockTypes::OAK_PLANKS));

    VoxelWorlds::placeVoxel(voxelBlockWorld,      -10, 6,  0, EmbeddedVoxel(BlockTypes::OAK_SLAB));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld, -10, 6,  0, BlockStateVoxel(new BlockStateStruct(SlabBlockState(0))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,       -8, 6,  0, EmbeddedVoxel(BlockTypes::OAK_SLAB));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,  -8, 6,  0, BlockStateVoxel(new BlockStateStruct(SlabBlockState(1))));

    VoxelWorlds::placeVoxel(voxelBlockWorld,       -6, 6,  0, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,  -6, 6,  0, BlockStateVoxel(new BlockStateStruct(StairBlockState(0))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,       -4, 6,  0, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,  -4, 6,  0, BlockStateVoxel(new BlockStateStruct(StairBlockState(1))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,       -2, 6,  0, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,  -2, 6,  0, BlockStateVoxel(new BlockStateStruct(StairBlockState(2))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,        0, 6,  0, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,   0, 6,  0, BlockStateVoxel(new BlockStateStruct(StairBlockState(3))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,        2, 6,  0, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,   2, 6,  0, BlockStateVoxel(new BlockStateStruct(StairBlockState(4))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,        4, 6,  0, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,   4, 6,  0, BlockStateVoxel(new BlockStateStruct(StairBlockState(5))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,        6, 6,  0, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,   6, 6,  0, BlockStateVoxel(new BlockStateStruct(StairBlockState(6))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,        8, 6,  0, EmbeddedVoxel(BlockTypes::OAK_STAIRS));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,   8, 6,  0, BlockStateVoxel(new BlockStateStruct(StairBlockState(7))));

    VoxelWorlds::placeVoxel(voxelBlockWorld,       12, 6,  0, EmbeddedVoxel(BlockTypes::OAK_PLANKS));

    VoxelWorlds::placeVoxel(voxelBlockWorld,       12, 7,  0, EmbeddedVoxel(BlockTypes::TORCH));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,  12, 7,  0, BlockStateVoxel(new BlockStateStruct(TorchBlockState(0))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,       13, 6,  0, EmbeddedVoxel(BlockTypes::TORCH));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,  13, 6,  0, BlockStateVoxel(new BlockStateStruct(TorchBlockState(1))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,       11, 6,  0, EmbeddedVoxel(BlockTypes::TORCH));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,  11, 6,  0, BlockStateVoxel(new BlockStateStruct(TorchBlockState(2))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,       12, 6,  1, EmbeddedVoxel(BlockTypes::TORCH));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,  12, 6,  1, BlockStateVoxel(new BlockStateStruct(TorchBlockState(3))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,       12, 6, -1, EmbeddedVoxel(BlockTypes::TORCH));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,  12, 6, -1, BlockStateVoxel(new BlockStateStruct(TorchBlockState(4))));

    VoxelWorlds::placeVoxel(voxelBlockWorld,       16, 6,  0, EmbeddedVoxel(BlockTypes::OAK_LOG));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,  16, 6,  0, BlockStateVoxel(new BlockStateStruct(LogBlockState(0))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,       18, 6,  0, EmbeddedVoxel(BlockTypes::OAK_LOG));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,  18, 6,  0, BlockStateVoxel(new BlockStateStruct(LogBlockState(1))));
    VoxelWorlds::placeVoxel(voxelBlockWorld,       20, 6,  0, EmbeddedVoxel(BlockTypes::OAK_LOG));
    VoxelWorlds::placeVoxel(voxelBlockStateWorld,  20, 6,  0, BlockStateVoxel(new BlockStateStruct(LogBlockState(2))));







    {
        auto placeNBT = [&voxelBlockWorld, &voxelBlockStateWorld](std::string filePath, i32 placeX, i32 placeY, i32 placeZ) {
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
                int stateIndex = std::get<i32>(compound.at("state")->value);

                if (stateIndex >= 0 && stateIndex < palette.size()) {
                    auto stateTag = palette[stateIndex];
                    const auto& stateCompound = std::get<std::map<std::string, NBT*>>(stateTag->value);
                    std::string name = std::get<std::string>(stateCompound.at("Name")->value);

                    bool hasProperties = stateCompound.find("Properties") != stateCompound.end();
                    std::map<std::string, NBT*>* properties;
                    if (hasProperties) {
                        properties = &std::get<std::map<std::string, NBT*>>(stateCompound.at("Properties")->value);
                    }

                    if (name == "minecraft:air" || name == "minecraft:jigsaw" || name == "minecraft:oak_door" || name == "minecraft:glass_pane" || name == "minecraft:white_bed" || name == "minecraft:yellow_bed" || name == "minecraft:chest" || name == "minecraft:oak_fence" || name == "minecraft:oak_pressure_plate") {
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
                    } else {
                        std::cerr << "Unsupported block type: " << name << "\n";
                        return 1;
                    }

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
                    }

                    i32 x = std::get<i32>(pos[0]->value);
                    i32 y = std::get<i32>(pos[1]->value);
                    i32 z = std::get<i32>(pos[2]->value);

                    x += placeX;
                    y += placeY;
                    z += placeZ;

                    VoxelWorlds::placeVoxel(voxelBlockWorld, x, y, z, EmbeddedVoxel(blockType));
                    VoxelWorlds::placeVoxel(voxelBlockStateWorld, x, y, z, BlockStateVoxel(newState));
                }
            }

            return 0;
        };

        if (placeNBT("assets/structures/plains_big_house_1.nbt", -20, 5, 10)) return 1;
        if (placeNBT("assets/structures/plains_medium_house_1.nbt", -10, 5, 10)) return 1;
        if (placeNBT("assets/structures/plains_medium_house_2.nbt", 10, 5, 10)) return 1;
    }
    

    



    // mesh / light
    for (i64 x = 0; x < world_size.x; x++) {
        for (i64 y = 0; y < world_size.y; y++) {
            for (i64 z = 0; z < world_size.z; z++) {
                // render
                VoxelChunkRenderer& chunk = voxelWorldRenderer.chunks[voxelWorldRenderer.getChunkIndex(x, y, z)];

                auto start = std::chrono::high_resolution_clock::now();

                chunk.generateMesh(voxelBlockWorld, voxelBlockStateWorld);
            
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> elapsed = end - start;
                std::cout << "Mesh gen: " << elapsed.count() << " ms\n";

                // light
                VoxelLightChunk& lightChunk = voxelLightWorld.chunks[voxelLightWorld.getChunkIndex(x, y, z)];
            }
        }
    }


    







    camera = new Camera(Vec3<f32>(0, 0, 0));
    camera->handleResolution(WINDOW_WIDTH, WINDOW_HEIGHT);
  
    float forwardMove = 0.0f;
    float rightMove = 0.0f;
    float noclipSpeed = 50.0f;
  
    float deltaTime = 0.0f;
  
    auto lastFrame = glfwGetTime();








    









    #if GL_API == 0
    // texture ssbo
    GLuint texture_ssbo;

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

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, std::floor(std::log2(std::max(texWidth, texHeight))) + 1, GL_RGBA8, texWidth, texHeight, numTextures);

    

    // repeat, only matters on greedy meshing i think (but we dont use that anymore)
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // turn off texture smoothing
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



    // LOAD TEXTURES
    u32* block_textures_data = new u32[array_size(block_textures)]{0};

    for (u16 i = 1; i < array_size(block_textures); i++) {
        std::string path = "assets/textures/";
        path += block_textures[i];
        path += ".png";
        i32 nrChannels;
        load_texture(path.c_str(), texIdx++, texWidth, texHeight, nrChannels);
        block_textures_data[i] = nrChannels;
    }

    // mipmap does some anti aliasing stuff bluhhh
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);


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
    #elif GL_API == 1
    // TODO
    #endif


    
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

            #if GL_API == 0
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
            #elif GL_API == 1
            // TODO
            #endif
            

            // printf("(%f %f %f) (%f %f %f)\n", camera->front.x, camera->front.y, camera->front.z, camera->position.x, camera->position.y, camera->position.z);

            


            // --------------------------------------
            // GEOMETRY PASS
            #if GL_API == 0
            // glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            #elif GL_API == 1
            // TODO
            #endif
            Vec3<i64> cameraChunkPos = camera->position / CS;

            // rendering
            f32 proj_mat[16];
            camera->projection.toGLMatrix(proj_mat);

            f32 view_mat[16];
            camera->getViewMatrix().toGLMatrix(view_mat);

            #if GL_API == 0
            Shader* activeShader;
            if (shaderType == 0 || true) {
                activeShader = &geometryShader;
            } else if (shaderType == 1) {
                activeShader = &edgeShader;
            }
            activeShader->use();
            #elif GL_API == 1
            // TODO
            #endif

            #if GL_API == 0
            activeShader->setMat4("u_projection", proj_mat);
            activeShader->setMat4("u_view", view_mat);
            activeShader->setVec3("eye_position", camera->position);
            #elif GL_API == 1
            // TODO
            #endif

            Vec3<i64> intCamPosition = camera->position;
            #if GL_API == 0
            activeShader->setIVec3("eye_position_int", intCamPosition);
            #elif GL_API == 1
            // TODO
            #endif

            #if GL_API == 0
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
            #elif GL_API == 1
            // TODO
            #endif

            #if GL_API == 0
            voxelWorldRenderer.render(geometryShader);
            #elif GL_API == 1
            // TODO
            #endif

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