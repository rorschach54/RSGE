#if GLSL_VERSION_ES == 0
#if GLSL_VERSION == 420
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

in vec3 normal[];

out vec4 eyeSpacePosition;
out vec4 eyeSpaceNormal;

void main() {
	for(int face = 0;face < 6;face++) {
		gl_Layer = face;
		for(int i = 0;i < 3;i++) {
			eyeSpacePosition = pointLight.transform[face] * object.transform * gl_in[i].gl_Position;
			eyeSpaceNormal = pointLight.transform[face] * object.transform * vec4(normal[i], 0);
			gl_Position = pointLight.projection * eyeSpacePosition;
			EmitVertex();
		}
        EndPrimitive();
	}
}
#endif
#else
#if GLSL_VERSION == 300
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

in vec3 normal[];

out vec4 eyeSpacePosition;
out vec4 eyeSpaceNormal;

void main() {
	for(int face = 0;face < 6;face++) {
		gl_Layer = face;
		for(int i = 0;i < 3;i++) {
			eyeSpacePosition = pointLight.transform[face] * object.transform * gl_in[i].gl_Position;
			eyeSpaceNormal = pointLight.transform[face] * object.transform * vec4(normal[i], 0);
			gl_Position = pointLight.projection * eyeSpacePosition;
			EmitVertex();
		}
        EndPrimitive();
	}
}
#endif
#endif
