#version 330 core

precision highp float;

smooth in vec4 pos;
smooth in vec4 prevPos;
smooth in vec2 texCoords;

out vec4 out_Color;

uniform float uVelocityScale = 0.1;
uniform sampler2D tex;

const int NUM_SAMPLES = 10;

void main()
{
	vec2 velocity = (pos.xy / pos.w - prevPos.xy / prevPos.w) * uVelocityScale;
	float texScale = 1.0 / max(textureSize(tex, 0).x, textureSize(tex, 0).y);
	out_Color = vec4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		out_Color.rgb += texture(tex, texCoords + velocity).rgb;
	}
	out_Color /= NUM_SAMPLES;
}