#version 460 core

layout(binding = 1) uniform sampler2DArray texArray;

in flat int texIndex;
in vec2 texUv;

out vec4 FragColor;

void main() {
    // texIndex selects the layer of the array
    FragColor = texture(texArray, vec3(texUv, texIndex));
}
