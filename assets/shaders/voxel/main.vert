#version 460 core

layout(location = 0) in vec3 aPos;

out flat int texIndex; // Pass to fragment shader without interpolation
out vec2 texUv; // Pass to fragment shader without interpolation
out flat uint Axis;
// ao
out vec3 FragPos;

struct QuadData {
  uint data1;
  uint data2;
};

layout(std430, binding = 0) readonly buffer instanceDataBuffer {
  QuadData instanceData[];
};

uniform mat4 u_view;
uniform mat4 u_projection;

uniform ivec3 eye_position_int;

uniform ivec3 chunk_pos;

const vec3 normalLookup[6] = {
  vec3( 1,  0,  0 ),
  vec3(-1,  0,  0 ),
  vec3( 0,  1,  0 ),
  vec3( 0, -1,  0 ),
  vec3( 0,  0,  1 ),
  vec3( 0,  0, -1 ),
};



void main() {
  QuadData data = instanceData[gl_InstanceID];
  uint data1 = data.data1;
  uint data2 = data.data2;

  uint data_x =          (data1 >>         0) & ( 32 - 1);
  uint data_y =          (data1 >>         5) & ( 32 - 1);
  uint data_z =          (data1 >>        10) & ( 32 - 1);
  uint data_face_x =     (data1 >>        15) & ( 16 - 1);
  uint data_face_y =     (data1 >>        19) & ( 16 - 1);
  uint data_face_depth = (data1 >>        23) & ( 16 - 1);
  uint data_face_w =     (data1 >>        27) & ( 16 - 1);
  uint data_face_h =     ((data2 & 7) << 1) | ((data1 >> 31) & 1); // (data1 >> 31) & (16 - 1);
  uint data_uv_x =       (data2 >> (35 - 32)) & ( 16 - 1);
  uint data_uv_y =       (data2 >> (39 - 32)) & ( 16 - 1);
  uint data_uv_w =       (data2 >> (43 - 32)) & ( 16 - 1);
  uint data_uv_h =       (data2 >> (47 - 32)) & ( 16 - 1);
  uint data_uv_rot =     (data2 >> (51 - 32)) & (  4 - 1);
  uint data_dir =        (data2 >> (53 - 32)) & (  8 - 1);
  uint data_type =       (data2 >> (56 - 32)) & (256 - 1);

  vec3 offset = vec3(data_x, data_y, data_z);

  vec2 face_offset = vec2(data_face_x, data_face_y) / 16.0f;
  float depth = data_face_depth / 16.0f;
  vec2 face_size = (vec2(data_face_w, data_face_h) + 1) / 16.0f;

  vec2 uvOffset = vec2(data_uv_x, data_uv_y) / 16.0f;
  vec2 uvSize = (vec2(data_uv_w, data_uv_h) + 1) / 16.0f;

  uint axis = data_dir;
  uint isNegative = axis & 1;

  vec2 fixedPos = face_offset + (face_size * vec2(aPos));

  vec3 vertexPos;

  if (axis == 0) {
    // right (+X)
    vertexPos.x = 1.0f - depth;
    vertexPos.y = fixedPos.y;
    vertexPos.z = fixedPos.x - 1.0f; // 1.0f - fixedPos.x;
  } else if (axis == 1) {
    // left (-X)
    vertexPos.x = depth;
    vertexPos.y = fixedPos.y;
    vertexPos.z = -fixedPos.x; // fixedPos.x;
  } else if (axis == 2) {
    // top (+Y)
    vertexPos.x = fixedPos.x;
    vertexPos.y = 1.0f - depth;
    vertexPos.z = fixedPos.y - 1.0f; // 1.0f - fixedPos.y;
  } else if (axis == 3) {
    // bottom (-Y)
    vertexPos.x = fixedPos.x;
    vertexPos.y = depth;
    vertexPos.z = -fixedPos.y; // fixedPos.y;
  } else if (axis == 4) {
    // back (+Z)
    vertexPos.x = 1.0f - fixedPos.x;
    vertexPos.y = fixedPos.y;
    vertexPos.z = -depth; // 0.0f;
  } else {
    // front (-Z)
    vertexPos.x = fixedPos.x;
    vertexPos.y = fixedPos.y;
    vertexPos.z = -1.0f + depth; // 1.0f;
  }

  gl_Position = u_projection * u_view * vec4(chunk_pos * 32 + offset + vertexPos, 1.0);





  texIndex = int(data_type);

  texUv = uvOffset + vec2(aPos) * uvSize;
  texUv.y = 1 - texUv.y;

  uint uvRot = data_uv_rot;
  if (uvRot == 1) {
    texUv = vec2(texUv.y, 1.0f - texUv.x);
  } else if (uvRot == 2) {
    texUv = vec2(1.0f - texUv.x, 1.0f - texUv.y);
  } else if (uvRot == 3) {
    texUv = vec2(1.0f - texUv.y, texUv.x);
  }




  Axis = axis;
  FragPos = vec3(gl_Position);
}
