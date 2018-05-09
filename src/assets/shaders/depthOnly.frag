#ifndef GLSL_VERSION_ES
#ifdef GLSL_VERSION_420
in vec4 eyeSpacePosition;
in vec4 eyeSpaceNormal;

void main() {
	gl_FragDepth = (length(eyeSpacePosition.xyz) + 0.05) / pointLight.clipPlanes.y;
}
#endif
#else
#ifdef GLSL_VERSION_300
in vec4 eyeSpacePosition;
in vec4 eyeSpaceNormal;

void main() {
	gl_FragDepth = (length(eyeSpacePosition.xyz) + 0.05) / pointLight.clipPlanes.y;
}
#endif
#endif
