#version 460 core

in vec2 v_texCoord;

out vec4 o_fragColor;

void main(){
	o_fragColor = vec4(v_texCoord.x, v_texCoord.y, 0.0, 1.0);
}