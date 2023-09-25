#version 460 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 norm;
layout(location=2) in vec2 texCoord;

out vec2 v_texCoord;
out vec3 v_norm;
out vec3 v_worldPos;

uniform mat4 mvp;
uniform mat4 modelMat;
uniform mat4 normMat;

void main(){
	v_norm = normalize(mat3(normMat) * norm);
	v_texCoord = texCoord;
	v_worldPos = (modelMat * vec4(pos, 1.0)).xyz;
	gl_Position = mvp * vec4(pos, 1.0);
}
