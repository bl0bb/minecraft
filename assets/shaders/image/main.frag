in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
	vec4 texResult = texture(tex, TexCoord);

	if (texResult.a == 0) {
        discard;
    }

	FragColor = texResult;
}