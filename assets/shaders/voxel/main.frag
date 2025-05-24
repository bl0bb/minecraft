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
in vec3 LightColor;
in float Sunlight;
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

// Function to compute AO at each corner
float getAO(float side1, float side2, float corner) {
    if (side1 == 1.0 && side2 == 1.0)
        return 0.0;
    return 1.0 - (side1 + side2 + corner) / 3.0;
}

void main() {
  bool isNegative = (Axis & 1) == 1;

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

  // directional darkness thingy
  FragColor *= vec4(vec3(lightLookup[Axis]), 1.0);

  // light / sunlight
  vec3 finalLight = clamp(vec3(Sunlight) + LightColor, 0.0, 1.0);

  FragColor *= vec4(finalLight, 1.0);

  




  // ao
  
  /*
  7 8 1
  6 9 2
  5 4 3
  */

  uint ao1 = (Ao >> 0) & 1;
  uint ao2 = (Ao >> 1) & 1;
  uint ao3 = (Ao >> 2) & 1;
  uint ao4 = (Ao >> 3) & 1;
  uint ao5 = (Ao >> 4) & 1;
  uint ao6 = (Ao >> 5) & 1;
  uint ao7 = (Ao >> 6) & 1;
  uint ao8 = (Ao >> 7) & 1;
  // center
  uint ao9 = (Ao >> 8) & 1;


  vec2 fixedAoUV = AoUV;
  if (Axis == 1) {
    fixedAoUV.x = 1.0f - fixedAoUV.x;
  } else if (Axis == 3) {
    fixedAoUV.y = 1.0f - fixedAoUV.y;
  } else if (Axis == 4) {
    fixedAoUV.x = 1.0f - fixedAoUV.x;
  }

  // Compute AO for each corner of the quad
  float aoTR = getAO(ao4, ao2, ao3); // top-right
  float aoTL = getAO(ao4, ao6, ao5); // top-left
  float aoBL = getAO(ao8, ao6, ao7); // bottom-left
  float aoBR = getAO(ao8, ao2, ao1); // bottom-right

  // Interpolate AO based on UV
  float top = mix(aoTL, aoTR, fixedAoUV.x);
  float bottom = mix(aoBL, aoBR, fixedAoUV.x);

  float ao = mix(top, bottom, fixedAoUV.y);

  // center
  // ao *= 1.0 - ao9;

  // Apply AO to output color (for example darken by AO)
  FragColor *= vec4(vec3(mix(0.4, 1.0, ao)), 1.0);







  // other stuff
  gPosition = vec4(FragPos, 1.0);
  gNormal = vec4(normalize(normalLookup[Axis]), 1.0);
}