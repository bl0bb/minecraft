#if OGL_VERSION == 46
#define DATA_LAYOUT \
struct QuadData {\
  uint uv_x;\
  uint uv_y;\
  uint uv_w;\
  uint uv_h;\
  uint uv_rot;\
  uint dir;\
};\
layout(std430, binding = 0) readonly buffer instanceDataBuffer {\
  QuadData instanceData[];\
};
#elif OGL_VERSION == 41
#define DATA_LAYOUT \
layout(location =  0) in uvec2 data_uv_pos;\
layout(location =  1) in uvec2 data_uv_size;\
layout(location =  2) in  uint data_uv_rot;\
layout(location =  3) in  uint data_dir;
#endif


flat out int texIndex;
out vec2 texUv;
flat out uint Axis;
out vec2 FaceUV;



flat out uint Ao;
out vec3 FragPos;

DATA_LAYOUT

uniform mat4 u_view;
uniform mat4 u_projection;

uniform ivec3 eye_position_int;

uniform ivec3 chunk_pos;

const vec2 vertexLookup[4] = vec2[4](
  vec2(0,  0),
  vec2(1,  0),
  vec2(1,  1),
  vec2(0,  1)
);

const vec3 normalLookup[6] = vec3[6](
  vec3( 1,  0,  0 ),
  vec3(-1,  0,  0 ),
  vec3( 0,  1,  0 ),
  vec3( 0, -1,  0 ),
  vec3( 0,  0,  1 ),
  vec3( 0,  0, -1 )
);

const float aoLookup[] = float[](
  1.0,
  0.7,
  0.5,
  0.15
);



// deg rad
#define M_PI 3.14159265358979323846
float deg_to_rad(float deg) {
  return deg * (M_PI / 180.0);
}

float rad_to_deg(float rad) {
  return rad * (180.0 / M_PI);
}

mat3 rotationMatrix(vec3 r) {
  float cx = cos(r.x), sx = sin(r.x);
  float cy = cos(r.y), sy = sin(r.y);
  float cz = cos(r.z), sz = sin(r.z);

  mat3 Rx = mat3(
      1.0, 0.0, 0.0,
      0.0, cx, -sx,
      0.0, sx, cx
  );

  mat3 Ry = mat3(
      cy, 0.0, sy,
      0.0, 1.0, 0.0,
      -sy, 0.0, cy
  );

  mat3 Rz = mat3(
      cz, -sz, 0.0,
      sz, cz, 0.0,
      0.0, 0.0, 1.0
  );

  return Rx * Ry * Rz;  // Rotation order: XYZ
}

void main() {
  uint vertexId = gl_VertexID % 6;
  uint faceId = gl_VertexID / 6;
  vec2 aPos;
  if (vertexId == 0 || vertexId == 5) {
    aPos = vertexLookup[2];
  } else if (vertexId == 1) {
    aPos = vertexLookup[1];
  } else if (vertexId == 2 || vertexId == 3) {
    aPos = vertexLookup[0];
  } else { // vertexId == 4
    aPos = vertexLookup[3];
  }
  // if (vertexId == 0 || vertexId == 5) {
  //   aPos = vertexLookup[0];
  // } else if (vertexId == 1) {
  //   aPos = vertexLookup[3];
  // } else if (vertexId == 2 || vertexId == 3) {
  //   aPos = vertexLookup[2];
  // } else { // vertexId == 4
  //   aPos = vertexLookup[1];
  // }

  #if OGL_VERSION == 46
  QuadData data = instanceData[faceId];
  uvec3 data_pos = uvec3(data.x, data.y, data.z);
  vec3 data_face_pos = vec3(data.face_x, data.face_y, data.face_z);
  vec2 data_face_size = vec2(data.face_width, data.face_height);
  vec3 data_face_rot = vec3(data.face_rot_x, data.face_rot_y, data.face_rot_z);
  uint data_uv_rot = data.uv_rot;
  uvec2 data_uv_pos = uvec2(data.uv_x, data.uv_y);
  uvec2 data_uv_size = uvec2(data.uv_w, data.uv_h);
  uint data_dir = data.dir;
  uint data_type = data.type;
  uint data_ao = data.ao;
  #elif OGL_VERSION == 41

  #endif






  vec3 offset = data_pos;

  vec3 face_pos = data_face_pos / 16.0;
  vec2 face_size = data_face_size / 16.0;
  vec3 face_rot = data_face_rot;

  vec2 uvOffset = data_uv_pos / 16.0;
  vec2 uvSize = data_uv_size / 16.0;

  uint axis = data_dir;
  uint isNegative = axis & 1u;

  vec3 vertexPos = face_pos + (rotationMatrix(face_rot) * vec3(aPos * face_size, 0));

  gl_Position = u_projection * u_view * vec4(chunk_pos * 32 + offset + vertexPos, 1.0);






  texUv = uvOffset + aPos * uvSize;
  texUv.x = 1 - texUv.x;
  // texUv.y = 1 - texUv.y;

  uint uvRot = data_uv_rot;
  if (uvRot == 1) {
    texUv = vec2(texUv.y, 1.0 - texUv.x);
  } else if (uvRot == 2) {
    texUv = vec2(1.0 - texUv.x, 1.0 - texUv.y);
  } else if (uvRot == 3) {
    texUv = vec2(1.0 - texUv.y, texUv.x);
  }




  // ao
  Ao = data_ao;

  // other
  FaceUV = aPos;
  Axis = axis;
  FragPos = vec3(gl_Position);
}