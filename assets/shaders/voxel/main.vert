#if OGL_VERSION == 46
#define DATA_LAYOUT \
struct QuadData {\
  uint x;\
  uint y;\
  uint z;\
  float face_x;\
  float face_y;\
  float face_z;\
  float face_width;\
  float face_height;\
  float face_rot_x;\
  float face_rot_y;\
  float face_rot_z;\
  uint uv_rot;\
  uint uv_x;\
  uint uv_y;\
  uint uv_w;\
  uint uv_h;\
  uint dir;\
  uint type;\
  uint light[9];\
  uint ao;\
};\
layout(std430, binding = 0) readonly buffer instanceDataBuffer {\
  QuadData instanceData[];\
};
#elif OGL_VERSION == 41
#define DATA_LAYOUT \
layout(location = 0) in uint  quadData1;\
layout(location = 1) in uint  quadData2;\
layout(location = 2) in uint  quadData3;\
layout(location = 3) in uint  quadData4;\
layout(location = 4) in uint  quadData5;\
layout(location = 5) in uint  quadData6;\
layout(location = 6) in uint  quadData7;\
layout(location = 7) in uint  quadData8;\
layout(location = 8) in uint  quadData9;\
layout(location = 9) in uint quadData10;
#endif


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
  } else {
    aPos = vertexLookup[3];
  }

  #if OGL_VERSION == 46
  QuadData data = instanceData[faceId];
  uint data_x = data.x;
  uint data_y = data.y;
  uint data_z = data.z;
  float data_face_x = data.face_x;
  float data_face_y = data.face_y;
  float data_face_z = data.face_z;
  float data_face_width = data.face_width;
  float data_face_height = data.face_height;
  float data_face_rot_x = data.face_rot_x;
  float data_face_rot_y = data.face_rot_y;
  float data_face_rot_z = data.face_rot_z;
  uint data_uv_rot = data.uv_rot;
  uint data_uv_x = data.uv_x;
  uint data_uv_y = data.uv_y;
  uint data_uv_w = data.uv_w;
  uint data_uv_h = data.uv_h;
  uint data_dir = data.dir;
  uint data_type = data.type;
  uint data_light[9] = data.light;
  uint data_ao = data.ao;
  #elif OGL_VERSION == 41
  uint  data1 =  quadData1;
  uint  data2 =  quadData2;
  uint  data3 =  quadData3;
  uint  data4 =  quadData4;
  uint  data5 =  quadData5;
  uint  data6 =  quadData6;
  uint  data7 =  quadData7;
  uint  data8 =  quadData8;
  uint  data9 =  quadData9;
  uint data10 = quadData10;
  #endif






  vec3 offset = vec3(data_x, data_y, data_z);

  vec3 face_pos = vec3(data_face_x, data_face_y, data_face_z) / 16.0;
  vec2 face_size = vec2(data_face_width, data_face_height) / 16.0;
  vec3 face_rot = vec3(data_face_rot_x, data_face_rot_y, data_face_rot_z);

  vec2 uvOffset = vec2(data_uv_x, data_uv_y) / 16.0;
  vec2 uvSize = vec2(data_uv_w, data_uv_h) / 16.0;

  uint axis = data_dir;
  uint isNegative = axis & 1u;

  vec3 vertexPos = face_pos + (rotationMatrix(face_rot) * vec3(aPos * face_size, 0));

  gl_Position = u_projection * u_view * vec4(chunk_pos * 32 + offset + vertexPos, 1.0);





  texIndex = int(data_type);

  texUv = uvOffset + aPos * uvSize;
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
  Light0 = data_light[0];
  Light1 = data_light[1];
  Light2 = data_light[2];
  Light3 = data_light[3];
  Light4 = data_light[4];
  Light5 = data_light[5];
  Light6 = data_light[6];
  Light7 = data_light[7];
  Light8 = data_light[8];





  // ao
  Ao = data_ao;

  // other
  FaceUV = aPos;
  Axis = axis;
  FragPos = vec3(gl_Position);
}