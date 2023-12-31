#version 460 core

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
uniform samplerCube depthMap;

uniform vec3 ambientLight;
uniform vec3 viewPos;

#define NUM_LIGHTS 8
uniform Light lights[NUM_LIGHTS];
uniform sampler2D shadowMaps[NUM_LIGHTS];
uniform samplerCube shadowCubemaps[NUM_LIGHTS];
uniform mat4 lightSpaceMatrices[NUM_LIGHTS];

struct Material{
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
uniform Material material;

vec3 diffuseScene(float intensity, vec3 lightColor, vec3 lightDir){
	return max(dot(v_norm, lightDir), 0) * lightColor * material.diffuse * intensity;
}

float calcShadowFactor(int lightIndex){
	 float shadow = 0.0;
    Light light = lights[lightIndex];

    if (light.type == 1 || light.type == 3) {
        mat4 lightSpaceMatrix = lightSpaceMatrices[lightIndex];
        vec3 toLight;
        if (light.type == 3) {
            toLight = light.position - v_worldPos.xyz;
        } else {
            toLight = -light.direction;
        }

        float shadowBias = max(0.005 * (1.0 - dot(v_norm, toLight)), 0.01);

        vec4 lightSpacePos = lightSpaceMatrix * vec4(v_worldPos, 1.0);
        vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
        projCoords = projCoords * 0.5 + 0.5;
        float shadowDepth = texture(shadowMaps[lightIndex], projCoords.xy).r;

         float thisFragmentRelativeDepth = projCoords.z;

        vec2 texelSize = 1.0 / textureSize(shadowMaps[lightIndex], 0);
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                float pcfDepth = texture(shadowMaps[lightIndex], projCoords.xy + vec2(x, y) * texelSize).r;

                shadow += thisFragmentRelativeDepth - shadowBias > pcfDepth ? 0.0 : 1.0;
            }
        }
        shadow /= 9.0;

        shadow = clamp(shadow, 0.0, 1.0);
    }
    
	if (light.type == 2) {
		vec3 toLight = light.position - v_worldPos.xyz;

		//sample the cubemap on a point along the line from the light to this fragment
		vec3 pointInCube = -toLight; 

		float shadowBias = max(0.05 * (1.0 - dot(v_norm, toLight)), 0.08);
	
		float shadowDepth = texture(shadowCubemaps[lightIndex], pointInCube).r;
		if(light.distanceLimit > 0){ 
			shadowDepth *= light.distanceLimit; 
		} else {
			shadowDepth *= 100;
		}


		float thisFragmentRelativeDepth = length(pointInCube);
		
		if(thisFragmentRelativeDepth - shadowBias > shadowDepth){
			return 0;
		}
		shadow = 1;
	}
	return shadow;
}

vec3 calcSpecular(float power, vec3 lightColor, vec3 toLight){
	//This is supposed to be a Phong model, but apparently for whatever reason
	//I get a Gouraud interpolation of the normals so it looks terrible.
	//I can't tell why this is happening right now.

    vec3 viewDir = normalize(viewPos - v_worldPos);
	vec3 halfwayDir = normalize(viewDir + toLight);

	//behaviour when raising to power of 0 is undefined.
	//AMD does the reasonable thing and outputs 1; everything else just breaks.
    return pow(max(0.0f, dot(halfwayDir, v_norm)), material.shininess == 0 ? 1 : material.shininess) 
		* material.specular * lightColor * power;
}


vec3 calcLightEffect(int lightIndex){
Light light = lights[lightIndex];
	if((light.type == 0) || (length(light.color) == 0.0) || (light.intensity == 0.0)){
		return vec3(0, 0, 0);
	}

	float shadowFactor = calcShadowFactor(lightIndex);
	if(shadowFactor == 0) {
		return vec3(0, 0, 0);
	}

	vec3 result = vec3(0, 0, 0);
	
	//Directional
	if(light.type == 1){
		if(length(light.direction) == 0){
			return result;
		}
		vec3 toLight = normalize(-light.direction);
		result = diffuseScene(light.intensity, light.color, toLight);
		//result += calcSpecular(1.0f, light.color, toLight);
	} 
	//Point
	else if(light.type == 2){
		vec3 toLight = light.position - v_worldPos.xyz;
		float dist = length(toLight);
		if((light.distanceLimit >= 0.0 && dist > light.distanceLimit) 
			|| (light.attenuationMax > 0.0 && dist > light.attenuationMax)){
			return result;
		}
		result = diffuseScene(light.intensity, light.color, normalize(toLight));
		//result += calcSpecular(1.0f, light.color, toLight);

		if(light.attenuationMax >= 0.0){
			result *= 1.0f - dist/light.attenuationMax;
		}
	} 
	//Spot
	else if(light.type == 3){
		vec3 toLight = light.position - v_worldPos.xyz;
		float dist = length(toLight);
		if((light.distanceLimit >= 0.0 && dist > light.distanceLimit)
			|| (light.attenuationMax > 0.0 && dist > light.attenuationMax)){
			return result;
		}
		if(acos(dot(normalize(toLight), normalize(-light.direction))) * 2 > light.angle){
			return result;
		}
		result = diffuseScene(light.intensity, light.color, normalize(toLight));
		//result += calcSpecular(1.0f, light.color, toLight);

		if(light.attenuationMax > 0.0){
			result *= 1.0f - dist/light.attenuationMax;
		}
	}
	
	return result * shadowFactor;
}

void main(){
	vec4 lightResult = vec4(ambientLight, 1);

	for(int i = 0; i < NUM_LIGHTS; i++){
		lightResult.xyz += calcLightEffect(i);
	}
	o_fragColor = texture(diffuseTex, v_texCoord) * lightResult; //vec4(v_norm, 1);//
}