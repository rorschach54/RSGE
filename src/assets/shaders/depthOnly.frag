in vec4 eyeSpacePosition;
in vec4 eyeSpaceNormal;

void main() {
	gl_FragDepth = (length(eyeSpacePosition.xyz) + 0.05) / pointLight.clipPlanes.y;
}