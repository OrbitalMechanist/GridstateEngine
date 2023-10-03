#version 460 core
in vec4 fragPos;

uniform vec3 cubeShadowLightPos;
uniform float cubeShadowDistanceLimit;

void main() {
	gl_FragDepth = length(fragPos.xyz - cubeShadowLightPos) / cubeShadowDistanceLimit;
}