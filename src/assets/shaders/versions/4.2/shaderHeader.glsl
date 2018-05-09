#version 420
#define GLSL_VERSION_420

layout (binding = 1, std140, row_major) uniform RenderUniforms
{
	vec2 viewport;
} render;

layout (binding = 2, std140, row_major) uniform SceneUniforms
{
	vec4 ambientLight;
} scene;

layout (binding = 3, std140, row_major) uniform CameraUniforms
{
	mat4 view;
	mat4 inverseView;
	mat4 projection;
	mat4 inverseProjection;
	vec2 fov;
	float zNear;
	float zFar;
} camera;

layout (binding=4, std140, row_major) uniform ObjectUniforms
{
	mat4 transform;
} object;

layout (binding=5, std140, row_major) uniform MaterialUniforms
{	
	vec2 bumpMapResolution;
	float specularExponent;
} material;

layout (binding = 6, std140, row_major) uniform PointLightUniforms
{
	mat4 transform[6];
	mat4 projection;
	vec4 position;
	vec4 intensity;
	vec2 clipPlanes;
} pointLight;
