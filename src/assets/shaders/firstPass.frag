layout (binding = 0) uniform sampler2D diffuseSampler;
layout (binding = 1) uniform sampler2D specularSampler;
layout (binding = 2) uniform sampler2D bumpSampler;
layout (binding = 3) uniform sampler2D emissivitySampler;

in vec4 eyeSpacePosition;
in vec4 eyeSpaceNormal;
in vec2 interpolatedTexCoord;
in mat4 UVtoWorld;

layout (location = 0) out vec4 diffuseReflectance;
layout (location = 1) out vec4 specularReflectance;
layout (location = 2) out vec4 normal;
layout (location = 3) out vec4 initialColour;

const float bumpMapFactor = 50;

vec4 applyBumpMap();

void main() {
	diffuseReflectance = texture(diffuseSampler, interpolatedTexCoord);
	specularReflectance = vec4(texture(specularSampler, interpolatedTexCoord).xyz, material.specularExponent / 1000);
	normal = applyBumpMap() * 0.5 + 0.5;
	initialColour = diffuseReflectance * scene.ambientLight;
	initialColour += texture(emissivitySampler, interpolatedTexCoord);
	gl_FragDepth = length(eyeSpacePosition.xyz) / camera.zFar;
}

vec4 applyBumpMap() {
	float perturbationU = length(texture(bumpSampler, interpolatedTexCoord + vec2(-1 / material.bumpMapResolution.x, 0)).rgb) - length(texture(bumpSampler, interpolatedTexCoord).rgb);
	float perturbationV = length(texture(bumpSampler, interpolatedTexCoord + vec2(0, -1 / material.bumpMapResolution.y)).rgb) - length(texture(bumpSampler, interpolatedTexCoord).rgb);	
	vec4 perturbation = normalize(vec4(perturbationU, perturbationV, 1 / bumpMapFactor, 0)) * UVtoWorld;
	return normalize(eyeSpaceNormal + perturbation);
}