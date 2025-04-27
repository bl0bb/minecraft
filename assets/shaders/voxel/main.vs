#version 460 core

layout(location = 0) in vec3 aPos;

out flat int texIndex; // Pass to fragment shader without interpolation
out vec2 texUv; // Pass to fragment shader without interpolation

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

const vec3 normalLookup[6] = {
  vec3( 0,  1,  0 ),
  vec3( 0, -1,  0 ),
  vec3( 1,  0,  0 ),
  vec3(-1,  0,  0 ),
  vec3( 0,  0,  1 ),
  vec3( 0,  0, -1 )
};



void main() {
  QuadData data = instanceData[gl_InstanceID];
  uint data1 = data.data1;
  uint data2 = data.data2;
  vec3 offset = vec3(data1 & 31, (data1 >> 5) & 31, (data1 >> 10) & 31);

  uint axis = (data1 >> 25) & 7;
  uint isNegative = axis & 1;

  vec3 vertexPos;

  if (axis == 0) {
    // top (+Y)
    vertexPos.x = aPos.x;
    vertexPos.y = 1.0f;
    vertexPos.z = aPos.y - 1.0f; // 1.0f - aPos.y;
  } else if (axis == 1) {
    // bottom (-Y)
    vertexPos.x = aPos.x;
    vertexPos.y = 0.0f;
    vertexPos.z = -aPos.y; // aPos.y;
  } else if (axis == 2) {
    // right (+X)
    vertexPos.x = 1.0f;
    vertexPos.y = aPos.y;
    vertexPos.z = aPos.x - 1.0f; // 1.0f - aPos.x;
  } else if (axis == 3) {
    // left (-X)
    vertexPos.x = 0.0f;
    vertexPos.y = aPos.y;
    vertexPos.z = -aPos.x; // aPos.x;
  } else if (axis == 4) {
    // back (+Z)
    vertexPos.x = 1.0f - aPos.x;
    vertexPos.y = aPos.y;
    vertexPos.z = 0.0f; // 0.0f;
  } else if (axis == 5) {
    // front (-Z)
    vertexPos.x = aPos.x;
    vertexPos.y = aPos.y;
    vertexPos.z = -1.0f; // 1.0f;
  }

  gl_Position = u_projection * u_view * vec4(vertexPos + offset, 1.0);





  texIndex = int((data1 >> 28) & 15) - 1;
  texUv = vec2(aPos.x, 1 - aPos.y);
}
