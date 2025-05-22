#version 460 core

layout(binding = 0) uniform sampler2DArray texArray;

layout(binding = 1) readonly buffer texDataBuffer {
  uint texData[];
};

layout(location = 1) out vec4 gPosition;
layout(location = 2) out vec4 gNormal;

in flat int texIndex;
in vec2 texUv;
in flat uint Axis;
in flat vec3 LightColor;
in float Ao;
in vec3 FragPos;

out vec4 FragColor;

const vec3 normalLookup[6] = {
  vec3( 1,  0,  0 ),
  vec3(-1,  0,  0 ),
  vec3( 0,  1,  0 ),
  vec3( 0, -1,  0 ),
  vec3( 0,  0,  1 ),
  vec3( 0,  0, -1 ),
};

const float lightLookup[6] = {
  0.9,
  0.6,
  1.0,
  0.4,
  0.6,
  0.9,
};

void main() {
    // texIndex selects the layer of the array
    vec4 sampled = texture(texArray, vec3(texUv, texIndex));

    uint nrChannels = texData[texIndex];
    if (nrChannels == 1) {
        FragColor = vec4(vec3(sampled.r), 1.0);
    } else if (nrChannels == 2) {
        // TODO
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else if (nrChannels == 3) {
        // FragColor = sampled;
        FragColor = vec4(vec3(sampled), 1.0);
    } else if (nrChannels == 4) {
        FragColor = sampled;
    }

    FragColor *= vec4(vec3(lightLookup[Axis]), 1.0);

    // FragColor *= vec4(LightColor, 1.0);
    FragColor *= Ao;

    // ao
    gPosition = vec4(FragPos, 1.0);
    gNormal = vec4(normalize(normalLookup[Axis]), 1.0);
}