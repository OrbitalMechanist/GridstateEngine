#version 460

in vec2 v_texCoord;

out vec4 o_fragColor;

uniform sampler2D diffuseTex;

void main(){
	o_fragColor = texture(diffuseTex, v_texCoord);
}