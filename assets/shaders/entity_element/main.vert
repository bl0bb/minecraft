#if OGL_VERSION == 46
#define DATA_LAYOUT \
struct ElementData {\
  float pos_x;\
  float pos_y;\
  float pos_z;\
  float rot_x;\
  float rot_y;\
  float rot_z;\
  float size_x;\
  float size_y;\
  float size_z;\
};\
layout(std430, binding = 0) readonly buffer elementDataBuffer {\
  ElementData elementData[];\
};\
struct QuadData {\
  uint element;\
  uint dir;\
  uint uv_rot;\
  uint uv_x;\
  uint uv_y;\
  uint uv_w;\
  uint uv_h;\
};\
layout(std430, binding = 1) readonly buffer instanceDataBuffer {\
  QuadData instanceData[];\
};
#elif OGL_VERSION == 41
#define DATA_LAYOUT \
layout(location =  0) in uvec2 data_uv_pos;\
layout(location =  1) in uvec2 data_uv_size;\
layout(location =  2) in  uint data_uv_rot;\
layout(location =  3) in  uint data_dir;
#endif


out vec2 texUv;
flat out uint Axis;
out vec3 FragPos;

DATA_LAYOUT

uniform mat4 u_view;
uniform mat4 u_projection;

uniform ivec3 eye_position_int;

const vec2 vertexLookup[4] = vec2[4](
  vec2(0,  0),
  vec2(1,  0),
  vec2(1,  1),
  vec2(0,  1)
);









// deg rad
#define M_PI 3.14159265358979323846
float deg_to_rad(float deg) {
  return deg * (M_PI / 180.0);
}

float rad_to_deg(float rad) {
  return rad * (180.0 / M_PI);
}

mat4 mat3RotToMat4(mat3 rotation) {
  vec3 position = vec3(0.0, 0.0, 0.0);

  return mat4(
    vec4(rotation[0], 0.0),  // First column
    vec4(rotation[1], 0.0),  // Second column
    vec4(rotation[2], 0.0),  // Third column
    vec4(position, 1.0)      // Translation column
  );
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

mat4 rotationMatrixMat4(vec3 r) {
  return mat3RotToMat4(rotationMatrix(r));
}

mat4 translationMatrix(vec3 position) {
  return mat4(
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    position.x, position.y, position.z, 1.0
  );
}





mat4 getFaceOrigRot(uint axis) {
  mat4 orig;
  if (axis == 0) {
      orig = rotationMatrixMat4(vec3(0, deg_to_rad(90), 0));
  } else if (axis == 1) {
      orig = rotationMatrixMat4(vec3(0, deg_to_rad(-90), 0));
  } else if (axis == 2) {
      orig = rotationMatrixMat4(vec3(deg_to_rad(-90), 0, 0));
  } else if (axis == 3) {
      orig = rotationMatrixMat4(vec3(deg_to_rad(90), 0, 0));
  } else if (axis == 4) {
      orig = rotationMatrixMat4(vec3(0, deg_to_rad(180), 0));
  } else {
      orig = rotationMatrixMat4(vec3(0, deg_to_rad(0), 0));
  }
  return orig;
}

mat4 getFaceOrig(uint axis) {
  mat4 orig = mat4(1.0);
  orig = orig * translationMatrix(vec3(0.5, 0.5, 0.5));
  orig = orig * getFaceOrigRot(axis);
  orig = orig * translationMatrix(vec3(-0.5, -0.5, -0.5));
  return orig;
}





vec3 getRotatedElementSize(uint axis, vec3 size) {
  vec3 rotatedSize;
  if (axis == 0) {
      rotatedSize = vec3(size.z, size.y, size.x);
  } else if (axis == 1) {
      rotatedSize = vec3(size.z, size.y, size.x);
  } else if (axis == 2) {
      rotatedSize = vec3(size.x, size.z, size.y);
  } else if (axis == 3) {
      rotatedSize = vec3(size.x, size.z, size.y);
  } else if (axis == 4) {
      rotatedSize = vec3(size.x, size.y, size.z);
  } else {
      rotatedSize = vec3(size.x, size.y, size.z);
  }
  return rotatedSize;
}

vec3 getExpandDir(uint axis, vec2 dir) {
  vec3 rotatedSize;
  if (axis == 0) {
      rotatedSize = vec3(0, dir.y, dir.x);
  } else if (axis == 1) {
      rotatedSize = vec3(0, dir.y, dir.x);
  } else if (axis == 2) {
      rotatedSize = vec3(dir.x, 0, dir.y);
  } else if (axis == 3) {
      rotatedSize = vec3(dir.x, 0, dir.y);
  } else if (axis == 4) {
      rotatedSize = vec3(dir.x, dir.y, 0);
  } else {
      rotatedSize = vec3(dir.x, dir.y, 0);
  }
  return rotatedSize;
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

  #if OGL_VERSION == 46
  QuadData data = instanceData[faceId];
  uint data_element = data.element;
  uint data_uv_rot = data.uv_rot;
  uvec2 data_uv_pos = uvec2(data.uv_x, data.uv_y);
  uvec2 data_uv_size = uvec2(data.uv_w, data.uv_h);
  uint data_dir = data.dir;

  ElementData element_data = elementData[data_element];
  vec3 element_data_pos = vec3(element_data.pos_x, element_data.pos_y, element_data.pos_z);
  vec3 element_data_rot = vec3(element_data.rot_x, element_data.rot_y, element_data.rot_z);
  vec3 element_data_size = vec3(element_data.size_x, element_data.size_y, element_data.size_z);

  #elif OGL_VERSION == 41

  #endif






  vec3 elementSize = element_data_size / 16.0;

  vec2 uvOffset = data_uv_pos / 64.0;
  vec2 uvSize = data_uv_size / 64.0;

  uint axis = data_dir;
  // uint isNegative = axis & 1u;




  // vec3 vertexPos = vec3(mat3RotToMat4(rotationMatrix(element_data_rot)) * getFaceOrig(axis) * vec4(aPos, 0.0, 1.0));
  // vec3 vertexPos = vec3(mat3RotToMat4(rotationMatrix(element_data_rot)) * getFaceOrig(axis) * vec4(element_data_size / 16.0 * 0.5, 1.0) * vec4(aPos, 0.0, 1.0));
  // vec3 vertexPos = vec3(aPos, 0.0);


  // vec2 faceSize;
  // if ((axis & 4u) != 0u) { // 0b100
  //   // z
  //   faceSize = vec2(elementSize.x, elementSize.y);
  // } else if ((axis & 2u) != 0u) { // 0b10
  //   // y
  //   faceSize = vec2(elementSize.x, elementSize.z);
  // } else {
  //   // x
  //   faceSize = vec2(elementSize.z, elementSize.y);
  // }
  // vec3 vertexPos = vec3(aPos * faceSize, 0.0);



  vec2 faceSize;
  if ((axis & 4u) != 0u) { // 0b100
    // z
    faceSize = vec2(elementSize.x, elementSize.y);
  } else if ((axis & 2u) != 0u) { // 0b10
    // y
    faceSize = vec2(elementSize.x, elementSize.z);
  } else {
    // x
    faceSize = vec2(elementSize.z, elementSize.y);
  }

  // vec3 vertexPos = vec3(getFaceOrig(axis) * vec4(aPos * faceSize, 0.0, 1.0));
  // vec3 vertexPos = vec3(getFaceOrig(axis) * translationMatrix(elementSize * 0.5) * vec4(aPos * faceSize, 0.0, 1.0));
  // mat4 origRot = getFaceOrigRot(axis);
  // vec3 vertexPos = vec3(translationMatrix(elementSize * 0.5) * origRot * translationMatrix(elementSize * -0.5) * vec4(aPos * faceSize, 0.0, 1.0));
  vec3 rotatedSize = getRotatedElementSize(axis, elementSize);
  vec3 expandDir = getExpandDir(axis, aPos * faceSize);
  mat4 origRot = getFaceOrigRot(axis);
  // vec3 vertexPos = vec3(getFaceOrig(axis)) + expandDir;
  vec3 vertexPos = vec3(translationMatrix(elementSize * 0.5) * origRot * translationMatrix(rotatedSize * -0.5) * vec4(aPos * faceSize, 0.0, 1.0));





  vertexPos += vec3((data_element % 6) * 2, 10.0, 0.0);
  if (data_element / 6 > 0) {
    // vertexPos.y += (data_element / 6) * 1.0;
    vertexPos += vec3(-0.5 / 2 / 16);
  }

  gl_Position = u_projection * u_view * vec4(vertexPos, 1.0);






  vec2 uvPos = aPos;
  uvPos.x = 1 - uvPos.x;
  uvPos.y = 1 - uvPos.y;

  if (axis == 3) {
    uvPos.y = 1 - uvPos.y;
  }

  texUv = uvOffset + uvPos * uvSize;
  // texUv.x = 1 - texUv.x;
  texUv.y = 1 - texUv.y;






  uint uvRot = data_uv_rot;
  if (uvRot == 1) {
    texUv = vec2(texUv.y, 1.0 - texUv.x);
  } else if (uvRot == 2) {
    texUv = vec2(1.0 - texUv.x, 1.0 - texUv.y);
  } else if (uvRot == 3) {
    texUv = vec2(1.0 - texUv.y, texUv.x);
  }





  // other
  Axis = axis;
  FragPos = vec3(gl_Position);
}