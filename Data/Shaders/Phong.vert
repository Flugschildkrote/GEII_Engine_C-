#version 330 core
   
layout(location = 0) in vec3 in_Vertex;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_UV;
    
out vec2 textCoords;
out vec3 worldPos;
out vec3 sh_normal;
out vec4 pos_LightSpace;

uniform mat4 model_matrix;
uniform mat4 light_matrix;
uniform mat4 mvp;

void main(void){
   gl_Position = mvp * vec4(in_Vertex,1.0);
   textCoords = in_UV;
   worldPos = (model_matrix*vec4(in_Vertex, 1.0f)).xyz;
   sh_normal = (model_matrix*vec4(in_Normal,0.0)).xyz; /*only without scale factor!*/
   pos_LightSpace = (light_matrix*vec4(in_Vertex, 1.0f));
}