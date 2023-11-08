#version 460 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 norm;
layout(location=2) in vec3 texCoord;
layout(location=3) in uvec4 boneIndices;
layout(location=4) in vec4 boneWeights;

uniform mat4 mvp; //this is just the model matrix when being used with shadow cubes

void main(){
	gl_Position = mvp * vec4(pos, 1.0);
}