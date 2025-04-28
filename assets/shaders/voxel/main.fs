#version 460 core

layout(binding = 0) uniform sampler2DArray texArray;

layout(location = 1) out vec4 gPosition;
layout(location = 2) out vec4 gNormal;

in flat int texIndex;
in vec2 texUv;
// ao
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main() {
    // texIndex selects the layer of the array
    FragColor = texture(texArray, vec3(texUv, texIndex));

    // ao
    gPosition = vec4(FragPos, 1.0);
    gNormal = vec4(normalize(Normal), 1.0);
}
