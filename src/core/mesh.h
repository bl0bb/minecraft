#ifndef MESH_H
#define MESH_H

#include "maths.h"

typedef struct {
    u16 vertex_data_count;
    f32* vertex_data;

    u16 indices_count;
    u32* indices;
} mesh;

bool mesh_create(mesh* out_mesh, u16 vertices_count, vec3* vertices, u16 tex_coords_count, vec2* tex_coords, u16 normals_count, vec3* normals, u16 faces_count, u32*** faces);

#endif