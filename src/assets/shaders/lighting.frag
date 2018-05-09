#if GLSL_VERSION_ES == 0
#if GLSL_VERSION == 420
layout (binding = 0) uniform sampler2D diffuseBuffer;	// R, G, B, unused
layout (binding = 1) uniform sampler2D specularBuffer;	// R, G, B, exponent / 1000
layout (binding = 2) uniform sampler2D normalBuffer;	// R, G, B, unused. Also values have to be converted from 0 - 1 to -1 - 1
layout (binding = 3) uniform sampler2D depthBuffer;
layout (binding = 4) uniform samplerCube shadowMapBuffer;

in vec3 eyeSpaceRay;

out vec4 fragmentColour;

void applyDiffuse();
void applySpecular();
void applyEmissivity();
void applyReflection();
vec4 depthToEyeSpace(mat4 projection, float depth);
float sampleLightMapDepth(vec3 scenePoint, vec3 lightPoint);
float lightAmount();

const float lightDropoffFactor = 1.05;
const float pi = 3.14159265358979323846f;

vec4 diffuse;
vec4 specular;
vec4 eyeSpaceNormal;
vec2 screenPos;
vec4 eyeSpacePosition;
vec4 eyeSpaceLightPosition;
vec4 eyeSpaceLightVector;
float lightDistance;
float attenuation;

void main() {
	screenPos = gl_FragCoord.xy / render.viewport;
	diffuse = texture(diffuseBuffer, screenPos);	
	specular = texture(specularBuffer, screenPos);	
	specular.w *= 1000;
	eyeSpaceNormal = vec4(normalize(texture(normalBuffer, screenPos).xyz * 2 - 1.0), 0); 
	eyeSpacePosition = depthToEyeSpace(camera.projection, texture(depthBuffer, screenPos).x);
	eyeSpaceLightPosition = camera.view * pointLight.position;
	eyeSpaceLightVector = vec4(normalize(eyeSpacePosition.xyz - eyeSpaceLightPosition.xyz), 0);
	lightDistance = distance(eyeSpaceLightPosition, eyeSpacePosition);
	attenuation = min(1 / pow(lightDistance, lightDropoffFactor), 1);
	
	applyDiffuse();
	applySpecular();
	fragmentColour = min(max(fragmentColour, 0), 1);
	fragmentColour *= lightAmount();
}

void applyDiffuse() {
	float dotProduct = max(dot(eyeSpaceNormal.xyz, -eyeSpaceLightVector.xyz), 0);
	fragmentColour = max(vec4(diffuse * dotProduct * pointLight.intensity), 0) * attenuation;
}

void applySpecular() {
	//Blinn-Phong
	vec3 halfwayVector = normalize(-eyeSpaceLightVector.xyz - normalize(eyeSpacePosition.xyz));
	float dotProduct = max(dot(halfwayVector, eyeSpaceNormal.xyz), 0);
	fragmentColour += vec4(specular.xyz, 1) * pow(dotProduct * pointLight.intensity, vec4(specular.www, 1)) * attenuation;
}
vec4 depthToEyeSpace(mat4 projection, float depth) {
	return vec4(normalize(eyeSpaceRay.xyz) * depth * camera.zFar, 1);
}

//https://stackoverflow.com/a/4275343/626796
float PRNG(vec2 seed) {
	return fract(sin(dot(seed.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float sampleLightMapDepth(vec3 scenePoint, vec3 lightPoint) {
	vec3 shadowSamplerRay = normalize((camera.inverseView * vec4((lightPoint - scenePoint), 0)).xyz);
	shadowSamplerRay.x *= -1;
	float mapDepth = texture(shadowMapBuffer, shadowSamplerRay).x;	
	return mapDepth;
}

float lightAmount() {
	float sampleTotal = 0;
	const float sampleRange = 0.05f;

	//This is a sphere
	const vec3 sampleList[13] = {
		vec3(0, 0, 0),
		vec3(0.185011744, 0.000050000, 0.982736290),
		vec3(0.543722034, 0.354457319, 0.760740697),
		vec3(-0.0940286964, 0.341989040, 0.934987724),
		vec3(-0.666421711, 0.478428960, 0.571828604),
		vec3(-0.251662046, 0.915312707, -0.314434320),
		vec3(0.830358803, 0.541318417, 0.132206663),
		vec3(0.188106239, 0.679270804, 0.709370971),
		vec3(-0.304940492, 0.736509264, 0.603792489),
		vec3(-0.381129295, 0.920524716, 0.0858761966),
		vec3(0.264770985, 0.956115067, -0.125460610),
		vec3(0.539285064, 0.766062260, 0.349743187),
		vec3(0.0589527227, 0.947874784, 0.313142180)
	};
		
	for(int i = 0;i < 13;i++) {
		float mapDepth = sampleLightMapDepth(eyeSpacePosition.xyz + sampleList[i] * sampleRange, eyeSpaceLightPosition.xyz);
        float realDepth = lightDistance / pointLight.clipPlanes.y;
        sampleTotal += (realDepth >= mapDepth ? 0 : 1);
    }

	return (sampleTotal / 13);
}
#endif
#else
#if GLSL_VERSION == 300
layout (binding = 0) uniform sampler2D diffuseBuffer;	// R, G, B, unused
layout (binding = 1) uniform sampler2D specularBuffer;	// R, G, B, exponent / 1000
layout (binding = 2) uniform sampler2D normalBuffer;	// R, G, B, unused. Also values have to be converted from 0 - 1 to -1 - 1
layout (binding = 3) uniform sampler2D depthBuffer;
layout (binding = 4) uniform samplerCube shadowMapBuffer;

in vec3 eyeSpaceRay;

out vec4 fragmentColour;

void applyDiffuse();
void applySpecular();
void applyEmissivity();
void applyReflection();
vec4 depthToEyeSpace(mat4 projection, float depth);
float sampleLightMapDepth(vec3 scenePoint, vec3 lightPoint);
float lightAmount();

const float lightDropoffFactor = 1.05;
const float pi = 3.14159265358979323846f;

vec4 diffuse;
vec4 specular;
vec4 eyeSpaceNormal;
vec2 screenPos;
vec4 eyeSpacePosition;
vec4 eyeSpaceLightPosition;
vec4 eyeSpaceLightVector;
float lightDistance;
float attenuation;

void main() {
	screenPos = gl_FragCoord.xy / render.viewport;
	diffuse = texture(diffuseBuffer, screenPos);	
	specular = texture(specularBuffer, screenPos);	
	specular.w *= 1000;
	eyeSpaceNormal = vec4(normalize(texture(normalBuffer, screenPos).xyz * 2 - 1.0), 0); 
	eyeSpacePosition = depthToEyeSpace(camera.projection, texture(depthBuffer, screenPos).x);
	eyeSpaceLightPosition = camera.view * pointLight.position;
	eyeSpaceLightVector = vec4(normalize(eyeSpacePosition.xyz - eyeSpaceLightPosition.xyz), 0);
	lightDistance = distance(eyeSpaceLightPosition, eyeSpacePosition);
	attenuation = min(1 / pow(lightDistance, lightDropoffFactor), 1);
	
	applyDiffuse();
	applySpecular();
	fragmentColour = min(max(fragmentColour, 0), 1);
	fragmentColour *= lightAmount();
}

void applyDiffuse() {
	float dotProduct = max(dot(eyeSpaceNormal.xyz, -eyeSpaceLightVector.xyz), 0);
	fragmentColour = max(vec4(diffuse * dotProduct * pointLight.intensity), 0) * attenuation;
}

void applySpecular() {
	//Blinn-Phong
	vec3 halfwayVector = normalize(-eyeSpaceLightVector.xyz - normalize(eyeSpacePosition.xyz));
	float dotProduct = max(dot(halfwayVector, eyeSpaceNormal.xyz), 0);
	fragmentColour += vec4(specular.xyz, 1) * pow(dotProduct * pointLight.intensity, vec4(specular.www, 1)) * attenuation;
}
vec4 depthToEyeSpace(mat4 projection, float depth) {
	return vec4(normalize(eyeSpaceRay.xyz) * depth * camera.zFar, 1);
}

//https://stackoverflow.com/a/4275343/626796
float PRNG(vec2 seed) {
	return fract(sin(dot(seed.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float sampleLightMapDepth(vec3 scenePoint, vec3 lightPoint) {
	vec3 shadowSamplerRay = normalize((camera.inverseView * vec4((lightPoint - scenePoint), 0)).xyz);
	shadowSamplerRay.x *= -1;
	float mapDepth = texture(shadowMapBuffer, shadowSamplerRay).x;	
	return mapDepth;
}

float lightAmount() {
	float sampleTotal = 0;
	const float sampleRange = 0.05f;

	//This is a sphere
	const vec3 sampleList[13] = {
		vec3(0, 0, 0),
		vec3(0.185011744, 0.000050000, 0.982736290),
		vec3(0.543722034, 0.354457319, 0.760740697),
		vec3(-0.0940286964, 0.341989040, 0.934987724),
		vec3(-0.666421711, 0.478428960, 0.571828604),
		vec3(-0.251662046, 0.915312707, -0.314434320),
		vec3(0.830358803, 0.541318417, 0.132206663),
		vec3(0.188106239, 0.679270804, 0.709370971),
		vec3(-0.304940492, 0.736509264, 0.603792489),
		vec3(-0.381129295, 0.920524716, 0.0858761966),
		vec3(0.264770985, 0.956115067, -0.125460610),
		vec3(0.539285064, 0.766062260, 0.349743187),
		vec3(0.0589527227, 0.947874784, 0.313142180)
	};
		
	for(int i = 0;i < 13;i++) {
		float mapDepth = sampleLightMapDepth(eyeSpacePosition.xyz + sampleList[i] * sampleRange, eyeSpaceLightPosition.xyz);
        float realDepth = lightDistance / pointLight.clipPlanes.y;
        sampleTotal += (realDepth >= mapDepth ? 0 : 1);
    }

	return (sampleTotal / 13);
}
#endif
