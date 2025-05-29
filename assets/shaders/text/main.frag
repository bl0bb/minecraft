in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D tex;

uniform int uColor;

void main() {
    FragColor = texture(tex, TexCoord) * vec4(vec3((uColor >> 0) & 15, (uColor >> 4) & 15, (uColor >> 8) & 15) / 15.0, 1.0);
}