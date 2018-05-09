#ifndef GLSL_VERSION_ES
#ifdef GLSL_VERSION_420
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 binormal;

out vec3 normal;

void main() {
	normal = inNormal;
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
#endif
#else
#ifdef GLSL_VERSION_300
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 binormal;

out vec3 normal;

void main() {
	normal = inNormal;
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
#endif
#endif
