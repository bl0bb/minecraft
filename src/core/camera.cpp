#include "camera.h"

void Camera::handleResolution(int width, int height) {
    ratio = (f32)width / (f32)height;

    projection = Mat4<f32>::perspective(Math::deg_to_rad<f32>(fov), ratio, nearD, farD);
}

Mat4<f32> Camera::getViewMatrix() {
    Vec3<f32> intraVoxelPos = position;// - position.floor();
    return Mat4<f32>::lookAt(intraVoxelPos, intraVoxelPos + front, up);
}

void Camera::processMouseMovement(f32 xOffset, f32 yOffset) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw   += xOffset;
    pitch += yOffset;

    if (pitch > 89.9f) pitch = 89.9f;
    if (pitch < -89.9f) pitch = -89.9f;

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    Vec3<f32> f;
    f.x = cos(Math::deg_to_rad<f32>(yaw)) * cos(Math::deg_to_rad<f32>(pitch));
    f.y = sin(Math::deg_to_rad<f32>(pitch));
    f.z = sin(Math::deg_to_rad<f32>(yaw)) * cos(Math::deg_to_rad<f32>(pitch));
    front = f.normalized();
    right = front.cross(worldUp).normalized();
    up    = right.cross(front).normalized();
}