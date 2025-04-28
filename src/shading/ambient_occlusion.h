#ifndef AMBIENT_OCCLUSION_H
#define AMBIENT_OCCLUSION_H

#include <glad/glad.h>

#include <vector>

#include "../core/types.h"
#include "../core/maths.h"

GLuint generateSSAOTexture(std::vector<Vec3<f32>>& ssaoKernel, std::vector<Vec3<f32>>& ssaoNoise);

#endif