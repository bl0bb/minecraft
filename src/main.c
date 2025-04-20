#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "renderer_3d/renderer_3d.h"
#include "core/maths.h"
#include "file_parsers/obj_parser.h"









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

mesh load_mesh_from_file(char* file_path) {
    mesh out_mesh;

    FILE* mesh_ptr = fopen(file_path, "rb");
    if (!mesh_ptr) {
        perror("Failed to open mesh");
        return out_mesh;
    }

    int32_t size = get_file_size(mesh_ptr);

    char* mesh_str = write_file_into_buffer(mesh_ptr, size);
    if (!mesh_str) {
        perror("Failed to allocate memory");
        return out_mesh;
    }

    out_mesh = objparser_parse_obj(size, mesh_str);

    // TODO: why cant i close above parser, but i can here???
    fclose(mesh_ptr);

    return out_mesh;
}









int main() {
    // TODO: add quad support for rendering and for obj importing??

    renderer_3d* renderer = renderer_3d_create(1024, 1024, "Test");



    // f32 cube_vertices[] = {
    //     -0.5f, -0.5f, -0.5f,
    //      0.5f, -0.5f, -0.5f,
    //      0.5f,  0.5f, -0.5f,
    //      0.5f,  0.5f, -0.5f,
    //     -0.5f,  0.5f, -0.5f,
    //     -0.5f, -0.5f, -0.5f,

    //     -0.5f, -0.5f,  0.5f,
    //      0.5f, -0.5f,  0.5f,
    //      0.5f,  0.5f,  0.5f,
    //      0.5f,  0.5f,  0.5f,
    //     -0.5f,  0.5f,  0.5f,
    //     -0.5f, -0.5f,  0.5f,

    //     -0.5f,  0.5f,  0.5f,
    //     -0.5f,  0.5f, -0.5f,
    //     -0.5f, -0.5f, -0.5f,
    //     -0.5f, -0.5f, -0.5f,
    //     -0.5f, -0.5f,  0.5f,
    //     -0.5f,  0.5f,  0.5f,

    //      0.5f,  0.5f,  0.5f,
    //      0.5f,  0.5f, -0.5f,
    //      0.5f, -0.5f, -0.5f,
    //      0.5f, -0.5f, -0.5f,
    //      0.5f, -0.5f,  0.5f,
    //      0.5f,  0.5f,  0.5f,

    //     -0.5f, -0.5f, -0.5f,
    //      0.5f, -0.5f, -0.5f,
    //      0.5f, -0.5f,  0.5f,
    //      0.5f, -0.5f,  0.5f,
    //     -0.5f, -0.5f,  0.5f,
    //     -0.5f, -0.5f, -0.5f,

    //     -0.5f,  0.5f, -0.5f,
    //      0.5f,  0.5f, -0.5f,
    //      0.5f,  0.5f,  0.5f,
    //      0.5f,  0.5f,  0.5f,
    //     -0.5f,  0.5f,  0.5f,
    //     -0.5f,  0.5f, -0.5f,
    // };



    // f32 pyramid_vertices[] = {
    //     // Base (square, two triangles)
    //     -1.0, 0.0, -1.0,  // 0 - back left
    //     1.0, 0.0, -1.0,  // 1 - back right
    //     1.0, 0.0,  1.0,  // 2 - front right

    //     -1.0, 0.0, -1.0,  // 0 - back left
    //     1.0, 0.0,  1.0,  // 2 - front right
    //     -1.0, 0.0,  1.0,  // 3 - front left

    //     // Side 1 (back)
    //     -1.0, 0.0, -1.0,  // 0 - base
    //     1.0, 0.0, -1.0,  // 1 - base
    //     0.0, 1.5,  0.0,  // 4 - tip

    //     // Side 2 (right)
    //     1.0, 0.0, -1.0,  // 1 - base
    //     1.0, 0.0,  1.0,  // 2 - base
    //     0.0, 1.5,  0.0,  // 4 - tip

    //     // Side 3 (front)
    //     1.0, 0.0,  1.0,  // 2 - base
    //     -1.0, 0.0,  1.0,  // 3 - base
    //     0.0, 1.5,  0.0,  // 4 - tip

    //     // Side 4 (left)
    //     -1.0, 0.0,  1.0,  // 3 - base
    //     -1.0, 0.0, -1.0,  // 0 - base
    //     0.0, 1.5,  0.0,   // 4 - tip
    // };


    // object_3d cube_object;
    // mat4_identity(&cube_object.transform);
    // mat4_translate_world(&cube_object.transform, -1, 0, 0);
    // cube_object.vertices_count = 108;
    // cube_object.vertices = malloc(sizeof(f32) * cube_object.vertices_count);
    // for (u16 i = 0; i < cube_object.vertices_count; i++) {
    //     cube_object.vertices[i] = cube_vertices[i];
    // }
    // cube_object.shader_program = renderer->shader_programs[0];
    // renderer_3d_add_object(renderer, &cube_object);


    // object_3d pyramid_object;
    // mat4_identity(&pyramid_object.transform);
    // mat4_translate_world(&pyramid_object.transform, 1, 0, 0);
    // pyramid_object.vertices_count = 54;
    // pyramid_object.vertices = malloc(sizeof(f32) * pyramid_object.vertices_count);
    // for (u16 i = 0; i < pyramid_object.vertices_count; i++) {
    //     pyramid_object.vertices[i] = pyramid_vertices[i];
    // }
    // pyramid_object.shader_program = renderer->shader_programs[0];
    // renderer_3d_add_object(renderer, &pyramid_object);





    renderer->shader_programs[0] = renderer_3d_compileShader("assets/shaders/texture/vertex.glsl", "assets/shaders/texture/fragment.glsl");






    // Load and create a texture
    int width, height, nrChannels;
    u8 *data = stbi_load("assets/textures/maxwell_base.jpg", &width, &height, &nrChannels, 0);
    renderer->textures[0] = renderer_3d_create_texture(data, width, height, nrChannels);
    stbi_image_free(data);





    mesh cube_mesh = load_mesh_from_file("assets/models/maxwell.obj");

    object_3d cube_object;
    mat4_identity(&cube_object.transform);
    // mat4_translate_world(&cube_object.transform, -1, 0, 0);

    cube_object.vertices_count = cube_mesh.vertex_data_count;
    cube_object.vertices = malloc(sizeof(f32) * cube_object.vertices_count);
    memcpy(cube_object.vertices, cube_mesh.vertex_data, sizeof(f32) * cube_object.vertices_count);

    cube_object.indices_count = cube_mesh.indices_count;
    cube_object.indices = malloc(sizeof(u32) * cube_object.indices_count);
    memcpy(cube_object.indices, cube_mesh.indices, sizeof(u32) * cube_object.indices_count);
    
    cube_object.shader_program = renderer->shader_programs[0];
    cube_object.texture = renderer->textures[0];
    renderer_3d_add_object(renderer, &cube_object);












    f64 last_time = glfwGetTime();
    f64 accumulator = 0.0f;

    // Main render loop
    while (!glfwWindowShouldClose(renderer->window)) {
        f64 current_time = glfwGetTime();
        f64 delta_time = current_time - last_time;
        accumulator += delta_time;

        if (accumulator >= 1.0 / 60.0) {
            accumulator = 0;
            renderer_3d_run(renderer);
        }

        last_time = current_time;
    }

    renderer_3d_free(renderer);

    return 0;
}