#ifndef MATHS_H
#define MATHS_H

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <array>
#include <stdexcept>
#include <algorithm>

#include "types.h"

// math.h stuff C++ in MS doesnt work properly or something so define pi manually
#define M_PI 3.14159265358979323846264338327950288

namespace Math {

// util
template <typename T>
static constexpr inline i8 sign(T n) {
    return n < 0 ? -1 : n > 0 ? 1 : 0;
}

// deg rad
template<typename T>
static constexpr inline T deg_to_rad(T deg) {
    return deg * (M_PI / 180.0);
}

template<typename T>
static constexpr inline T rad_to_deg(T rad) {
    return rad * (180.0 / M_PI);
}


// lerp
template<typename T>
static constexpr inline T lerp(T a, T b, T t) {
    return a + (b - a) * t;
}

}




// vec2
template<typename T>
struct Vec2 {
    T x, y;

    // Constructors
    Vec2() : x(T(0)), y(T(0)) {}
    Vec2(T x_, T y_) : x(x_), y(y_) {}

    // Type-casting constructor
    template<typename U>
    Vec2(const Vec2<U>& other)
        : x(static_cast<T>(other.x)),
          y(static_cast<T>(other.y)) {}

    // Addition
    Vec2<T> operator+(const Vec2<T>& other) const {
        return Vec2<T>(x + other.x, y + other.y);
    }

    // Subtraction
    Vec2<T> operator-(const Vec2<T>& other) const {
        return Vec2<T>(x - other.x, y - other.y);
    }

    // Scalar multiplication
    template<typename T1>
    Vec2<T> operator*(T1 scalar) const {
        return Vec2<T>(x * scalar, y * scalar);
    }

    // Scalar division
    template<typename T1>
    Vec2<T> operator/(T1 scalar) const {
        return Vec2<T>(x / scalar, y / scalar);
    }

    // Dot product
    T dot(const Vec2<T>& other) const {
        return x * other.x + y * other.y;
    }

    // Cross product
    T cross(const Vec2<T>& other) const {
        return x * other.y - y * other.x;
    }

    // Magnitude (length)
    f64 magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    // Normalize
    Vec2<T> normalized() const {
        T mag = magnitude();
        return mag == T(0) ? *this : *this / mag;
    }

    // Floor
    Vec2<T> floor() const {
        return Vec2<T>(
            floor(x),
            floor(y)
        );
    }

    // Sum
    T sum() const {
        return x + y;
    }

    // Volume
    T volume() const {
        return x * y;
    }

    Vec2<T> abs() const {
        return Vec2<T>(std::abs(x), std::abs(y));
    }

    T max() const {
        return std::max(x, y);
    }

    T min() const {
        return std::min(x, y);
    }

    // Index
    const T& operator[](size_t index) const {
        switch(index) {
            case 0: return x;
            case 1: return y;
            default: throw std::out_of_range("Index out of range for Vector2");
        }
    }

    // Equal
    bool operator==(const Vec2<T>& other) const {
        return x == other.x && y == other.y;
    }

    // Not equal
    bool operator!=(const Vec2<T>& other) const {
        return x != other.x || y != other.y;
    }
};


// vec3
template<typename T>
struct Vec3 {
    T x, y, z;

    static Vec3<T> min(const Vec3<T>& a, const Vec3<T>& b) {
        return Vec3<T>(
            std::min(a.x, b.x),
            std::min(a.y, b.y),
            std::min(a.z, b.z)
        );
    }

    static Vec3<T> max(const Vec3<T>& a, const Vec3<T>& b) {
        return Vec3<T>(
            std::max(a.x, b.x),
            std::max(a.y, b.y),
            std::max(a.z, b.z)
        );
    }

    // Constructors
    Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

    // Type-casting constructor
    template<typename U>
    Vec3(const Vec3<U>& other)
        : x(static_cast<T>(other.x)),
          y(static_cast<T>(other.y)),
          z(static_cast<T>(other.z)) {}

    // Addition
    Vec3<T> operator+(const Vec3<T>& other) const {
        return Vec3<T>(x + other.x, y + other.y, z + other.z);
    }

    // Subtraction
    Vec3<T> operator-(const Vec3<T>& other) const {
        return Vec3<T>(x - other.x, y - other.y, z - other.z);
    }

    // Scalar multiplication
    template<typename T1>
    Vec3<T> operator*(T1 scalar) const {
        return Vec3<T>(x * scalar, y * scalar, z * scalar);
    }

    // Scalar division
    template<typename T1>
    Vec3<T> operator/(T1 scalar) const {
        return Vec3<T>(x / scalar, y / scalar, z / scalar);
    }

    // Dot product
    T dot(const Vec3<T>& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Cross product
    Vec3<T> cross(const Vec3<T>& other) const {
        return Vec3<T>(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Magnitude (length)
    f64 magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Normalize
    Vec3<T> normalized() const {
        T mag = magnitude();
        return mag == T(0) ? *this : *this / mag;
    }

    // Floor
    Vec3<T> floor() const {
        return Vec3<T>(
            std::floor(x),
            std::floor(y),
            std::floor(z)
        );
    }

    // Sign
    Vec3<T> sign() const {
        return Vec3<T>(
            Math::sign(x),
            Math::sign(y),
            Math::sign(z)
        );
    }

    // Sum
    T sum() const {
        return x + y + z;
    }

    // Volume
    T volume() const {
        return x * y * z;
    }

    Vec3<T> abs() const {
        return Vec3<T>(std::abs(x), std::abs(y), std::abs(z));
    }

    T max() const {
        return std::max(std::max(x, y), z);
    }

    T min() const {
        return std::min(std::min(x, y), z);
    }

    // Has smaller dim
    bool hasSmallerDim(const Vec3<T>& other) const {
        T sortedA[3] = {x, y, z};
        T sortedB[3] = {other.x, other.y, other.z};

        std::sort(sortedA, sortedA + 3);
        std::sort(sortedB, sortedB + 3);

        for (u8 i = 0; i < 3; i++) {
            // we are smaller
            if (sortedA[i] < sortedB[i]) return true;

            // they are smaller
            if (sortedA[i] > sortedB[i]) return false;

            // we are equal, check next
        }

        // equally close
        return false;
    }

    // Index
    const T& operator[](size_t index) const {
        switch(index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Index out of range for Vector3");
        }
    }

    // Equal
    bool operator==(const Vec3<T>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // Not equal
    bool operator!=(const Vec3<T>& other) const {
        return x != other.x || y != other.y || z != other.z;
    }
};








// matrix 4
template <typename T>
struct Mat4 {
    std::array<std::array<T, 4>, 4> m{};

    static Mat4<T> identity() {
        Mat4<T> mat{};
        for (int i = 0; i < 4; ++i)
            mat.m[i][i] = static_cast<T>(1);
        return mat;
    }

    Mat4<T> operator*(const Mat4<T>& rhs) const {
        Mat4<T> result{};
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                for (int k = 0; k < 4; ++k)
                    result.m[i][j] += m[i][k] * rhs.m[k][j];
        return result;
    }

    Vec3<T> multiplyVec3(const Vec3<T>& vec, T w) const {
        Vec3<T> result{};
        result.x = vec.x * m[0][0] + vec.y * m[0][1] + vec.z * m[0][2] + w * m[0][3];
        result.y = vec.x * m[1][0] + vec.y * m[1][1] + vec.z * m[1][2] + w * m[1][3];
        result.z = vec.x * m[2][0] + vec.y * m[2][1] + vec.z * m[2][2] + w * m[2][3];
        return result;
    }

    void scale(T x, T y, T z) {
        *this = (*this) * scalingMatrix(x, y, z);
    }

    void translate(T x, T y, T z) {
        *this = (*this) * translationMatrix(x, y, z);
    }

    void translate(const Vec3<T>& vec) {
        translate(vec.x, vec.y, vec.z);
    }

    void translateWorld(T x, T y, T z) {
        *this = translationMatrix(x, y, z) * (*this);
    }

    void translateWorld(const Vec3<T>& vec) {
        translateWorld(vec.x, vec.y, vec.z);
    }

    void rotate(T x, T y, T z) {
        *this = *this * rotationMatrix(x, y, z);
    }

    Vec3<T> extractPosition() const {
        return { m[0][3], m[1][3], m[2][3] };
    }

    static Mat4<T> perspective(T fov_y, T aspect, T z_near, T z_far) {
        // Mat4<T> result{};
        // T f = static_cast<T>(1) / std::tan(fov_y / static_cast<T>(2));
        // result.m[0][0] = f / aspect;
        // result.m[1][1] = f;
        // result.m[2][2] = (z_far + z_near) / (z_near - z_far);
        // result.m[3][2] = -1;
        // result.m[2][3] = (static_cast<T>(2) * z_far * z_near) / (z_near - z_far);
        // return result;



        f64 tan_half_fov_y = std::tan(fov_y / static_cast<T>(2));

        Mat4<T> result = identity();
        result.m[0][0] = static_cast<T>(1) / (aspect * tan_half_fov_y);
        result.m[1][1] = static_cast<T>(1) / (tan_half_fov_y);
        result.m[2][2] = -(z_far + z_near) / (z_far - z_near);
        result.m[3][2] = static_cast<T>(-1);
        result.m[2][3] = -(static_cast<T>(2) * z_far * z_near) / (z_far - z_near);
        return result;
    }

    static Mat4<T> ortho(T left, T right, T bottom, T top, T z_near = static_cast<T>(-1), T z_far = static_cast<T>(1)) {
        Mat4<T> result = identity();

        result.m[0][0] = static_cast<T>(2) / (right - left);
        result.m[1][1] = static_cast<T>(2) / (top - bottom);
        result.m[2][2] = static_cast<T>(-2) / (z_far - z_near);

        result.m[0][3] = -(right + left) / (right - left);
        result.m[1][3] = -(top + bottom) / (top - bottom);
        result.m[2][3] = -(z_far + z_near) / (z_far - z_near);
        
        return result;
    }

    static Mat4<T> lookAt(Vec3<T> const& eye, Vec3<T> const& center, Vec3<T> const& up) {
        const Vec3<T> f((center - eye).normalized());
		const Vec3<T> s(f.cross(up).normalized());
		const Vec3<T> u(s.cross(f));

		Mat4<T> Result = identity();
		Result[0][0] =  s.x;
		Result[0][1] =  s.y;
		Result[0][2] =  s.z;
		Result[1][0] =  u.x;
		Result[1][1] =  u.y;
		Result[1][2] =  u.z;
		Result[2][0] = -f.x;
		Result[2][1] = -f.y;
		Result[2][2] = -f.z;
		Result[0][3] = -s.dot(eye);
		Result[1][3] = -u.dot(eye);
		Result[2][3] =  f.dot(eye);
		return Result;
    }

    void toGLMatrix(float* out) const {
        out[0]  = m[0][0];
        out[1]  = m[1][0];
        out[2]  = m[2][0];
        out[3]  = m[3][0];
        out[4]  = m[0][1];
        out[5]  = m[1][1];
        out[6]  = m[2][1];
        out[7]  = m[3][1];
        out[8]  = m[0][2];
        out[9]  = m[1][2];
        out[10] = m[2][2];
        out[11] = m[3][2];
        out[12] = m[0][3];
        out[13] = m[1][3];
        out[14] = m[2][3];
        out[15] = m[3][3];
        // for (int i = 0; i < 4; ++i) {
        //     for (int j = 0; j < 4; ++j) {
        //         out[j * 4 + i] = static_cast<float>(m[i][j]);
        //     }
        // }
    }

    // Overload for non-const access
    std::array<T, 4>& operator[](size_t row) {
        return m[row];
    }

    // Overload for const access
    const std::array<T, 4>& operator[](size_t row) const {
        return m[row];
    }

private:
    static Mat4<T> scalingMatrix(T x, T y, T z) {
        Mat4<T> mat = Mat4<T>::identity();
        mat.m[0][0] = x;
        mat.m[1][1] = y;
        mat.m[2][2] = z;
        return mat;
    }

    static Mat4<T> translationMatrix(T x, T y, T z) {
        Mat4<T> mat = Mat4<T>::identity();
        mat.m[0][3] = x;
        mat.m[1][3] = y;
        mat.m[2][3] = z;
        return mat;
    }

    static Mat4<T> rotationMatrix(T x, T y, T z) {
        T sin_x = sin(x);
        T cos_x = cos(x);
        T sin_y = sin(y);
        T cos_y = cos(y);
        T sin_z = sin(z);
        T cos_z = cos(z);

        Mat4<T> mat;
        mat.m[0][0] = cos_y * cos_z;
        mat.m[0][1] = -cos_y * sin_z;
        mat.m[0][2] = sin_y;
        mat.m[0][3] = 0;
        mat.m[1][0] = sin_x * sin_y * cos_z + cos_x * sin_z;
        mat.m[1][1] = -sin_x * sin_y * sin_z + cos_x * cos_z;
        mat.m[1][2] = -sin_x * cos_y;
        mat.m[1][3] = 0;
        mat.m[2][0] = -cos_x * sin_y * cos_z + sin_x * sin_z;
        mat.m[2][1] = cos_x * sin_y * sin_z + sin_x * cos_z;
        mat.m[2][2] = cos_x * cos_y;
        mat.m[2][3] = 0;
        mat.m[3][0] = 0;
        mat.m[3][1] = 0;
        mat.m[3][2] = 0;
        mat.m[3][3] = 1;
        return mat;
    }
};














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
    return  (r << RGBA_R_SHIFT) |
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
    return  ((rgba)(rgba_r(rgba1) * v) << RGBA_R_SHIFT) |
            ((rgba)(rgba_g(rgba1) * v) << RGBA_G_SHIFT) |
            ((rgba)(rgba_b(rgba1) * v) << RGBA_B_SHIFT) |
            ((rgba)(rgba_a(rgba1) * v) << RGBA_A_SHIFT);
}

static inline rgba rgba_lerp(const rgba rgba1, const rgba rgba2, f64 t) {
    return  ((rgba)lerp((f64)rgba_r(rgba1), (f64)rgba_r(rgba2), t) << RGBA_R_SHIFT) |
            ((rgba)lerp((f64)rgba_g(rgba1), (f64)rgba_g(rgba2), t) << RGBA_G_SHIFT) |
            ((rgba)lerp((f64)rgba_b(rgba1), (f64)rgba_b(rgba2), t) << RGBA_B_SHIFT) |
            ((rgba)lerp((f64)rgba_a(rgba1), (f64)rgba_a(rgba2), t) << RGBA_A_SHIFT);
}

static inline rgba rgba_lerp_color(const rgba rgba1, const rgba rgba2, f64 t) {
    return  ((rgba)lerp((f64)rgba_r(rgba1), (f64)rgba_r(rgba2), t) << RGBA_R_SHIFT) |
            ((rgba)lerp((f64)rgba_g(rgba1), (f64)rgba_g(rgba2), t) << RGBA_G_SHIFT) |
            ((rgba)lerp((f64)rgba_b(rgba1), (f64)rgba_b(rgba2), t) << RGBA_B_SHIFT) |
            (rgba1 & RGBA_A_MASK);
}











#endif