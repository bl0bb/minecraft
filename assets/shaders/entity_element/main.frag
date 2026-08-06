
#if OGL_VERSION == 46
layout(binding = 0) uniform sampler2D skinTexture;
#elif OGL_VERSION == 41
uniform sampler2D skinTexture;
#endif

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 gPosition;
layout(location = 2) out vec4 gNormal;

in vec2 texUv;
flat in uint Axis;
in vec3 FragPos;

const vec3 normalLookup[6] = vec3[6](
  vec3( 1,  0,  0 ),
  vec3(-1,  0,  0 ),
  vec3( 0,  1,  0 ),
  vec3( 0, -1,  0 ),
  vec3( 0,  0,  1 ),
  vec3( 0,  0, -1 )
);

void main() {
  bool isNegative = (Axis & 1u) == 1;

  // texIndex selects the layer of the array
  vec4 sampled = texture(skinTexture, texUv);

  FragColor = sampled;

  if (FragColor.w == 0) {
    discard;
  }

  // FragColor = vec4(texUv, 0.0, 1.0);







  // other stuff
  gPosition = vec4(FragPos, 1.0);
  gNormal = vec4(normalize(normalLookup[Axis]), 1.0);
}