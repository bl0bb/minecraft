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
in vec2 AoUV;
in flat uint Ao;
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

// Hardcoded corner colors (could also be uniforms)
// vec2 c1 = vec2(0.0, 0.5);
// vec2 c2 = vec2(0.0, 1.0);
// vec2 c3 = vec2(0.5, 1.0);
// vec2 c4 = vec2(1.0, 1.0);
// vec2 c5 = vec2(1.0, 0.5);
// vec2 c6 = vec2(1.0, 0.0);
// vec2 c7 = vec2(0.5, 0.0);
// // vec2 c8 = vec2(0.0, 0.0);

// vec2 c1 = vec2(0.0, 0.0);
// vec2 c2 = vec2(0.5, 0.0);
// vec2 c3 = vec2(1.0, 0.0);
// vec2 c4 = vec2(1.0, 0.5);
// vec2 c5 = vec2(1.0, 1.0);
// vec2 c6 = vec2(0.5, 1.0);
// vec2 c7 = vec2(0.0, 1.0);
// vec2 c8 = vec2(0.0, 0.5);

vec2 c1 = vec2(1.0, 1.0);
vec2 c2 = vec2(1.0, 0.5);
vec2 c3 = vec2(1.0, 0.0);
vec2 c4 = vec2(0.5, 0.0);
vec2 c5 = vec2(0.0, 0.0);
vec2 c6 = vec2(0.0, 0.5);
vec2 c7 = vec2(0.0, 1.0);
vec2 c8 = vec2(0.5, 1.0);

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
  // FragColor *= vec4(vec3(Ao), 1.0);
  
  uint ao1 = (Ao >> 0) & 1;
  uint ao2 = (Ao >> 1) & 1;
  uint ao3 = (Ao >> 2) & 1;
  uint ao4 = (Ao >> 3) & 1;
  uint ao5 = (Ao >> 4) & 1;
  uint ao6 = (Ao >> 5) & 1;
  uint ao7 = (Ao >> 6) & 1;
  uint ao8 = (Ao >> 7) & 1;

  float AoColor = 0.0;
  AoColor += (1 - (abs((c1 - AoUV).x) + abs((c1 - AoUV).y)) / 2) * ao1;
  AoColor += (1 - abs((c2 - AoUV).x) / 1) * ao2;
  AoColor += (1 - (abs((c3 - AoUV).x) + abs((c3 - AoUV).y)) / 2) * ao3;
  AoColor += (1 - abs((c4 - AoUV).y) / 1) * ao4;
  AoColor += (1 - (abs((c5 - AoUV).x) + abs((c5 - AoUV).y)) / 2) * ao5;
  AoColor += (1 - abs((c6 - AoUV).x) / 1) * ao6;
  AoColor += (1 - (abs((c7 - AoUV).x) + abs((c7 - AoUV).y)) / 2) * ao7;
  AoColor += (1 - abs((c8 - AoUV).y) / 1) * ao8;

  FragColor *= vec4(vec3(mix(1.0, 0.8, AoColor)), 1.0);

  // ao
  gPosition = vec4(FragPos, 1.0);
  gNormal = vec4(normalize(normalLookup[Axis]), 1.0);
}