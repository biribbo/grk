#version 430 core

uniform vec3 color;
uniform vec3 lightPos;
uniform vec3 lightColor;

in vec3 fragNormal;
in vec3 fragPosition;

out vec4 outColor;
void main()
{
	vec3 normal = normalize(fragNormal);
	vec3 viewDir = normalize(fragPosition);
	vec3 lightDir = normalize(fragPosition - lightPos);
	vec3 lightDirNormalized = normalize(-lightDir);
	vec3 reflectionDir = reflect(lightDirNormalized, normal);
	float diffuse = max(dot(normal, lightDirNormalized), 0.0);
	float specular = max(dot(viewDir, reflectionDir), 0.0);
	vec3 finalColor = color * (diffuse + specular) * lightColor;
	outColor = vec4(finalColor, 1.0);
}
