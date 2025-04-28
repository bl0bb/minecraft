#include <random>

#include "ambient_occlusion.h"



template<typename T>
T mix(const T& x, const T& y, float a)
{
    return x * (1.0f - a) + y * a;
}



GLuint generateSSAOTexture(std::vector<Vec3<f32>>& ssaoKernel, std::vector<Vec3<f32>>& ssaoNoise) {
    // SSAO Kernel
    std::uniform_real_distribution<f32> randomFloats(0.0, 1.0);
    std::default_random_engine generator;

    for (u16 i = 0; i < 64; ++i) {
        Vec3<f32> sample(
            randomFloats(generator) * 2.0 - 1.0, // x: [-1, 1]
            randomFloats(generator) * 2.0 - 1.0, // y: [-1, 1]
            randomFloats(generator)             // z: [0, 1]
        );
        sample = sample.normalized();
        sample = sample * randomFloats(generator);

        // Scale samples to be closer to origin
        f32 scale = f32(i) / 64.0;
        scale = mix(0.1f, 1.0f, scale * scale);
        sample = sample * scale;
        ssaoKernel.push_back(sample);
    }

    // SSAO Noise Texture
    for (u16 i = 0; i < 16; i++) {
        Vec3<f32> noise(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            0.0f
        );
        ssaoNoise.push_back(noise);
    }

    GLuint noiseTexture;
    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return noiseTexture;
}