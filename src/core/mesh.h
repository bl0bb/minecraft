#ifndef MESH_H
#define MESH_H

#include "maths.h"

typedef struct {
    std::vector<f32> vertex_data;

    std::vector<u32> indices;
} mesh;

bool mesh_create(mesh* out_mesh, u16 vertices_count, Vec3<f64>* vertices, u16 tex_coords_count, Vec2<f64>* tex_coords, u16 normals_count, vec3* normals, u16 faces_count, u32*** faces);

#endif