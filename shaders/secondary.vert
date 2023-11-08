#version 460 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 norm;
layout(location=2) in vec2 texCoord;
layout(location=3) in uvec4 boneIndices;
layout(location=4) in vec4 boneWeights;

out vec2 v_texCoord;
out vec3 v_norm;

uniform mat4 mvp;

void main(){
	v_texCoord = texCoord;
	v_norm = norm;
	gl_Position = mvp * vec4(pos, 1.0);
}