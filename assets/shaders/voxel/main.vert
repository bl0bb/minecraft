#if OGL_VERSION == 46
#define DATA_LAYOUT layout(std430, binding = 0) readonly buffer instanceDataBuffer {\
  QuadData instanceData[];\
};
#elif OGL_VERSION == 41
#define DATA_LAYOUT layout(location = 1) in uint quadData1;\
layout(location = 2) in uint quadData2;\
layout(location = 3) in uint quadData3;\
layout(location = 4) in uint quadData4;\
layout(location = 5) in uint quadData5;\
layout(location = 6) in uint quadData6;\
layout(location = 7) in uint quadData7;\
layout(location = 8) in uint quadData8;
#endif

layout(location = 0) in vec3 aPos;

flat out int texIndex;
out vec2 texUv;
flat out uint Axis;
out vec2 FaceUV;



flat out uint Light0;
flat out uint Light1;
flat out uint Light2;
flat out uint Light3;
flat out uint Light4;
flat out uint Light5;
flat out uint Light6;
flat out uint Light7;
flat out uint Light8;

// out vec3 LightColor;
// out float Sunlight;



flat out uint Ao;
out vec3 FragPos;

struct QuadData {
  uint data1;
  uint data2;
  uint data3;
  uint data4;
  uint data5;
  uint data6;
  uint data7;
  uint data8;
};

DATA_LAYOUT

uniform mat4 u_view;
uniform mat4 u_projection;

uniform ivec3 eye_position_int;

uniform ivec3 chunk_pos;

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

  #if OGL_VERSION == 46
  QuadData data = instanceData[gl_InstanceID];
  uint data1 = data.data1;
  uint data2 = data.data2;
  uint data3 = data.data3;
  uint data4 = data.data4;
  uint data5 = data.data5;
  uint data6 = data.data6;
  uint data7 = data.data7;
  uint data8 = data.data8;
  #elif OGL_VERSION == 41
  uint data1 = quadData1;
  uint data2 = quadData2;
  uint data3 = quadData3;
  uint data4 = quadData4;
  uint data5 = quadData5;
  uint data6 = quadData6;
  uint data7 = quadData7;
  uint data8 = quadData8;
  #endif

  uint data_x =          (data1 >>         0) & (      32u - 1);
  uint data_y =          (data1 >>         5) & (      32u - 1);
  uint data_z =          (data1 >>        10) & (      32u - 1);
  uint data_face_x =     (data1 >>        15) & (      16u - 1);
  uint data_face_y =     (data1 >>        19) & (      16u - 1);
  uint data_face_depth = (data1 >>        23) & (      16u - 1);
  uint data_face_w =     (data1 >>        27) & (      16u - 1);
  uint data_face_h =     ((data2 & 7u) << 1) | ((data1 >> 31) & 1u);
  uint data_uv_x =       (data2 >> (35 - 32)) & (      16u - 1);
  uint data_uv_y =       (data2 >> (39 - 32)) & (      16u - 1);
  uint data_uv_w =       (data2 >> (43 - 32)) & (      16u - 1);
  uint data_uv_h =       (data2 >> (47 - 32)) & (      16u - 1);
  uint data_uv_rot =     (data2 >> (51 - 32)) & (       4u - 1);
  uint data_dir =        (data2 >> (53 - 32)) & (       8u - 1);
  uint data_type =       (data2 >> (56 - 32)) & (     256u - 1);

  uint data_light0 =     (data3 >>         0) & ( 1048576u - 1);
  uint data_light1 =     ((data4 & (256u - 1)) << 8) | ((data3 >> 20) & (4096u - 1));
  uint data_light2 =     (data4 >> (40 - 32)) & (16777216u - 1);
  uint data_light3 =     ((data5 & (65536u - 1)) << 16) | ((data4 >> (60 - 32)) & (16u - 1));
  uint data_light4 =     ((data6 & (16u - 1)) << 4) | ((data5 >> 16) & (65536u - 1));
  uint data_light5 =     (data6 >>         4) & ( 1048576u - 1);
  uint data_light6 =     ((data7 & (4096u - 1)) << 8) | ((data6 >> (56 - 32)) & (256u - 1));
  uint data_light7 =     (data7 >>        12) & ( 1048576u - 1);
  uint data_light8 =     (data8 >> (32 - 32)) & ( 1048576u - 1);

  uint data_ao =         (data8 >> (52 - 32)) & (    512u - 1);

  vec3 offset = vec3(data_x, data_y, data_z);

  vec2 face_offset = vec2(data_face_x, data_face_y) / 16.0;
  float depth = data_face_depth / 16.0;
  vec2 face_size = (vec2(data_face_w, data_face_h) + 1) / 16.0;

  vec2 uvOffset = vec2(data_uv_x, data_uv_y) / 16.0;
  vec2 uvSize = (vec2(data_uv_w, data_uv_h) + 1) / 16.0;

  uint axis = data_dir;
  uint isNegative = axis & 1u;

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




  // light / sunlight
  // uint lightR = (data_light8 >> (4 * 0)) & 15;
  // uint lightG = (data_light8 >> (4 * 1)) & 15;
  // uint lightB = (data_light8 >> (4 * 2)) & 15;
  // uint lightI = (data_light8 >> (4 * 3)) & 15;
  // uint lightS = (data_light8 >> (4 * 4)) & 15;

  // LightColor = vec3(lightR, lightG, lightB) / 15.0;

  // Sunlight = lightS / 15.0;

  Light0 = data_light0;
  Light1 = data_light1;
  Light2 = data_light2;
  Light3 = data_light3;
  Light4 = data_light4;
  Light5 = data_light5;
  Light6 = data_light6;
  Light7 = data_light7;
  Light8 = data_light8;





  // ao
  Ao = data_ao;

  // other
  FaceUV = vec2(aPos);
  Axis = axis;
  FragPos = vec3(gl_Position);
}