#version 330 core

layout(location = 0) in vec3 in_Vertex;

out vec3 position;
uniform mat4 mvp;

void main()
{
    gl_Position = mvp*vec4(in_Vertex, 1.0);
	position = in_Vertex;
	//gl_Position = vec4(0.0,0.0,0.0,1.0);
}
