#include <stdlib.h>
#include <string.h>

#include "../core/darray.h"
#include "../core/mesh.h"

#include "obj_parser.h"

static void objparser_fill_number_string(char* number_str, darray* line, u32 j, u16 *k) {
    memset(number_str, 0, sizeof(char) * 32);
    u8 i = 0;
    while (1) {
        char ch = *(char*)darray_get(line, j + *k);
        if (!ch || ch == ' ' || ch == '\n' || ch == '\r') {
            break;
        }
        number_str[i++] = ch;
        (*k)++;
    }
}

static f64 objparser_parse_vector_number(char* number_str, darray* line, u32 j, u16 *k) {
    objparser_fill_number_string(number_str, line, j, k);
    return atof(number_str);
}

static vec3 objparser_parse_vec3(char* number_str, darray* line, u32 j, u16 *k) {
    vec3 v;
    v.x = objparser_parse_vector_number(number_str, line, j, k);
    (*k)++;
    v.y = objparser_parse_vector_number(number_str, line, j, k);
    (*k)++;
    v.z = objparser_parse_vector_number(number_str, line, j, k);
    (*k)++;
    return v;
}

static vec2 objparser_parse_vec2(char* number_str, darray* line, u32 j, u16 *k) {
    vec2 v;
    v.x = objparser_parse_vector_number(number_str, line, j, k);
    (*k)++;
    v.y = objparser_parse_vector_number(number_str, line, j, k);
    (*k)++;
    return v;
}

mesh objparser_parse_obj(const u32 file_count, const char* file) {
    // just start with 3 vertices to create a triangle, because i would NEVER add a mesh that doesnt have a single triangle... right?
    darray vertices;
    darray_init(&vertices, sizeof(vec3), 3);
    darray tex_coords;
    darray_init(&tex_coords, sizeof(vec2), 1);
    darray normals;
    darray_init(&normals, sizeof(vec3), 1);
    darray faces;
    darray_init(&faces, sizeof(u32**), 1);

    darray line;
    darray_init(&line, sizeof(char), 1);

    char* token = malloc(sizeof(char) * (6 + 1));

    // i chose 32 because 2^5. its used to store each number string that should be parsed to a float / f64. i think 32 characters is more than enough
    char* number_str = malloc(sizeof(char) * 32);

    u32 i = 0;

    while (i < file_count) {
        char ch = file[i];
        darray_push(&line, &ch);
        if (ch == '\n') {
            memset(token, 0, sizeof(char) * (6 + 1));
            u32 j = 0;
            while (1) {
                char *ch = darray_get(&line, j);
                if (!ch || *ch == ' ' || *ch == '\n' || *ch == '\r') {
                    break;
                }
                token[j++] = *ch;
            }

            j++;
            
            if (strcmp(token, "v") == 0) {
                u16 k = 0;

                vec3 vertex = objparser_parse_vec3(number_str, &line, j, &k);
                darray_push(&vertices, &vertex);
            } else if (strcmp(token, "vt") == 0) {
                u16 k = 0;

                vec2 tex_coord = objparser_parse_vec2(number_str, &line, j, &k);
                darray_push(&tex_coords, &tex_coord);
            } else if (strcmp(token, "vn") == 0) {
                u16 k = 0;

                vec3 normal = objparser_parse_vec3(number_str, &line, j, &k);
                darray_push(&normals, &normal);
            } else if (strcmp(token, "f") == 0) {
                u32** face = malloc(sizeof(u32*) * 3);
                
                u16 k = 0;

                u16 l = 0; // amount of spaces encountered
                u16 m; // amount of slashes encountered
                while (l < 3) {
                    face[l] = malloc(sizeof(u32) * 3);
                    m = 0;
                    while (m < 3) {
                        // do not mistake this function for the objparser_fill_number_string function.... this one stops at the '/' character as well
                        memset(number_str, 0, sizeof(char) * 32);
                        u8 o = 0;
                        while (1) {
                            char *ch = darray_get(&line, j + k);
                            if (!ch || *ch == ' ' || *ch == '\n' || *ch == '\r' || *ch == '/') {
                                break;
                            }
                            number_str[o++] = *ch;
                            k++;
                        }


                        face[l][m] = atoi(number_str);
                        m++;
                        k++;
                    }
                    l++;
                }

                darray_push(&faces, &face);
            }

            // clear line
            memset(line.data, 0, line.element_size * line.size);
            line.i = 0;
        }
        i++;
    }

    mesh new_mesh;
    mesh_create(&new_mesh, vertices.i, vertices.data, tex_coords.i, tex_coords.data, normals.i, normals.data, faces.i, faces.data);

    free(token);
    free(number_str);
    
    darray_free(&vertices);
    darray_free(&tex_coords);
    darray_free(&normals);
    darray_free(&faces);

    darray_free(&line);

    return new_mesh;
}