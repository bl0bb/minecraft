#version 330 core

in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    // Visualize normals as color (for testing)
    FragColor = vec4(abs(Normal), 1.0);
}
