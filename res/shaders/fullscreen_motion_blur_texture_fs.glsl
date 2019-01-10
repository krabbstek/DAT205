#version 420 core

in vec4 position;
in vec2 texCoords;

out vec4 out_Color;

layout (binding = 0) uniform sampler2D colorTexture;
layout (binding = 1) uniform sampler2D velocityTexture;

const unsigned int NUM_SAMPLES = 20;
const float NUM_SAMPLES_LIM = 0.5f * (NUM_SAMPLES - 1);
const float STEP_MULTIPLIER = 5;

void main()
{
	vec2 texScale = 1.0 / textureSize(colorTexture, 0);

	vec4 texVelocity = texture(velocityTexture, texCoords);
	vec2 velocity = texVelocity.xy / texVelocity.w;

	out_Color = vec4(0.0, 0.0, 0.0, 1.0);
	vec2 stepSize = STEP_MULTIPLIER * texScale;
	for (float offset = -NUM_SAMPLES_LIM; offset <= NUM_SAMPLES_LIM; offset += 1.0)
	{
		out_Color += texture(colorTexture, texCoords + stepSize * offset * velocity);
	}
	out_Color /= NUM_SAMPLES;
}