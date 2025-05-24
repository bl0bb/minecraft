#version 460 core

layout(location = 0) in vec3 aPos;

out flat int texIndex;
out vec2 texUv;
out flat uint Axis;
out vec3 LightColor;
out vec2 AoUV;
out flat uint Ao;
out vec3 FragPos;

struct QuadData {
  uint data1;
  uint data2;
  uint data3;
  uint data4;
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

// TODO: "aoLookup[4]" instead of "aoLookup[]"?
const float aoLookup[] = {
  1.0,
  0.7,
  0.5,
  0.15,
};



void main() {
  uint vertex_id;
  if (gl_VertexID == 0) {
    vertex_id = 0;
  } else if (gl_VertexID == 1) {
    vertex_id = 1;
  } else if (gl_VertexID == 2) {
    vertex_id = 2;
  } else {
    vertex_id = 3;
  }

  QuadData data = instanceData[gl_InstanceID];
  uint data1 = data.data1;
  uint data2 = data.data2;
  uint data3 = data.data3;
  uint data4 = data.data4;

  uint data_x =          (data1 >>         0) & (     32 - 1);
  uint data_y =          (data1 >>         5) & (     32 - 1);
  uint data_z =          (data1 >>        10) & (     32 - 1);
  uint data_face_x =     (data1 >>        15) & (     16 - 1);
  uint data_face_y =     (data1 >>        19) & (     16 - 1);
  uint data_face_depth = (data1 >>        23) & (     16 - 1);
  uint data_face_w =     (data1 >>        27) & (     16 - 1);
  uint data_face_h =     ((data2 & 7) << 1) | ((data1 >> 31) & 1); // (data1 >> 31) & (16 - 1);
  uint data_uv_x =       (data2 >> (35 - 32)) & (     16 - 1);
  uint data_uv_y =       (data2 >> (39 - 32)) & (     16 - 1);
  uint data_uv_w =       (data2 >> (43 - 32)) & (     16 - 1);
  uint data_uv_h =       (data2 >> (47 - 32)) & (     16 - 1);
  uint data_uv_rot =     (data2 >> (51 - 32)) & (      4 - 1);
  uint data_dir =        (data2 >> (53 - 32)) & (      8 - 1);
  uint data_type =       (data2 >> (56 - 32)) & (    256 - 1);
  uint data_light =      (data3 >>         0) & (1048576 - 1);
  uint data_ao =         (data3 >>        20) & (    512 - 1);

  vec3 offset = vec3(data_x, data_y, data_z);

  vec2 face_offset = vec2(data_face_x, data_face_y) / 16.0;
  float depth = data_face_depth / 16.0;
  vec2 face_size = (vec2(data_face_w, data_face_h) + 1) / 16.0;

  vec2 uvOffset = vec2(data_uv_x, data_uv_y) / 16.0;
  vec2 uvSize = (vec2(data_uv_w, data_uv_h) + 1) / 16.0;

  uint axis = data_dir;
  uint isNegative = axis & 1;

  vec2 fixedPos = face_offset + (face_size * vec2(aPos));

  vec3 vertexPos;

  if (axis == 0) {
    // right (+X)
    vertexPos.x = 1.0 - depth;
    vertexPos.y = fixedPos.y;
    vertexPos.z = fixedPos.x - 1.0; // 1.0 - fixedPos.x;
  } else if (axis == 1) {
    // left (-X)
    vertexPos.x = depth;
    vertexPos.y = fixedPos.y;
    vertexPos.z = -fixedPos.x; // fixedPos.x;
  } else if (axis == 2) {
    // top (+Y)
    vertexPos.x = fixedPos.x;
    vertexPos.y = 1.0 - depth;
    vertexPos.z = fixedPos.y - 1.0; // 1.0 - fixedPos.y;
  } else if (axis == 3) {
    // bottom (-Y)
    vertexPos.x = fixedPos.x;
    vertexPos.y = depth;
    vertexPos.z = -fixedPos.y; // fixedPos.y;
  } else if (axis == 4) {
    // back (+Z)
    vertexPos.x = 1.0 - fixedPos.x;
    vertexPos.y = fixedPos.y;
    vertexPos.z = -depth; // 0.0;
  } else {
    // front (-Z)
    vertexPos.x = fixedPos.x;
    vertexPos.y = fixedPos.y;
    vertexPos.z = -1.0 + depth; // 1.0;
  }

  gl_Position = u_projection * u_view * vec4(chunk_pos * 32 + offset + vertexPos, 1.0);





  texIndex = int(data_type);

  texUv = uvOffset + vec2(aPos) * uvSize;
  texUv.y = 1 - texUv.y;

  uint uvRot = data_uv_rot;
  if (uvRot == 1) {
    texUv = vec2(texUv.y, 1.0 - texUv.x);
  } else if (uvRot == 2) {
    texUv = vec2(1.0 - texUv.x, 1.0 - texUv.y);
  } else if (uvRot == 3) {
    texUv = vec2(1.0 - texUv.y, texUv.x);
  }


  // light
  uint lightR = (data_light >> (4 * 0)) & 15;
  uint lightG = (data_light >> (4 * 1)) & 15;
  uint lightB = (data_light >> (4 * 2)) & 15;
  uint lightI = (data_light >> (4 * 3)) & 15;
  uint lightS = (data_light >> (4 * 4)) & 15;

  // float lightIFloat = lightI / 16.0;

  LightColor = vec3(lightR, lightG, lightB) / 16.0;

  // ao
  // uint ao_fill = 0;
  // ao_fill += (data_ao >> (((vertex_id * 2) + 0) % 8)) & 1;
  // ao_fill += (data_ao >> (((vertex_id * 2) + 1) % 8)) & 1;
  // ao_fill += (data_ao >> (((vertex_id * 2) + 2) % 8)) & 1;
  // Ao = 1.0 - (ao_fill / 1.0); // aoLookup[ao_fill];

  Ao = data_ao;
  AoUV = vec2(aPos);

  // other
  Axis = axis;
  FragPos = vec3(gl_Position);
}