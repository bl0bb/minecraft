
#if OGL_VERSION == 46
layout(binding = 0) uniform sampler2DArray texArray;
#define TEX_LAYOUT layout(binding = 1) readonly buffer texDataBuffer {\
  uint texData[];\
};
#elif OGL_VERSION == 41
uniform sampler2DArray texArray;
#define TEX_LAYOUT layout(std140) uniform texDataBuffer {\
    uint texData[69];\
};
#endif

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 gPosition;
layout(location = 2) out vec4 gNormal;

in vec2 texUv;
flat in uint Axis;


flat in uint Ao;
in vec3 FragPos;


TEX_LAYOUT

const vec3 normalLookup[6] = vec3[6](
  vec3( 1,  0,  0 ),
  vec3(-1,  0,  0 ),
  vec3( 0,  1,  0 ),
  vec3( 0, -1,  0 ),
  vec3( 0,  0,  1 ),
  vec3( 0,  0, -1 )
);

const float lightLookup[6] = float[6](
  0.9,
  0.6,
  1.0,
  0.4,
  0.6,
  0.9
);

void main() {
  bool isNegative = (Axis & 1u) == 1;

  // // texIndex selects the layer of the array
  // vec4 sampled = texture(texArray, vec3(texUv, texIndex));

  // uint nrChannels = texData[texIndex];
  // if (nrChannels == 1) {
  //     FragColor = vec4(vec3(sampled.r), 1.0);
  // } else if (nrChannels == 2) {
  //     // TODO
  //     FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  // } else if (nrChannels == 3) {
  //     // FragColor = sampled;
  //     FragColor = vec4(vec3(sampled), 1.0);
  // } else if (nrChannels == 4) {
  //     FragColor = sampled;
  // }

  // if (FragColor.w == 0) {
  //   discard;
  // }


  // // directional darkness thingy
  // FragColor *= vec4(vec3(lightLookup[Axis]), 1.0);

  // FragColor = vec4(1.0, 1.0, 0.0, 0.0);
  FragColor = vec4(1,0,1,1);







  // other stuff
  gPosition = vec4(FragPos, 1.0);
  gNormal = vec4(normalize(normalLookup[Axis]), 1.0);
}