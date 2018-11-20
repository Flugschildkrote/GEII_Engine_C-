#version 330 core

out vec4 out_Color;

in vec3 position;

void main()
{
    out_Color = vec4(position+0.3, 1.0);
}
