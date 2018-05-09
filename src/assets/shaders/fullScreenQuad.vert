#ifndef GLSL_VERSION_ES
#ifdef GLSL_VERSION_420
out vec3 eyeSpaceRay;

const vec2 halfSizeNearPlane = tan(camera.fov / 2);
const vec2 corners[4] = { vec2(-1, -1), vec2( 1, -1), vec2( -1, 1), vec2( 1, 1 )};

void main() {
	eyeSpaceRay = vec3(halfSizeNearPlane * corners[gl_VertexID].xy, -1);
	gl_Position = vec4(corners[gl_VertexID], -1.0, 1.0);
}
#endif
#else
#ifdef GLSL_VERSION_300
out vec3 eyeSpaceRay;

void main() {
	vec2 corners[4];
	corners[0] = vec2(-1,-1);
	corners[1] = vec2(1,-1);
	corners[2] = vec2(-1,1);
	corners[3] = vec2(1,1);
	vec2 fov = camera.fov;
	vec2 corner = corners[gl_VertexID];
	eyeSpaceRay = vec3(tan(fov)*corner,-1);
	gl_Position = vec4(corner,-1.0,1.0);
}
#endif
#endif
