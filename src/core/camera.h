#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <math.h>

#include "maths.h"

const f32 YAW         =  0.0f;
const f32 PITCH       =  0.0f;
const f32 SENSITIVITY =  0.075f;
const f32 FOV         =  80.0f;

class Camera {
public:
    Vec3<f32> position;
    Vec3<f32> front = Vec3<f32>(0.0f, 0.0f, -1.0f);
    Vec3<f32> up = Vec3<f32>(0.0f, 1.0f, 0.0f);
    Vec3<f32> right;
    Vec3<f32> worldUp = Vec3<f32>(0.0f, 1.0f, 0.0f);
    Mat4<f32> projection;
    f32 yaw = YAW;
    f32 pitch = PITCH;
    f32 mouseSensitivity = SENSITIVITY;
    f32 fov = FOV;
    f32 nearD = 0.1f;
    f32 farD = 10000.0f;
    f32 ratio;

    Camera(Vec3<f32> position) : position(position) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        handleResolution(mode->width, mode->height);
        updateCameraVectors();
    }

    void handleResolution(int width, int height);

    inline void updatePosition(Vec3<f32> pos) {
        position = pos;
    }

    Mat4<f32> getViewMatrix();

    void processMouseMovement(f32 xOffset, f32 yOffset);

private:
    void updateCameraVectors();
};

#endif