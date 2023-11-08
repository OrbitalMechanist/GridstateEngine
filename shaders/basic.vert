#version 460 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 norm;
layout(location=2) in vec2 texCoord;
/*
//4 is a common bone-per-vertex limit and is plenty for our purposes.
//This bit of trickery makes my life a little easier when
//dealing with layouts.
layout(location=3) in uvec4 boneIndices;
layout(location=4) in vec4 boneWeights;
*/

out vec2 v_texCoord;
out vec3 v_norm;
out vec3 v_worldPos;

uniform mat4 mvp;
uniform mat4 modelMat;
uniform mat4 normMat;

/*
//This isn't going to work because each of these matrices will be its own uniform,
//and the number of uniforms is limited by implementation/hardware.
//SSBOs should probably be used instead.
#define MAX_BONES 256
uniform mat4 boneTransforms[MAX_BONES];
*/
void main(){
/*
	mat4 boneSum = boneTransforms[boneIndices.x] * boneWeights.x 
		+ boneTransforms[boneIndices.y] * boneWeights.y
		+ boneTransforms[boneIndices.z] * boneWeights.z
		+ boneTransforms[boneIndices.w] * boneWeights.w;
*/
	v_norm = normalize(mat3(normMat) * norm);
	v_texCoord = texCoord;
	v_worldPos = (modelMat * vec4(pos, 1.0)).xyz;
	gl_Position = mvp * vec4(pos, 1.0);
}
