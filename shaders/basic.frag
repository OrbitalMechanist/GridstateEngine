#version 460

in vec2 v_texCoord;
in vec3 v_norm;
in vec3 v_worldPos;

out vec4 o_fragColor;

struct Light{
	//0 for off, 1 for directional, 2 for point, 3 for spot
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
};

uniform sampler2D diffuseTex;

uniform vec3 ambientLight;

#define NUM_LIGHTS 8
uniform Light lights[NUM_LIGHTS];

vec3 diffuse(float intensity, vec3 lightColor, vec3 lightDir){
	return max(dot(v_norm, lightDir), 0) * lightColor * intensity;
}

vec3 calcLightEffect(Light light){
	if((light.type == 0) || (length(light.color) == 0.0) || (light.intensity == 0.0)){
		return vec3(0, 0, 0);
	}

	vec3 result = vec3(0, 0, 0);
	
	//Directional
	if(light.type == 1){
		if(length(light.direction) == 0){
			return result;
		}
		vec3 toLight = normalize(-light.direction);
		result = diffuse(light.intensity, light.color, toLight);
	} 
	//Point
	else if(light.type == 2){
		vec3 toLight = normalize(light.position - v_worldPos);
		result = diffuse(light.intensity, light.color, toLight);
	}
	
	return result;
}

void main(){
	vec4 lightResult = vec4(ambientLight, 1);

	for(int i = 0; i < NUM_LIGHTS; i++){
		lightResult.xyz += calcLightEffect(lights[i]);
	}

	o_fragColor = texture(diffuseTex, v_texCoord) * lightResult;
}