#if GLSL_VERSION_ES == 0
#if GLSL_VERSION == 420
layout (binding = 0) uniform sampler2D inputTexture;

out vec4 output;

void main() {
	output = vec4(texture(inputTexture, gl_FragCoord.xy / render.viewport).xyz, 1);
}
#endif
#else
#if GLSL_VERSION == 300
layout (binding = 0) uniform sampler2D inputTexture;

out vec4 output;

void main() {
	output = vec4(texture(inputTexture, gl_FragCoord.xy / render.viewport).xyz, 1);
}
#endif
#endif
