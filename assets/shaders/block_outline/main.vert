layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 uView;
uniform mat4 uProjection;

uniform vec3 uPos;

void main() {
  gl_Position = uProjection * uView * vec4(uPos + aPos, 1.0);
}