#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;

uniform mat4 transformation;
uniform mat4 modelMatrix;
uniform vec3 cameraPos;

out vec3 fragNormal;
out vec3 fragPosition;

void main()
{
	gl_Position = transformation * vec4(vertexPosition, 1.0);
	vec4 V = vec4(vertexNormal, 0.0);
	vec4 U = vec4(vertexPosition, 0.0);
	V = V * modelMatrix;
	U = U * modelMatrix;
	fragNormal = normalize(V.xyz);
	fragPosition = normalize(cameraPos - U.xyz);
}
