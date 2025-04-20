#version 330 core

in vec2 TexCoord;
in float waveHeight;

out vec4 FragColor;

uniform sampler2D waterTexture;
uniform vec3 lightColor;

void main()
{
    vec3 baseColor = texture(waterTexture, TexCoord).rgb;

    // Simple color modulation based on wave height
    float intensity = 0.5 + waveHeight;
    vec3 waterColor = mix(baseColor, lightColor, intensity);

    FragColor = vec4(waterColor, 1.0);
}
