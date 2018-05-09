#if GLSL_VERSION_ES == 0
#if GLSL_VERSION == 420
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 binormal;

out vec4 eyeSpaceNormal;
out vec4 eyeSpacePosition;
out vec2 interpolatedTexCoord;
out mat4 UVtoWorld;

void main() {
	mat4 modelView = camera.view * object.transform;
	mat4 fullMatrix = camera.projection * modelView;

    eyeSpaceNormal = normalize(modelView * vec4(normal, 0.0));
	interpolatedTexCoord = texCoord;
	eyeSpacePosition = modelView * vec4(pos, 1.0);
	gl_Position = fullMatrix * vec4(pos, 1.0);
	
 	vec4 transformedTangent = fullMatrix * vec4(tangent, 0.0);
 	vec4 transformedBinormal = fullMatrix * vec4(binormal, 0.0);

	UVtoWorld[0] = vec4(transformedTangent.x, transformedBinormal.x, eyeSpaceNormal.x, 0);
	UVtoWorld[1] = vec4(transformedTangent.y, transformedBinormal.y, eyeSpaceNormal.y, 0);
	UVtoWorld[2] = vec4(transformedTangent.z, transformedBinormal.z, eyeSpaceNormal.z, 0);
	UVtoWorld[2] = vec4(0, 0, 0, 1);
}
#endif
#else
#if GLSL_VERSION == 300
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 binormal;

out vec4 eyeSpaceNormal;
out vec4 eyeSpacePosition;
out vec2 interpolatedTexCoord;
out mat4 UVtoWorld;

void main() {
	mat4 modelView = camera.view * object.transform;
	mat4 fullMatrix = camera.projection * modelView;

    eyeSpaceNormal = normalize(modelView * vec4(normal, 0.0));
	interpolatedTexCoord = texCoord;
	eyeSpacePosition = modelView * vec4(pos, 1.0);
	gl_Position = fullMatrix * vec4(pos, 1.0);
	
 	vec4 transformedTangent = fullMatrix * vec4(tangent, 0.0);
 	vec4 transformedBinormal = fullMatrix * vec4(binormal, 0.0);

	UVtoWorld[0] = vec4(transformedTangent.x, transformedBinormal.x, eyeSpaceNormal.x, 0);
	UVtoWorld[1] = vec4(transformedTangent.y, transformedBinormal.y, eyeSpaceNormal.y, 0);
	UVtoWorld[2] = vec4(transformedTangent.z, transformedBinormal.z, eyeSpaceNormal.z, 0);
	UVtoWorld[2] = vec4(0, 0, 0, 1);
}
#endif
#endif
