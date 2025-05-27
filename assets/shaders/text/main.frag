in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D fontTexture;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // texture(fontTexture, TexCoord);
}
