#version 330 core
   
layout (location = 0) out vec4 out_Color;

uniform vec3 colorPicking;

void main(void){
    out_Color = vec4(colorPicking, 1.0);
}

