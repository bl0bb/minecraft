#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float texelWidth;
uniform float texelHeight;

void main() {
    // Kernel offsets
    vec2 offsets[9] = vec2[](
        vec2(-texelWidth,  texelHeight), // top-left
        vec2(0.0f,         texelHeight), // top-center
        vec2(texelWidth,   texelHeight), // top-right
        vec2(-texelWidth,  0.0f),        // mid-left
        vec2(0.0f,         0.0f),        // center
        vec2(texelWidth,   0.0f),        // mid-right
        vec2(-texelWidth, -texelHeight), // bottom-left
        vec2(0.0f,        -texelHeight), // bottom-center
        vec2(texelWidth,  -texelHeight)  // bottom-right
    );

    // Grayscale kernel (luminance)
    float kernel[9];
    for (int i = 0; i < 9; ++i) {
        vec3 sample = texture(screenTexture, TexCoords + offsets[i]).rgb;
        kernel[i] = dot(sample, vec3(0.2126, 0.7152, 0.0722)); // luminance
    }

    // Sobel edge detection
    float gx = kernel[2] + 2.0 * kernel[5] + kernel[8]
             - kernel[0] - 2.0 * kernel[3] - kernel[6];
    float gy = kernel[0] + 2.0 * kernel[1] + kernel[2]
             - kernel[6] - 2.0 * kernel[7] - kernel[8];

    float edge = sqrt(gx * gx + gy * gy);

    FragColor = vec4(vec3(edge), 1.0);
}
