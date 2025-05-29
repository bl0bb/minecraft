layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

uniform vec2 uOffset;
uniform vec2 uScale;

uniform mat4 uProjection;

uniform vec2 uTexOffset;
uniform vec2 uTexSize;

out vec2 TexCoord;

void main() {
    vec2 pos = aPos * uScale + uOffset;

    gl_Position = uProjection * vec4(pos.xy, 0.0, 1.0);
    TexCoord = uTexOffset + aTex * uTexSize;
}