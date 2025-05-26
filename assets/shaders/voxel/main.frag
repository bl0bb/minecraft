layout(binding = 0) uniform sampler2DArray texArray;

layout(binding = 1) readonly buffer texDataBuffer {
  uint texData[];
};

layout(location = 1) out vec4 gPosition;
layout(location = 2) out vec4 gNormal;

in flat int texIndex;
in vec2 texUv;
in flat uint Axis;
in vec2 FaceUV;


in flat uint Light0;
in flat uint Light1;
in flat uint Light2;
in flat uint Light3;
in flat uint Light4;
in flat uint Light5;
in flat uint Light6;
in flat uint Light7;
in flat uint Light8;

// in vec3 LightColor;
// in float Sunlight;


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

// Hardcoded corners
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

vec4 fixLight(uint light) {
  return vec4(
    (light >> 0) & 15, // r
    (light >> 4) & 15, // g
    (light >> 8) & 15, // b
    (light >> 16) & 15 // sunlight
  ) / 15.0;
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






  vec2 fixedFaceUV = FaceUV;
  if (Axis == 1) {
    fixedFaceUV.x = 1.0f - fixedFaceUV.x;
  } else if (Axis == 3) {
    fixedFaceUV.y = 1.0f - fixedFaceUV.y;
  } else if (Axis == 4) {
    fixedFaceUV.x = 1.0f - fixedFaceUV.x;
  }





  // light / sunlight
  // vec3 finalLight = clamp(vec3(Sunlight) + LightColor, 0.0, 1.0);
  // FragColor *= vec4(finalLight, 1.0);


  /*
  6 7 0
  5 8 1
  4 3 2

  7 8 1
  6 9 2
  5 4 3
  */
  

  

  vec4 fixedLight0 = fixLight(Light0);
  vec4 fixedLight1 = fixLight(Light1);
  vec4 fixedLight2 = fixLight(Light2);
  vec4 fixedLight3 = fixLight(Light3);
  vec4 fixedLight4 = fixLight(Light4);
  vec4 fixedLight5 = fixLight(Light5);
  vec4 fixedLight6 = fixLight(Light6);
  vec4 fixedLight7 = fixLight(Light7);
  // center
  vec4 fixedLight8 = fixLight(Light8);

  // Compute light for each corner of the quad
  vec4 lightTR = fixedLight2; // top-right
  vec4 lightTL = fixedLight4; // top-left
  vec4 lightBL = fixedLight6; // bottom-left
  vec4 lightBR = fixedLight0; // bottom-right

  // Interpolate AO based on UV
  vec4 lightTop = mix(lightTL, lightTR, fixedFaceUV.x);
  vec4 lightBottom = mix(lightBL, lightBR, fixedFaceUV.x);

  vec4 mixedLight = mix(lightTop, lightBottom, fixedFaceUV.y) * fixedLight8;
  FragColor *= vec4(vec3(mixedLight), 1.0);

  // vec4 fixedLight8 = fixLight(Light8);
  // FragColor *= vec4(vec3(fixedLight8), 1.0);






  // ao

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

  // Compute AO for each corner of the quad
  float aoTR = getAO(ao4, ao2, ao3); // top-right
  float aoTL = getAO(ao4, ao6, ao5); // top-left
  float aoBL = getAO(ao8, ao6, ao7); // bottom-left
  float aoBR = getAO(ao8, ao2, ao1); // bottom-right

  // Interpolate AO based on UV
  float aoTop = mix(aoTL, aoTR, fixedFaceUV.x);
  float aoBottom = mix(aoBL, aoBR, fixedFaceUV.x);

  float mixedAo = mix(aoTop, aoBottom, fixedFaceUV.y);

  // center
  // mixedAo *= 1.0 - ao9;

  // Apply AO to output color (for example darken by AO)
  FragColor *= vec4(vec3(mix(0.4, 1.0, mixedAo)), 1.0);







  // other stuff
  gPosition = vec4(FragPos, 1.0);
  gNormal = vec4(normalize(normalLookup[Axis]), 1.0);
}