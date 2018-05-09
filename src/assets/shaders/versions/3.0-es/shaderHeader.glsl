#version 300 es
#define GLSL_VERSION_300
#define GLSL_VERSION_ES
#extension GL_ARB_shading_language_420pack: require

layout (std140, row_major) uniform RenderUniforms
{
	vec2 viewport;
} render;

layout (std140, row_major) uniform SceneUniforms
{
	vec4 ambientLight;
} scene;

layout (std140, row_major) uniform CameraUniforms
{
	mat4 view;
	mat4 inverseView;
	mat4 projection;
	mat4 inverseProjection;
	vec2 fov;
	float zNear;
	float zFar;
} camera;

layout (std140, row_major) uniform ObjectUniforms
{
	mat4 transform;
} object;

layout (std140, row_major) uniform MaterialUniforms
{	
	vec2 bumpMapResolution;
	float specularExponent;
} material;

layout (std140, row_major) uniform PointLightUniforms
{
	mat4 transform[6];
	mat4 projection;
	vec4 position;
	vec4 intensity;
	vec2 clipPlanes;
} pointLight;
