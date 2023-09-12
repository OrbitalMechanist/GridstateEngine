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
	//Spotlights only - arc of the light, in radians.
	float angle;
	//Distance at which spot and point lights no longer get calculated. Set to negative for unlimited range.
	float distanceLimit;
	//Distance at which spot and point lights' power reaches zero. Set to negative to disable attenuation.
	float attenuationMax;

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
		vec3 toLight = light.position - v_worldPos;
		float dist = length(toLight);
		if((light.distanceLimit >= 0.0 && dist > light.distanceLimit) 
			|| (light.attenuationMax > 0.0 && dist > light.attenuationMax)){
			return result;
		}
		result = diffuse(light.intensity, light.color, normalize(toLight));
		if(light.attenuationMax >= 0.0){
			result *= 1.0f - dist/light.attenuationMax;
		}
	} 
	//Spot
	else if(light.type == 3){
		vec3 toLight = light.position - v_worldPos;
		float dist = length(toLight);
		if((light.distanceLimit >= 0.0 && dist > light.distanceLimit)
			|| (light.attenuationMax > 0.0 && dist > light.attenuationMax)){
			return result;
		}
		if(acos(dot(normalize(toLight), normalize(-light.direction))) > light.angle){
			return result;
		}
		result = diffuse(light.intensity, light.color, normalize(toLight));
		if(light.attenuationMax > 0.0){
			result *= 1.0f - dist/light.attenuationMax;
		}
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