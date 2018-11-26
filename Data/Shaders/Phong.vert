#version 330 core
#define MAX_LIGHTS 2

layout(location = 0) in vec3 in_Vertex;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_UV;
    
out vec2 textCoords;
out vec3 worldPos;
out vec3 sh_normal;
out vec4 lightSpacePos[MAX_LIGHTS];


uniform mat4 model_matrix;
uniform mat4 mvp;
uniform int lightCount;
uniform mat4 light_matrix[MAX_LIGHTS];

void main(void){
	gl_Position = mvp * vec4(in_Vertex,1.0);
	textCoords = in_UV;
	worldPos = (model_matrix*vec4(in_Vertex, 1.0f)).xyz;
	sh_normal = (model_matrix*vec4(in_Normal,0.0)).xyz; /*only without scale factor!*/
	for(int i=0; i < lightCount; i++){
		lightSpacePos[i] = light_matrix[i]*vec4(in_Vertex, 1.0);
	}
}