#version 430 core

in vec4 color;
in vec4 pos_local;
in vec4 pos_global;

out vec4 out_color;
void main()
{
	out_color = color;
}