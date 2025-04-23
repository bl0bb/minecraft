#version 460 core

struct QuadData {
  uint data1;
  uint data2;
};

layout(std430, binding = 0) buffer Positions {
    QuadData data[];
};

uniform mat4 u_view;
uniform mat4 u_projection;
uniform ivec3 eye_position_int;

void main() {
    QuadData data = data[gl_VertexID];
    uint data1 = data.data1;
    uint data2 = data.data2;

    // Normalize or scale if needed. Here we just convert to float:
    vec3 pos = vec3(data1 & 31, (data1 >> 5) & 31, (data1 >> 10) & 31); 

    // Example: Just map directly to clip space for simple test
    gl_Position = u_projection * u_view * vec4(pos, 1);
}