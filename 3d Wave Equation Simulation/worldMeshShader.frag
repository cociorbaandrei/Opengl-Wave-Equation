#version 330 core

in vec3 Color;
in vec3 Normal;  
in vec3 FragPos;  

out vec4 color;

void main()
{    
	color = vec4(Color * .2f, 1.0f);
}
