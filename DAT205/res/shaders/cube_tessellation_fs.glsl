#version 430 core

in vec3 viewSpacePosition_FS;
in vec3 viewSpaceNormal_FS;
in vec2 texCoord_FS;

out vec4 out_Color;

void main()
{
	out_Color = vec4(viewSpaceNormal_FS, 1.0);
}