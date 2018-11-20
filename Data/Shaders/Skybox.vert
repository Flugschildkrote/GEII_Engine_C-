#version 330 core
   
layout(location = 0) in vec3 in_Vertex;
layout(location = 2) in vec2 in_UV;
    
out vec2 textCoords;

uniform mat4 mvp;

void main(void){
   gl_Position = mvp * vec4(in_Vertex,1.0);
   textCoords = in_UV;
}