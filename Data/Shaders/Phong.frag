#version 330 core
    
layout(location = 0) out vec4 out_Color;

uniform vec3 color_Kd;
uniform vec3 color_Ks;
uniform float color_Ns;
uniform float color_Alpha;
uniform sampler2D texture_sampler;
uniform bool useTexture;
uniform bool lightSensitive;
   
uniform vec3 lightDir;
uniform vec3 lightIntensity;
uniform vec3 lightColor;

uniform vec3 worldeye_pos;

uniform sampler2D shadowMap;

in vec2 textCoords;
in vec3 worldPos;
in vec3 sh_normal;
in vec4 pos_LightSpace;


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = 0.0f;
	// Pour adoucir l'ombre, on fait la moyenne des pixels envirronants
	vec2 textelSize = 1.0f/ textureSize(shadowMap, 0);
	for(int x=-1; x <= 1; x++){
		for(int y=-1; y <= 1; y++){
		    float closestDepth = texture(shadowMap, projCoords.xy+vec2(x, y)*textelSize).r; 
			shadow += currentDepth-0.0005f > closestDepth ? 1.0 : 0.0;
		}
	}
	
	shadow /= 9;
	
	if(projCoords.z > 1.0f){
		 shadow = 0.0f;
	}

    return shadow;
}


void main(void){
	vec4 tex;
	if(useTexture == true){ tex = texture(texture_sampler,textCoords); }else{ tex = vec4(1.0, 1.0, 1.0, 1.0); }
	if(!lightSensitive){
		//float linear_depth = (2.0 * 0.1f) / (200.0f + 0.1f - tex.x * (200.0f - 0.1f));
       // out_Color = vec4(linear_depth, linear_depth, linear_depth, 1.0f);
		out_Color = tex*vec4(color_Kd, color_Alpha);
		return;
	}

    vec3 pointToLight =normalize(-lightDir);
    vec3 normal=normalize(sh_normal);
    vec3 eye= normalize(worldeye_pos);
    vec3 reflectDir= normalize(reflect(pointToLight,normal));
    float lightFactor = max(0.0, dot(normal, pointToLight));
    float specularFactor = pow(max(0.0, dot(reflectDir,eye)), color_Ns);
	
    vec4 colorambient=tex*vec4(color_Kd*lightColor,color_Alpha);
    vec4 colorlight=tex*vec4((lightFactor*color_Kd + specularFactor*color_Ks)*lightIntensity,color_Alpha);
	
	float shadow = ShadowCalculation(pos_LightSpace);
	colorlight = vec4(colorlight.rgb*(1.0f-shadow), colorlight.a);
	out_Color = colorlight+colorambient;
}