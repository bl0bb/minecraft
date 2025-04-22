#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "renderer_3d/renderer_3d.h"
#include "core/maths.h"
#include "core/camera.h"
#include "core/shader.h"
// #include "file_parsers/obj_parser.h"

#include "voxel/voxel_mesher.h"
#include "voxel/voxel_renderer.h"









i32 get_file_size(FILE* file) {
    fseek(file, 0, SEEK_END); // seek to end of file
    i32 size = ftell(file); // get current file pointer
    fseek(file, 0, SEEK_SET); // seek back to beginning of file
    return size;
}

char* write_file_into_buffer(FILE* fp, size_t size) {
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        perror("Failed to allocate memory");
        return NULL;
    }

    size_t bytesRead = fread(buffer, 1, size, fp);
    buffer[bytesRead] = 0;

    return buffer;
}

// mesh load_mesh_from_file(char* file_path) {
//     mesh out_mesh;

//     FILE* mesh_ptr = fopen(file_path, "rb");
//     if (!mesh_ptr) {
//         perror("Failed to open mesh");
//         return out_mesh;
//     }

//     int32_t size = get_file_size(mesh_ptr);

//     char* mesh_str = write_file_into_buffer(mesh_ptr, size);
//     if (!mesh_str) {
//         perror("Failed to allocate memory");
//         return out_mesh;
//     }

//     out_mesh = objparser_parse_obj(size, mesh_str);

//     // TODO: why cant i close above parser, but i can here???
//     fclose(mesh_ptr);

//     return out_mesh;
// }










inline int get_zxy_index(int x, int y, int z) {
    return z + (x * CS_P) + (y * CS_P2);
}



struct ChunkRenderData {
    Vec3<i64> chunkPos = Vec3<i64>(0, 0, 0);
    std::vector<DrawElementsIndirectCommand*> faceDrawCommands = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
};










constexpr u16 WINDOW_WIDTH = 1024;
constexpr u16 WINDOW_HEIGHT = 1024;

int main() {
    // TODO: add quad support for rendering and for obj importing??

    Renderer3D renderer(WINDOW_WIDTH, WINDOW_HEIGHT, "Test");



    // renderer.shader_programs.push_back(Renderer3D::compile_shader("assets/shaders/voxel/main.vs", "assets/shaders/voxel/main.fs"));

    f64 last_time = glfwGetTime();
    f64 accumulator = 0.0f;








    MeshData mainThreadMeshData;
    mainThreadMeshData.opaqueMask = new uint64_t[CS_P2] { 0 };
    mainThreadMeshData.faceMasks = new uint64_t[CS_2 * 6] { 0 };
    mainThreadMeshData.forwardMerged = new uint8_t[CS_2] { 0 };
    mainThreadMeshData.rightMerged = new uint8_t[CS] { 0 };
    mainThreadMeshData.vertices = new std::vector<uint64_t>(10000);
    mainThreadMeshData.maxVertices = 10000;

    ChunkRenderer chunkRenderer;
    std::vector<ChunkRenderData> chunkRenderData;
    chunkRenderData.push_back(ChunkRenderData());





    u8* voxels = new u8[CS_P3] {0};
    memset(voxels, 0, CS_P3);
    memset(mainThreadMeshData.opaqueMask, 0, CS_P2 * sizeof(u64));

    int r = CS_P / 2;
    for (int x = -r; x < r; x++) {
        for (int y = -r; y < r; y++) {
            for (int z = -r; z < r; z++) {
                if (std::sqrt(x * x + y * y + z * z) < 30.0f) {
                    voxels[get_zxy_index(x + r, y + r, z + r)] = 8;
                    mainThreadMeshData.opaqueMask[((y + r) * CS_P) + (x + r)] |= 1ull << (z + r);
                }
            }
        }
    }

    generate_voxel_mesh(voxels, mainThreadMeshData);







    








    



    // Shader* shader = new Shader("voxel", "voxel");
    // Camera* camera = new Camera(Vec3<f32>(0, 0, 0));
    // camera->handleResolution(WINDOW_WIDTH, WINDOW_HEIGHT);



    // // Main render loop
    // while (!glfwWindowShouldClose(renderer.window)) {
    //     f64 current_time = glfwGetTime();
    //     f64 delta_time = current_time - last_time;
    //     accumulator += delta_time;

    //     if (accumulator >= 1.0 / 60.0) {
    //         accumulator = 0;
            






    //         Vec3<i64> cameraChunkPos = camera->position / CS;

    //         for (const auto& data : chunkRenderData) {
    //             for (int i = 0; i < 6; i++) {
    //                 auto& d = data.faceDrawCommands[i];
    //                 printf("yewssss %i\n", d);
    //                 if (d) {
    //                     switch (i) {
    //                         case 0:
    //                         if (cameraChunkPos.y >= data.chunkPos.y) {
    //                             chunkRenderer.addDrawCommand(*d);
    //                         }
    //                         break;
                
    //                         case 1:
    //                         if (cameraChunkPos.y <= data.chunkPos.y) {
    //                             chunkRenderer.addDrawCommand(*d);
    //                         }
    //                         break;
                
    //                         case 2:
    //                         if (cameraChunkPos.x >= data.chunkPos.x) {
    //                             chunkRenderer.addDrawCommand(*d);
    //                         }
    //                         break;
                
    //                         case 3:
    //                         if (cameraChunkPos.x <= data.chunkPos.x) {
    //                             chunkRenderer.addDrawCommand(*d);
    //                         }
    //                         break;
                
    //                         case 4:
    //                         if (cameraChunkPos.z >= data.chunkPos.z) {
    //                             chunkRenderer.addDrawCommand(*d);
    //                         }
    //                         break;
                
    //                         case 5:
    //                         if (cameraChunkPos.z <= data.chunkPos.z) {
    //                             chunkRenderer.addDrawCommand(*d);
    //                         }
    //                         break;
    //                     }
    //                 }
    //             }
    //         }
        
    //         chunkRenderer.render(*shader, *camera);
        
    //         glfwSwapBuffers(renderer.window);
    //         glfwPollEvents();
    //     }

    //     last_time = current_time;
    // }

    // TODO: free renderer?

    return 0;
}