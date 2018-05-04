layout (binding = 0) uniform sampler2D inputTexture;

out vec4 output;

void main() {
	output = vec4(texture(inputTexture, gl_FragCoord.xy / render.viewport).xyz, 1);
}