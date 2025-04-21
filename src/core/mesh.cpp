#include <stdlib.h>
#include <stdio.h>

#include "hashmap.h"

#include "mesh.h"

bool mesh_create(mesh* out_mesh, u16 vertices_count, vec3* vertices, u16 tex_coords_count, vec2* tex_coords, u16 normals_count, vec3* normals, u16 faces_count, u32*** faces) {
    out_mesh->vertex_data_count = faces_count * 3 * 8;
    out_mesh->vertex_data = malloc(sizeof(f32) * out_mesh->vertex_data_count);
    memset(out_mesh->vertex_data, 0, sizeof(f32) * out_mesh->vertex_data_count);

    out_mesh->indices_count = faces_count * 3;
    out_mesh->indices = malloc(sizeof(u32) * out_mesh->indices_count);
    memset(out_mesh->indices, 0, sizeof(u32) * out_mesh->indices_count);

    Hashmap* existing_indices = hashmap_create();

    u16 i;
    u16 vertex_idx = 0;
    u16 index_idx = 0;
    for (i = 0; i < faces_count; i++) {
        u32** face = faces[i];

        for (u8 j = 0; j < 3; j++) {
            int len1 = (face[j][0] - 1) ? ((int)log10(face[j][0] - 1) + 1) : 1;
            int len2 = (face[j][1] - 1) ? ((int)log10(face[j][1] - 1) + 1) : 1;
            int len3 = (face[j][2] - 1) ? ((int)log10(face[j][2] - 1) + 1) : 1;

            // Total length: sum of digits + 2 separators ('/') + null terminator
            int result_size = len1 + len2 + len3 + 2 + 1; // +2 for the '/' characters, +1 for the null terminator

            // Dynamically allocate memory based on calculated size
            char result[result_size];
            memset(result, 0, sizeof(result));

            // Using sprintf to format the string
            sprintf(result, "%d/%d/%d", face[j][0] - 1, face[j][1] - 1, face[j][2] - 1);



            // int found_idx = hashmap_search(existing_indices, result);
            int found_idx = -1;
            if (found_idx > -1) {
                out_mesh->indices[index_idx] = found_idx;
                index_idx++;
            } else {
                vec3* vertex = &vertices[face[j][0] - 1];
                out_mesh->vertex_data[vertex_idx]        = vertex->x;
                out_mesh->vertex_data[vertex_idx + 1]    = vertex->y;
                out_mesh->vertex_data[vertex_idx + 2]    = vertex->z;

                vec3* normal = &normals[face[j][2] - 1];
                out_mesh->vertex_data[vertex_idx + 3] = normal->x;
                out_mesh->vertex_data[vertex_idx + 4] = normal->y;
                out_mesh->vertex_data[vertex_idx + 5] = normal->z;

                vec2* tex_coord = &tex_coords[face[j][1] - 1];
                out_mesh->vertex_data[vertex_idx + 6] = tex_coord->x;
                out_mesh->vertex_data[vertex_idx + 7] = 1.0f - tex_coord->y;

                vertex_idx += 8;

                hashmap_insert(existing_indices, result, index_idx);

                out_mesh->indices[index_idx] = index_idx;
                index_idx++;
            }
        }
    }

    hashmap_free(existing_indices);

    // u16 i;
    // u16 gl_index;
    // for (i = 0, gl_index = 0; i < vertices_count; i++, gl_index += 8) {
    //     vec3* vertex = &vertices[i];
    //     out_mesh->vertex_data[gl_index]        = vertex->x;
    //     out_mesh->vertex_data[gl_index + 1]    = vertex->y;
    //     out_mesh->vertex_data[gl_index + 2]    = vertex->z;

    //     vec3* normal = &normals[i];
    //     out_mesh->vertex_data[gl_index + 3] = normal->x;
    //     out_mesh->vertex_data[gl_index + 4] = normal->y;
    //     out_mesh->vertex_data[gl_index + 5] = normal->z;

    //     vec2* tex_coord = &tex_coords[i];
    //     out_mesh->vertex_data[gl_index + 6] = tex_coord->x;
    //     out_mesh->vertex_data[gl_index + 7] = tex_coord->y;
    // }

    // for (i = 0, gl_index = 0; i < faces_count; i++, gl_index += 3) {
    //     u32** face = faces[i];
    //     out_mesh->indices[gl_index]     = face[0][0];
    //     out_mesh->indices[gl_index + 1] = face[1][0];
    //     out_mesh->indices[gl_index + 2] = face[2][0];
    // }






    return true;

    // out_mesh->faces_count = faces_count;
    // out_mesh->faces = (face*)malloc(sizeof(face) * faces_count);
    // for (u16 i = 0; i < faces_count; i++) {
    //     u16** face_data = faces[i];
    //     face *cur_face = &out_mesh->faces[i];
    //     vertex* vs[3] = { &cur_face->v0, &cur_face->v1, &cur_face->v2 };
    //     for (u8 j = 0; j < 3; j++) {
    //         u16* vertex_data = face_data[j];
    //         vs[j]->pos = vertices[vertex_data[0] - 1];
    //         vs[j]->tex_coord = tex_coords[vertex_data[1] - 1];
    //         vs[j]->normal = normals[vertex_data[2] - 1];
    //         vs[j]->color = 0xffff;
    //     }
    //     face_calc_tangent_and_bi_tangent(cur_face);
    // }
}