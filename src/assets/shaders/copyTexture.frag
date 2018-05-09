#ifndef GLSL_VERSION_ES
#ifdef GLSL_VERSION_420
layout (binding = 0) uniform sampler2D inputTexture;

out vec4 output;

void main() {
	output = vec4(texture(inputTexture, gl_FragCoord.xy / render.viewport).xyz, 1);
}
#endif
#else
#ifdef GLSL_VERSION_300
precision mediump float;
precision mediump int;
uniform sampler2D inputTexture;

out vec4 outputPos;

void main() {
	outputPos = vec4(texture(inputTexture, gl_FragCoord.xy / render.viewport).xyz, 1);
}
#endif
#endif
