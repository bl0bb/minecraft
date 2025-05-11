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
  vec3 offset = vec3(data1 & 31, (data1 >> 5) & 31, (data1 >> 10) & 31);

  vec2 face_offset = vec2((data1 >> 15) & 15, (data1 >> 19) & 15) / 16.0f;
  vec2 face_size = (vec2((data1 >> 27) & 15, ((data2 & 7) << 1) | ((data1 >> 31) & 1)) + 1) / 16.0f;
  float depth = ((data1 >> 23) & 15) / 16.0f;

  uint axis = (data2 >> 3) & 7;
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





  texIndex = int((data2 >> 6) & 15);

  texUv = vec2(aPos.x * face_size.x, (1 - aPos.y * face_size.y));


  Axis = axis;
  FragPos = vec3(gl_Position);
}
