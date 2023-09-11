#version 460

in vec2 v_texCoord;
in vec3 v_norm;

out vec4 o_fragColor;

uniform sampler2D diffuseTex;

vec3 diffuse(vec3 lightColor, vec3 lightDir){
	return max(dot(v_norm, lightDir), 0) * lightColor;
}

void main(){
	vec4 lightResult = vec4(0.25, 0.25, 0.25, 1);
	lightResult.xyz += diffuse(vec3(1.0, 1.0, 1.0), normalize(vec3(0.5, 1.0, 1.0)));
	o_fragColor = texture(diffuseTex, v_texCoord) * lightResult;
}