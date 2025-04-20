#ifndef MATHS_H
#define MATHS_H

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "types.h"

// math.h stuff C++ in MS doesnt work properly or something so define pi manually
#define M_PI 3.14159265358979323846


// util


// deg rad
static inline f64 deg_to_rad(f64 deg) {
    return deg * (M_PI / 180.0);
}

static inline f64 rad_to_deg(f64 rad) {
    return rad * (180.0 / M_PI);
}


// lerp
static inline int lerp(int a, int b, int t) {
    return a + (b - a) * t;
}

static inline f64 lerpd(f64 a, f64 b, f64 t) {
    return a + (b - a) * t;
}




// vector 2
typedef struct {
    f64 x;
    f64 y;
} vec2;

f64 vec2_length(const vec2* vec);

vec2 vec2_normalized(const vec2* vec);

void vec2_normalize(vec2* vec);

f64 vec2_dot(const vec2* vec1, const vec2* vec_2);

f64 vec2_cross(const vec2* vec1, const vec2* vec_2);

void vec2_add(vec2* vec1, const vec2* vec_2);

void vec2_sub(vec2* vec1, const vec2* vec_2);

void vec2_mul(vec2* vec1, f64 n);

void vec2_mul_vec(vec2* vec1, const vec2* vec_2);

void vec2_div(vec2* vec1, f64 n);

void vec2_div_vec(vec2* vec1, const vec2* vec_2);

void vec2_lerp(vec2* vec1, const vec2* vec_2, f64 t);

bool vec2_equals(const vec2* vec1, const vec2* vec_2);

bool vec2_is_zero(const vec2* vec1);






// vector 3
typedef struct {
    f64 x;
    f64 y;
    f64 z;
} vec3;

f64 vec3_length(const vec3* vec);

vec3 vec3_normalized(const vec3* vec);

void vec3_normalize(vec3* vec);

f64 vec3_dot(const vec3* vec1, const vec3* vec_2);

void vec3_cross(vec3* vec1, const vec3* vec_2);

void vec3_add(vec3* vec1, const vec3* vec_2);

void vec3_sub(vec3* vec1, const vec3* vec_2);

void vec3_mul(vec3* vec1, f64 n);

void vec3_mul_vec(vec3* vec1, const vec3* vec_2);

void vec3_div(vec3* vec1, f64 n);

void vec3_div_vec(vec3* vec1, const vec3* vec_2);

void vec3_lerp(vec3* vec1, const vec3* vec_2, f64 t);

bool vec3_equals(const vec3* vec1, const vec3* vec_2);

bool vec3_is_zero(const vec3* vec1);





// matrix 4
typedef struct {
    f64 m00;
    f64 m01;
    f64 m02;
    f64 m03;
    f64 m10;
    f64 m11;
    f64 m12;
    f64 m13;
    f64 m20;
    f64 m21;
    f64 m22;
    f64 m23;
    f64 m30;
    f64 m31;
    f64 m32;
    f64 m33;
} mat4;

void mat4_zero(mat4* mat);

void mat4_identity(mat4* mat1);

void mat4_mul_mat4(mat4* mat1, const mat4* mat2);

void mat4_inverse(mat4* mat);

void mat4_mul_vec3(const mat4* mat, vec3* vec, f64 w);

void mat4_scale(mat4* mat, f64 x, f64 y, f64 z);

void mat4_translate(mat4* mat, f64 x, f64 y, f64 z);

void mat4_translate_vec3(mat4* mat, const vec3* vec);

void mat4_translate_world(mat4* mat, f64 x, f64 y, f64 z);

void mat4_translate_world_vec3(mat4* mat, const vec3* vec);

void mat4_rotate(mat4* mat, f64 x, f64 y, f64 z);

void mat4_pos_to_vec3(const mat4* mat, vec3* vec);

void mat4_perspective(f64 fov_y, f64 aspect, f64 z_near, f64 z_far, mat4* out);

void mat4_to_gl_mat4(const mat4* mat, f32* out);











// quat
typedef struct {
    f64 x;
    f64 y;
    f64 z;
    f64 w;
} quat;

void quat_identity(quat* self);

void quat_from_vec3(quat* self, const vec3* v, f32 theta);

void quat_normalize(quat* self);

mat4 quat_get_matrix(const quat *self);

quat quat_mul(const quat* u, const quat* v);













// rgba
#define RGBA_R_MASK 0xff000000
#define RGBA_G_MASK 0x00ff0000
#define RGBA_B_MASK 0x0000ff00
#define RGBA_A_MASK 0x000000ff

#define RGBA_R_SHIFT 24
#define RGBA_G_SHIFT 16
#define RGBA_B_SHIFT 8
#define RGBA_A_SHIFT 0

typedef u32 rgba;

static inline rgba rgba_create(const u8 r, const u8 g, const u8 b, const u8 a) {
    return (r << RGBA_R_SHIFT) |
            (g << RGBA_G_SHIFT) |
            (b << RGBA_B_SHIFT) |
            (a << RGBA_A_SHIFT);
}

static inline rgba rgba_r(const rgba rgba) {
    return rgba & RGBA_R_MASK >> RGBA_R_SHIFT;
}

static inline rgba rgba_g(const rgba rgba) {
    return rgba & RGBA_G_MASK >> RGBA_G_SHIFT;
}

static inline rgba rgba_b(const rgba rgba) {
    return rgba & RGBA_B_MASK >> RGBA_B_SHIFT;
}

static inline rgba rgba_a(const rgba rgba) {
    return rgba & RGBA_A_MASK >> RGBA_A_SHIFT;
}

static inline rgba rgba_mul(const rgba rgba1, const f64 v) {
    return ((rgba)(rgba_r(rgba1) * v) << RGBA_R_SHIFT) |
            ((rgba)(rgba_g(rgba1) * v) << RGBA_G_SHIFT) |
            ((rgba)(rgba_b(rgba1) * v) << RGBA_B_SHIFT) |
            ((rgba)(rgba_a(rgba1) * v) << RGBA_A_SHIFT);
}

static inline rgba rgba_lerp(const rgba rgba1, const rgba rgba2, f64 t) {
    return ((rgba)lerp(rgba_r(rgba1), rgba_r(rgba2), t) << RGBA_R_SHIFT) |
            ((rgba)lerp(rgba_g(rgba1), rgba_g(rgba2), t) << RGBA_G_SHIFT) |
            ((rgba)lerp(rgba_b(rgba1), rgba_b(rgba2), t) << RGBA_B_SHIFT) |
            ((rgba)lerp(rgba_a(rgba1), rgba_a(rgba2), t) << RGBA_A_SHIFT);
}

static inline rgba rgba_lerp_color(const rgba rgba1, const rgba rgba2, f64 t) {
    return ((rgba)lerp(rgba_r(rgba1), rgba_r(rgba2), t) << RGBA_R_SHIFT) |
            ((rgba)lerp(rgba_g(rgba1), rgba_g(rgba2), t) << RGBA_G_SHIFT) |
            ((rgba)lerp(rgba_b(rgba1), rgba_b(rgba2), t) << RGBA_B_SHIFT) |
            (rgba1 & RGBA_A_MASK);
}






// utils
void lerp_3_attribute_vec2(
    vec2* a, vec2* b, vec2* c,
    f64 w0, f64 w1, f64 w2,
    f64 z0, f64 z1, f64 z2,
    f64 z,
    vec2* wa, vec2* wb, vec2* wc,
    vec2* out
);

void lerp_2_attribute_vec3(
    vec3* a, vec3* b,
    f64 w0, f64 w1,
    f64 z0, f64 z1,
    f64 z,
    vec3* wa, vec3* wb,
    vec3* out
);

void lerp_3_attribute_vec3(
    vec3* a, vec3* b, vec3* c,
    f64 w0, f64 w1, f64 w2,
    f64 z0, f64 z1, f64 z2,
    f64 z,
    vec3* wa, vec3* wb, vec3* wc,
    vec3* out
);

rgba lerp_2_attribute_rgba(
    rgba a, rgba b,
    f64 w0, f64 w1,
    f64 z0, f64 z1,
    f64 z
);

rgba lerp_3_attribute_rgba(
    rgba a, rgba b, rgba c,
    f64 w0, f64 w1, f64 w2,
    f64 z0, f64 z1, f64 z2,
    f64 z
);








#endif