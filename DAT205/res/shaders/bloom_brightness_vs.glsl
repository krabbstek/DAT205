#version 430 core

precision highp float;

layout (location = 0) in vec2 in_Position;

out vec2 texCoord;

void main() 
{
	gl_Position = vec4(in_Position, 0.0, 1.0);
	texCoord = 0.5 * (in_Position + vec2(1.0));
}