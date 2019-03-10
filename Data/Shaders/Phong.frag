#version 330 core
    
#define LIGHT_SUN 0
#define LIGHT_SPOT 1
#define MAX_LIGHTS 2

layout(location = 0) out vec4 out_Color;

uniform vec3 sceneAmbiantColor;
uniform vec3 color_Kd;
uniform vec3 color_Ks;
uniform float color_Ns;
uniform float color_Alpha;
uniform sampler2D texture_sampler;
uniform bool useTexture;
uniform bool lightSensitive;
uniform bool useNormalMap = false;
uniform sampler2D normalMap;
   
struct Light{
	vec3 pos;
	vec3 dir;
	vec3 intensity;
	vec3 color;
	int type;
	float angle;
	sampler2D shadowMap;
};

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform vec3 worldeye_pos;

in vec2 textCoords;
in vec3 worldPos;
in vec3 sh_normal;
in vec4 lightSpacePos[MAX_LIGHTS];
in mat3 TBN_matrix;



float ShadowCalculation(float bias, int lightIndex){
    // perform perspective divide
	vec3 projCoords = lightSpacePos[lightIndex].xyz / lightSpacePos[lightIndex].w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = 0.0f;
	// Pour adoucir l'ombre, on fait la moyenne des pixels envirronants
	vec2 textelSize = 1.0f/ textureSize(lights[lightIndex].shadowMap, 0);
	for(int x=-1; x <= 1; x++){
		for(int y=-1; y <= 1; y++){
		    float closestDepth = texture(lights[lightIndex].shadowMap, projCoords.xy+vec2(x, y)*textelSize).r; 
			shadow += ((currentDepth-bias) > closestDepth ? 1.0 : 0.0);
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
	vec3 modelColor = tex.rgb*(color_Kd);
	if(tex.a == 0){
		discard;
	}
	vec4 ambiantColor = tex*vec4(sceneAmbiantColor*color_Kd, color_Alpha);
	
	vec3 lightColorSum = vec3(0.0f, 0.0f, 0.0f);
	vec3 lightSpecularSum = vec3(0.0f, 0.0f, 0.0f);
	
	vec4 colorambient = tex*vec4(0.0f, 0.0f, 0.0f, color_Alpha);
	vec4 colorlight = tex*vec4(0.0f, 0.0f, 0.0f,color_Alpha);
	
	float shadowSum = 0.0f;
	float bias = 0.0005;

	
	for(int i=0; i < lightCount; i++){
		if(lights[i].type == LIGHT_SUN){
			float shadow = (1.0-ShadowCalculation(bias, i));
			if(shadow > 0){
			//if(true){
				vec3 pointToLight = normalize(-lights[i].dir);
				vec3 normal=normalize(sh_normal);
				float lightFactor = max(0.0, dot(normal, pointToLight));
				
				if(useNormalMap == true && lightFactor > 0.0){
					normal = texture(normalMap, textCoords).rgb;
					normal = normalize(normal * 2.0 - 1.0);
					
					normal = normalize(TBN_matrix * normal);
					lightFactor = max(0.0, dot(normal, pointToLight));
				}
				vec3 eye= normalize(worldeye_pos);
				//float lightFactor = max(0.0, dot(normal, pointToLight));
				vec3 reflectDir= normalize(reflect(pointToLight,normal));
				float specularFactor = pow(max(0.0, dot(reflectDir,eye)), color_Ns);
				specularFactor = 0;
				
				lightColorSum += lights[i].color*lightFactor*lights[i].intensity/**shadow*/;
				lightSpecularSum += lights[i].color*specularFactor*lights[i].intensity/**shadow*/;
			}
			
			//shadowSum += ;
		}else{
			vec3 pointToLight = normalize(lights[i].pos-worldPos);
			vec3 normal=normalize(sh_normal);
			if(useNormalMap == true){
					normal = texture(normalMap, textCoords).rgb;
					normal = normalize(normal * 2.0 - 1.0);
					normal = normalize(TBN_matrix * normal);
			}
			float angle = acos(dot(pointToLight, -lights[i].dir))*360.0/(2.0*3.14);
			if(angle*2 <= lights[i].angle){
				vec3 eye= normalize(worldeye_pos);
				vec3 reflectDir= normalize(reflect(pointToLight,normal));
				float shadow = (1.0-ShadowCalculation(bias, i));
				float lightFactor = max(0.0, dot(normal, pointToLight));
				float specularFactor = pow(max(0.0, dot(reflectDir,eye)), color_Ns);
				lightColorSum += lights[i].color*lightFactor*lights[i].intensity*shadow;
				lightSpecularSum += lights[i].color*specularFactor*lights[i].intensity*shadow;	
				//lightColorSum += lights[i].color*lightFactor*lights[i].intensity;
				//lightSpecularSum += lights[i].color*specularFactor*lights[i].intensity;
				//shadowSum += ShadowCalculation(bias, i);
			
				
			}
		}
		/*if(i == 1){
			out_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
			return;
		}*/
		/*colorambient = vec4(colorambient.rgb + color_Kd*tex.rgb*lightColor[i],colorambient.a);
		colorlight=tex*vec4((lightFactor*color_Kd + specularFactor*color_Ks)*lightIntensity[0],color_Alpha);*/
	}
	//shadowSum /= lightCount;
	lightColorSum *= modelColor;
	lightSpecularSum *= (tex.rgb*color_Ks);
	
	// Le bias augmente en fonction de l'angle entre la lumiere et l'objet
	//if(dot(normal, -lightDir) < 0.25){ bias = 0.005; }else{ bias = 0.0005; }
	//colorlight = vec4(colorlight.rgb*(1.0f-shadow), colorlight.a);
	//out_Color = colorlight+colorambient;
	out_Color = vec4(ambiantColor.rgb+(lightColorSum+lightSpecularSum), ambiantColor.a);
}