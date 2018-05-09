#version 300 es
#define GLSL_VERSION_300
#define GLSL_VERSION_ES

uniform RenderUniforms {
	highp vec2 viewport;
} render;

uniform SceneUniforms {
	highp vec4 ambientLight;
} scene;

uniform CameraUniforms {
	highp mat4 view;
	highp mat4 inverseView;
	highp mat4 projection;
	highp mat4 inverseProjection;
	highp vec2 fov;
	highp float zNear;
	highp float zFar;
} camera;

uniform ObjectUniforms {
	highp mat4 transform;
} object;

uniform MaterialUniforms {
	highp vec2 bumpMapResolution;
	highp float specularExponent;
} material;

uniform PointLightUniforms {
	highp mat4 transform[6];
	highp mat4 projection;
	highp vec4 position;
	highp vec4 intensity;
	highp vec2 clipPlanes;
} pointLight;

