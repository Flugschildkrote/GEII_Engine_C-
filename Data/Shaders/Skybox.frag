#version 330 core
    
layout(location = 0) out vec4 out_Color;

uniform sampler2D texture_sampler;
   
in vec2 textCoords;

void main(void){
	out_Color = texture(texture_sampler,textCoords);
}