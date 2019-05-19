#version 430 core

#define PI 3.141592653589793

in vec3 viewSpacePosition_FS;
in vec3 viewSpaceNormal_FS;
in vec2 texCoord_FS;

out vec4 out_Color;

void main()
{
	out_Color = vec4(1.0);
}