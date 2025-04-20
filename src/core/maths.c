#include <stdbool.h>
#include <stdint.h>

#include "maths.h"




// vector 2
f64 vec2_length(const vec2* vec) {
    return sqrt(vec->x * vec->x + vec->y * vec->y);
}

vec2 vec2_normalized(const vec2* vec) {
    f64 invLength = 1.0 / vec2_length(vec);
    return (vec2) {
        .x = vec->x * invLength,
        .y = vec->y * invLength,
    };
}

void vec2_normalize(vec2* vec) {
    f64 invLength = 1.0 / vec2_length(vec);
    vec->x *= invLength;
    vec->y *= invLength;
}

f64 vec2_dot(const vec2* vec1, const vec2* vec_2) {
    return vec1->x * vec_2->x + vec1->y * vec_2->y;
}

f64 vec2_cross(const vec2* vec1, const vec2* vec_2) {
    return vec1->y * vec_2->x - vec1->x * vec_2->y;
}

void vec2_add(vec2* vec1, const vec2* vec_2) {
    vec1->x += vec_2->x;
    vec1->y += vec_2->y;
}

void vec2_sub(vec2* vec1, const vec2* vec_2) {
    vec1->x -= vec_2->x;
    vec1->y -= vec_2->y;
}

void vec2_mul(vec2* vec1, f64 n) {
    vec1->x *= n;
    vec1->y *= n;
}

void vec2_mul_vec(vec2* vec1, const vec2* vec_2) {
    vec1->x *= vec_2->x;
    vec1->y *= vec_2->y;
}

void vec2_div(vec2* vec1, f64 n) {
    vec1->x /= n;
    vec1->y /= n;
}

void vec2_div_vec(vec2* vec1, const vec2* vec_2) {
    vec1->x /= vec_2->x;
    vec1->y /= vec_2->y;
}

void vec2_lerp(vec2* vec1, const vec2* vec_2, f64 t) {
    vec1->x = lerpd(vec1->x, vec_2->x, t);
    vec1->y = lerpd(vec1->y, vec_2->y, t);
}

bool vec2_equals(const vec2* vec1, const vec2* vec_2) {
    return vec1->x == vec_2->x && vec1->y == vec_2->y;
}

bool vec2_is_zero(const vec2* vec1) {
    return vec1->x == 0 && vec1->y == 0;
}








// vector 3
f64 vec3_length(const vec3* vec) {
    return sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

vec3 vec3_normalized(const vec3* vec) {
    f64 invLength = 1.0 / vec3_length(vec);
    return (vec3) {
        .x = vec->x * invLength,
        .y = vec->y * invLength,
        .z = vec->z * invLength,
    };
}

void vec3_normalize(vec3* vec) {
    f64 invLength = 1.0 / vec3_length(vec);
    vec->x *= invLength;
    vec->y *= invLength;
    vec->z *= invLength;
}

f64 vec3_dot(const vec3* vec1, const vec3* vec_2) {
    return vec1->x * vec_2->x + vec1->y * vec_2->y + vec1->z * vec_2->z;
}

void vec3_cross(vec3* vec1, const vec3* vec_2) {
    f64 x = vec1->y * vec_2->z - vec1->z * vec_2->y;
    f64 y = vec1->z * vec_2->x - vec1->x * vec_2->z;
    f64 z = vec1->x * vec_2->y - vec1->y * vec_2->x;
    vec1->x = x;
    vec1->y = y;
    vec1->z = z;
}

void vec3_add(vec3* vec1, const vec3* vec_2) {
    vec1->x += vec_2->x;
    vec1->y += vec_2->y;
    vec1->z += vec_2->z;
}

void vec3_sub(vec3* vec1, const vec3* vec_2) {
    vec1->x -= vec_2->x;
    vec1->y -= vec_2->y;
    vec1->z -= vec_2->z;
}

void vec3_mul(vec3* vec1, f64 n) {
    vec1->x *= n;
    vec1->y *= n;
    vec1->z *= n;
}

void vec3_mul_vec(vec3* vec1, const vec3* vec_2) {
    vec1->x *= vec_2->x;
    vec1->y *= vec_2->y;
    vec1->z *= vec_2->z;
}

void vec3_div(vec3* vec1, f64 n) {
    vec1->x /= n;
    vec1->y /= n;
    vec1->z /= n;
}

void vec3_div_vec(vec3* vec1, const vec3* vec_2) {
    vec1->x /= vec_2->x;
    vec1->y /= vec_2->y;
    vec1->z /= vec_2->z;
}

void vec3_lerp(vec3* vec1, const vec3* vec_2, f64 t) {
    vec1->x = lerpd(vec1->x, vec_2->x, t);
    vec1->y = lerpd(vec1->y, vec_2->y, t);
    vec1->z = lerpd(vec1->z, vec_2->z, t);
}

bool vec3_equals(const vec3* vec1, const vec3* vec_2) {
    return vec1->x == vec_2->x && vec1->y == vec_2->y && vec1->z == vec_2->z;
}

bool vec3_is_zero(const vec3* vec1) {
    return vec1->x == 0 && vec1->y == 0 && vec1->z == 0;
}











// matrix 4
void mat4_identity(mat4* mat1) {
    *mat1 = (mat4) {
        .m00 = 1,
        .m01 = 0,
        .m02 = 0,
        .m03 = 0,
        .m10 = 0,
        .m11 = 1,
        .m12 = 0,
        .m13 = 0,
        .m20 = 0,
        .m21 = 0,
        .m22 = 1,
        .m23 = 0,
        .m30 = 0,
        .m31 = 0,
        .m32 = 0,
        .m33 = 1,
    };
}

void mat4_zero(mat4* mat) {
    *mat = (mat4) {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
}

void mat4_mul_mat4(mat4* mat1, const mat4* mat2) {
    mat4 new_mat;

    new_mat.m00 = mat1->m00 * mat2->m00 + mat1->m01 * mat2->m10 + mat1->m02 * mat2->m20 + mat1->m03 * mat2->m30;
    new_mat.m01 = mat1->m00 * mat2->m01 + mat1->m01 * mat2->m11 + mat1->m02 * mat2->m21 + mat1->m03 * mat2->m31;
    new_mat.m02 = mat1->m00 * mat2->m02 + mat1->m01 * mat2->m12 + mat1->m02 * mat2->m22 + mat1->m03 * mat2->m32;
    new_mat.m03 = mat1->m00 * mat2->m03 + mat1->m01 * mat2->m13 + mat1->m02 * mat2->m23 + mat1->m03 * mat2->m33;

    new_mat.m10 = mat1->m10 * mat2->m00 + mat1->m11 * mat2->m10 + mat1->m12 * mat2->m20 + mat1->m13 * mat2->m30;
    new_mat.m11 = mat1->m10 * mat2->m01 + mat1->m11 * mat2->m11 + mat1->m12 * mat2->m21 + mat1->m13 * mat2->m31;
    new_mat.m12 = mat1->m10 * mat2->m02 + mat1->m11 * mat2->m12 + mat1->m12 * mat2->m22 + mat1->m13 * mat2->m32;
    new_mat.m13 = mat1->m10 * mat2->m03 + mat1->m11 * mat2->m13 + mat1->m12 * mat2->m23 + mat1->m13 * mat2->m33;

    new_mat.m20 = mat1->m20 * mat2->m00 + mat1->m21 * mat2->m10 + mat1->m22 * mat2->m20 + mat1->m23 * mat2->m30;
    new_mat.m21 = mat1->m20 * mat2->m01 + mat1->m21 * mat2->m11 + mat1->m22 * mat2->m21 + mat1->m23 * mat2->m31;
    new_mat.m22 = mat1->m20 * mat2->m02 + mat1->m21 * mat2->m12 + mat1->m22 * mat2->m22 + mat1->m23 * mat2->m32;
    new_mat.m23 = mat1->m20 * mat2->m03 + mat1->m21 * mat2->m13 + mat1->m22 * mat2->m23 + mat1->m23 * mat2->m33;

    new_mat.m30 = mat1->m30 * mat2->m00 + mat1->m31 * mat2->m10 + mat1->m32 * mat2->m20 + mat1->m33 * mat2->m30;
    new_mat.m31 = mat1->m30 * mat2->m01 + mat1->m31 * mat2->m11 + mat1->m32 * mat2->m21 + mat1->m33 * mat2->m31;
    new_mat.m32 = mat1->m30 * mat2->m02 + mat1->m31 * mat2->m12 + mat1->m32 * mat2->m22 + mat1->m33 * mat2->m32;
    new_mat.m33 = mat1->m30 * mat2->m03 + mat1->m31 * mat2->m13 + mat1->m32 * mat2->m23 + mat1->m33 * mat2->m33;

    *mat1 = new_mat;
}

void mat4_inverse(mat4* mat) {
    f64 det = 
        mat->m00*mat->m11*mat->m22*mat->m33 +
        mat->m00*mat->m12*mat->m23*mat->m31 +
        mat->m00*mat->m13*mat->m21*mat->m32 +

        mat->m01*mat->m10*mat->m23*mat->m32 +
        mat->m01*mat->m12*mat->m20*mat->m33 +
        mat->m01*mat->m13*mat->m22*mat->m30 +

        mat->m02*mat->m10*mat->m21*mat->m33 +
        mat->m02*mat->m11*mat->m23*mat->m30 +
        mat->m02*mat->m13*mat->m20*mat->m31 +

        mat->m03*mat->m10*mat->m22*mat->m31 +
        mat->m03*mat->m11*mat->m20*mat->m32 +
        mat->m03*mat->m12*mat->m21*mat->m30 -

        mat->m00*mat->m11*mat->m23*mat->m32 -
        mat->m00*mat->m12*mat->m21*mat->m33 -
        mat->m00*mat->m13*mat->m22*mat->m31 -

        mat->m01*mat->m10*mat->m22*mat->m33 -
        mat->m01*mat->m12*mat->m23*mat->m30 -
        mat->m01*mat->m13*mat->m20*mat->m32 -

        mat->m02*mat->m10*mat->m23*mat->m31 -
        mat->m02*mat->m11*mat->m20*mat->m33 -
        mat->m02*mat->m13*mat->m21*mat->m30 -

        mat->m03*mat->m10*mat->m21*mat->m32 -
        mat->m03*mat->m11*mat->m22*mat->m30 -
        mat->m03*mat->m12*mat->m20*mat->m31; 

    f64 a0 = mat->m11*mat->m22*mat->m33 + mat->m12*mat->m23*mat->m31 + mat->m13*mat->m21*mat->m32
        - mat->m11*mat->m23*mat->m32 - mat->m12*mat->m21*mat->m33 - mat->m13*mat->m22*mat->m31;

    f64 a1 = mat->m01*mat->m23*mat->m32 + mat->m02*mat->m21*mat->m33 + mat->m03*mat->m22*mat->m31
        - mat->m01*mat->m22*mat->m33 - mat->m02*mat->m23*mat->m31 - mat->m03*mat->m21*mat->m32;

    f64 a2 = mat->m01*mat->m12*mat->m33 + mat->m02*mat->m13*mat->m31 + mat->m03*mat->m11*mat->m32
        - mat->m01*mat->m13*mat->m32 - mat->m02*mat->m11*mat->m33 - mat->m03*mat->m12*mat->m31;

    f64 a3 = mat->m01*mat->m13*mat->m22 + mat->m02*mat->m11*mat->m23 + mat->m03*mat->m12*mat->m21
        - mat->m01*mat->m12*mat->m23 - mat->m02*mat->m13*mat->m21 - mat->m03*mat->m11*mat->m22;

    f64 a4 = mat->m10*mat->m23*mat->m32 + mat->m12*mat->m20*mat->m33 + mat->m13*mat->m22*mat->m30
        - mat->m10*mat->m22*mat->m33 - mat->m12*mat->m23*mat->m30 - mat->m13*mat->m20*mat->m32;

    f64 a5 = mat->m00*mat->m22*mat->m33 + mat->m02*mat->m23*mat->m30 + mat->m03*mat->m20*mat->m32
        - mat->m00*mat->m23*mat->m32 - mat->m02*mat->m20*mat->m33 - mat->m03*mat->m22*mat->m30;

    f64 a6 = mat->m00*mat->m13*mat->m32 + mat->m02*mat->m10*mat->m33 + mat->m03*mat->m12*mat->m30
        - mat->m00*mat->m12*mat->m33 - mat->m02*mat->m13*mat->m30 - mat->m03*mat->m10*mat->m32;

    f64 a7 = mat->m00*mat->m12*mat->m23 + mat->m02*mat->m13*mat->m20 + mat->m03*mat->m10*mat->m22
        - mat->m00*mat->m13*mat->m22 - mat->m02*mat->m10*mat->m23 - mat->m03*mat->m12*mat->m20;

    f64 a8 = mat->m10*mat->m21*mat->m33 + mat->m11*mat->m23*mat->m30 + mat->m13*mat->m20*mat->m31
        - mat->m10*mat->m23*mat->m31 - mat->m11*mat->m20*mat->m33 - mat->m13*mat->m21*mat->m30;

    f64 a9 = mat->m00*mat->m23*mat->m31 + mat->m01*mat->m20*mat->m33 + mat->m03*mat->m21*mat->m30
        - mat->m00*mat->m21*mat->m33 - mat->m01*mat->m23*mat->m30 - mat->m03*mat->m20*mat->m31;

    f64 a10 = mat->m00*mat->m11*mat->m33 + mat->m01*mat->m13*mat->m30 + mat->m03*mat->m10*mat->m31
        - mat->m00*mat->m13*mat->m31 - mat->m01*mat->m10*mat->m33 - mat->m03*mat->m11*mat->m30;

    f64 a11 = mat->m00*mat->m13*mat->m21 + mat->m01*mat->m10*mat->m23 + mat->m03*mat->m11*mat->m20
        - mat->m00*mat->m11*mat->m23 - mat->m01*mat->m13*mat->m20 - mat->m03*mat->m10*mat->m21;

    f64 a12 = mat->m10*mat->m22*mat->m31 + mat->m11*mat->m20*mat->m32 + mat->m12*mat->m21*mat->m30
        - mat->m10*mat->m21*mat->m32 - mat->m11*mat->m22*mat->m30 - mat->m12*mat->m20*mat->m31;

    f64 a13 = mat->m00*mat->m21*mat->m32 + mat->m01*mat->m22*mat->m30 + mat->m02*mat->m20*mat->m31
        - mat->m00*mat->m22*mat->m31 - mat->m01*mat->m20*mat->m32 - mat->m02*mat->m21*mat->m30;

    f64 a14 = mat->m00*mat->m12*mat->m31 + mat->m01*mat->m10*mat->m32 + mat->m02*mat->m11*mat->m30
        - mat->m00*mat->m11*mat->m32 - mat->m01*mat->m12*mat->m30 - mat->m02*mat->m10*mat->m31;

    f64 a15 = mat->m00*mat->m11*mat->m22 + mat->m01*mat->m12*mat->m20 + mat->m02*mat->m10*mat->m21
        - mat->m00*mat->m12*mat->m21 - mat->m01*mat->m10*mat->m22 - mat->m02*mat->m11*mat->m20;

    *mat = (mat4) { 
            a0 / det, a1 / det, a2 / det, a3 / det,
            a4 / det, a5 / det, a6 / det, a7 / det,
            a8 / det, a9 / det, a10 / det, a11 / det, 
            a12 / det, a13 / det, a14 / det, a15 / det
    };
}

void mat4_mul_vec3(const mat4* mat, vec3* vec, f64 w) {
    vec3 new_vec;

    new_vec.x = mat->m00 * vec->x + mat->m01 * vec->y + mat->m02 * vec->z + mat->m03 * w;
    new_vec.y = mat->m10 * vec->x + mat->m11 * vec->y + mat->m12 * vec->z + mat->m13 * w;
    new_vec.z = mat->m20 * vec->x + mat->m21 * vec->y + mat->m22 * vec->z + mat->m23 * w;

    *vec = new_vec;
}

void mat4_scale(mat4* mat, f64 x, f64 y, f64 z) {
    mat4 scale;
    mat4_identity(&scale);
    scale.m00 = x;
    scale.m11 = y;
    scale.m22 = z;

    mat4_mul_mat4(mat, &scale);
}

void mat4_translate(mat4* mat, f64 x, f64 y, f64 z) {
    mat4 translate;
    mat4_identity(&translate);
    translate.m03 = x;
    translate.m13 = y;
    translate.m23 = z;

    mat4_mul_mat4(mat, &translate);
}

void mat4_translate_vec3(mat4* mat, const vec3* vec) {
    mat4 translate;
    mat4_identity(&translate);
    translate.m03 = vec->x;
    translate.m13 = vec->y;
    translate.m23 = vec->z;

    mat4_mul_mat4(mat, &translate);
}

void mat4_translate_world(mat4* mat, f64 x, f64 y, f64 z) {
    mat->m03 += x;
    mat->m13 += y;
    mat->m23 += z;
}

void mat4_translate_world_vec3(mat4* mat, const vec3* vec) {
    mat->m03 += vec->x;
    mat->m13 += vec->y;
    mat->m23 += vec->z;
}

void mat4_rotate(mat4* mat, f64 x, f64 y, f64 z) {
    f64 sin_x = sin(x);
    f64 cos_x = cos(x);
    f64 sin_y = sin(y);
    f64 cos_y = cos(y);
    f64 sin_z = sin(z);
    f64 cos_z = cos(z);

    mat4 rotate = {
        .m00 = cos_y * cos_z,
        .m01 = -cos_y * sin_z,
        .m02 = sin_y,
        .m03 = 0,
        .m10 = sin_x * sin_y * cos_z + cos_x * sin_z,
        .m11 = -sin_x * sin_y * sin_z + cos_x * cos_z,
        .m12 = -sin_x * cos_y,
        .m13 = 0,
        .m20 = -cos_x * sin_y * cos_z + sin_x * sin_z,
        .m21 = cos_x * sin_y * sin_z + sin_x * cos_z,
        .m22 = cos_x * cos_y,
        .m23 = 0,
        .m30 = 0,
        .m31 = 0,
        .m32 = 0,
        .m33 = 1,
    };

    mat4_mul_mat4(mat, &rotate);
}

void mat4_pos_to_vec3(const mat4* mat, vec3* vec) {
    vec->x = mat->m03;
    vec->y = mat->m13;
    vec->z = mat->m23;
}

void mat4_perspective(f64 fov_y, f64 aspect, f64 z_near, f64 z_far, mat4* out) {
    f64 tan_half_fov_y = tan(fov_y / (f64)2);

    // mat4_identity(out);
    mat4_zero(out);
    out->m00 = (f64)1 / (aspect * tan_half_fov_y);
    out->m11 = (f64)1 / (tan_half_fov_y);
    out->m22 = -(z_far + z_near) / (z_far - z_near);
    out->m32 = -(f64)1;
    out->m23 = -((f64)2 * z_far * z_near) / (z_far - z_near);
}

void mat4_to_gl_mat4(const mat4* mat, f32* out) {
    out[0]  = mat->m00;
    out[1]  = mat->m10;
    out[2]  = mat->m20;
    out[3]  = mat->m30;
    out[4]  = mat->m01;
    out[5]  = mat->m11;
    out[6]  = mat->m21;
    out[7]  = mat->m31;
    out[8]  = mat->m02;
    out[9]  = mat->m12;
    out[10] = mat->m22;
    out[11] = mat->m32;
    out[12] = mat->m03;
    out[13] = mat->m13;
    out[14] = mat->m23;
    out[15] = mat->m33;

    // out[0]  = mat->m00;
    // out[1]  = mat->m01;
    // out[2]  = mat->m02;
    // out[3]  = mat->m03;
    // out[4]  = mat->m10;
    // out[5]  = mat->m11;
    // out[6]  = mat->m12;
    // out[7]  = mat->m13;
    // out[8]  = mat->m20;
    // out[9]  = mat->m21;
    // out[10] = mat->m22;
    // out[11] = mat->m23;
    // out[12] = mat->m30;
    // out[13] = mat->m31;
    // out[14] = mat->m32;
    // out[15] = mat->m33;
}











// quat
void quat_identity(quat* self) {
    *self = (quat) { 0, 0, 0, 1 };
}

void quat_from_vec3(quat* self, const vec3* v, f32 theta) {
    f32 temp = sin(theta / 2.0);
    self->x = temp * v->x;
    self->y = temp * v->y;
    self->z = temp * v->z;
    self->w = cos(theta / 2.0);
    quat_normalize(self);
}

void quat_normalize(quat* self) {
    f32 mag = sqrt(self->w*self->w + self->x*self->x + self->y*self->y + self->z*self->z);

    if (mag == 0) {
        return;
    }

    f64 inv = 1 / mag;
    self->x *= inv;
    self->y *= inv;
    self->z *= inv;
    self->w *= inv;
}

mat4 quat_get_matrix(const quat *self) {
    f32 x = self->x;
    f32 y = self->y;
    f32 z = self->z;
    f32 w = self->w;

    return (mat4) {   1.0 - 2.0 * y * y - 2.0 * z * z, 2.0 * x * y - 2.0 * w * z      , 2.0 * x * z + 2.0 * w * y      , 0.0,
                        2.0 * x * y + 2.0 * w * z      , 1.0 - 2.0 * x * x - 2.0 * z * z, 2.0 * y * z - 2.0 * w * x      , 0.0, 
                        2.0 * x * z - 2.0 * w * y      , 2.0 * y * z + 2.0 * w * x      , 1.0 - 2.0 * x * x - 2.0 * y * y, 0.0, 
                        0.0                            , 0.0                            , 0.0                            , 1.0};
}

quat quat_mul(const quat* u, const quat* v) {
    f32 x = v->w * u->x + v->x * u->w - v->y * u->z + v->z * u->y;
    f32 y = v->w * u->y + v->x * u->z + v->y * u->w - v->z * u->x;
    f32 z = v->w * u->z - v->x * u->y + v->y * u->x + v->z * u->w;
    f32 w = v->w * u->w - v->x * u->x - v->y * u->y - v->z * u->z;

    quat new_quat = { x, y, z, w };
    quat_normalize(&new_quat);
    return new_quat;
}




















// rgba







// utils
void lerp_3_attribute_vec2(
    vec2* a, vec2* b, vec2* c,
    f64 w0, f64 w1, f64 w2,
    f64 z0, f64 z1, f64 z2,
    f64 z,
    vec2* wa, vec2* wb, vec2* wc,
    vec2* out
) {
    *wa = *a;
    vec2_mul(wa, w0 / z0 * z);
    *wb = *b;
    vec2_mul(wb, w1 / z1 * z);
    *wc = *c;
    vec2_mul(wc, w2 / z2 * z);
    *out = (vec2) {
        .x = wa->x + wb->x + wc->x,
        .y = wa->y + wb->y + wc->y,
    };
}

void lerp_2_attribute_vec3(
    vec3* a, vec3* b,
    f64 w0, f64 w1,
    f64 z0, f64 z1,
    f64 z,
    vec3* wa, vec3* wb,
    vec3* out
) {
    *wa = *a;
    vec3_mul(wa, w0 / z0 * z);
    *wb = *b;
    vec3_mul(wb, w1 / z1 * z);
    *out = (vec3) {
        .x = wa->x + wb->x,
        .y = wa->y + wb->y,
        .z = wa->z + wb->z,
    };
}

void lerp_3_attribute_vec3(
    vec3* a, vec3* b, vec3* c,
    f64 w0, f64 w1, f64 w2,
    f64 z0, f64 z1, f64 z2,
    f64 z,
    vec3* wa, vec3* wb, vec3* wc,
    vec3* out
) {
    *wa = *a;
    vec3_mul(wa, w0 / z0 * z);
    *wb = *b;
    vec3_mul(wb, w1 / z1 * z);
    *wc = *c;
    vec3_mul(wc, w2 / z2 * z);
    *out = (vec3) {
        .x = wa->x + wb->x + wc->x,
        .y = wa->y + wb->y + wc->y,
        .z = wa->z + wb->z + wc->z,
    };
}

rgba lerp_2_attribute_rgba(
    rgba a, rgba b,
    f64 w0, f64 w1,
    f64 z0, f64 z1,
    f64 z
) {
    rgba wa = rgba_mul(a, w0 / z0 * z);
    rgba wb = rgba_mul(b, w1 / z1 * z);
    return rgba_create(rgba_r(wa) + rgba_r(wb), rgba_g(wa) + rgba_g(wb), rgba_b(wa) + rgba_b(wb), rgba_a(wa) + rgba_a(wb));
}

rgba lerp_3_attribute_rgba(
    rgba a, rgba b, rgba c,
    f64 w0, f64 w1, f64 w2,
    f64 z0, f64 z1, f64 z2,
    f64 z
) {
    rgba wa = rgba_mul(a, w0 / z0 * z);
    rgba wb = rgba_mul(b, w1 / z1 * z);
    rgba wc = rgba_mul(c, w2 / z2 * z);
    return rgba_create(rgba_r(wa) + rgba_r(wb) + rgba_r(wc), rgba_g(wa) + rgba_g(wb) + rgba_g(wc), rgba_b(wa) + rgba_b(wb) + rgba_b(wc), rgba_a(wa) + rgba_a(wb) + rgba_a(wc));
}