#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out float waveHeight;

uniform float time;

void main()
{
    vec3 pos = aPos;

    // Simple sine wave effect for water surface
    pos.y += sin(pos.x * 4.0 + time) * 0.1;
    pos.y += cos(pos.z * 4.0 + time) * 0.1;

    waveHeight = pos.y;
    TexCoord = aTexCoord;
    gl_Position = vec4(pos, 1.0);
}
