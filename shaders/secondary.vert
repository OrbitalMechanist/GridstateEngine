#version 460

layout(location=0) in vec3 pos;
layout(location=1) in vec3 norm;
layout(location=2) in vec2 texCoord;

out vec2 v_texCoord;

uniform mat4 mvp;

void main(){
	v_texCoord = texCoord;
	gl_Position = mvp * vec4(pos, 1.0);
}