#version 330 core

in vec3 Color;
in vec3 Normal;  
in vec3 FragPos;  
in vec3 FragColor;

out vec4 color;

uniform vec3 lightPos;  

void main()
{    
	
	float ambientStrength = 0.24f;
	vec3 lightColor = vec3(1.f,1.f,1.f);
    vec3 ambient = ambientStrength * lightColor;

	vec3 objectColor = FragColor;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 result = (ambient + diffuse) * objectColor;

	color = vec4(result, 1.0f);
	
	//color = vec4(FragColor, 1.0f);
}
