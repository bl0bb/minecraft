#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D ssao;

void main() {
    vec3 color = vec3(0.3, 0.5, 0.7); // Fake albedo color
    float ambientOcclusion = texture(ssao, TexCoords).r;

    vec3 ambient = vec3(ambientOcclusion) * color;
    FragColor = vec4(ambient, 1.0);
}
