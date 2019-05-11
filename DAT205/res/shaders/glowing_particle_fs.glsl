#version 430 core

in vec2 currentPosition;
in vec2 clipSpaceVelocity;
in vec3 emission;
in float lifetime;
in float lifelength;
in float radius;

out vec4 out_Color;

uniform float u_LightIntensityMultiplier;

layout (location = 0) uniform sampler2D u_FireSphereTexture;

void main()
{
	out_Color = texture(u_FireSphereTexture, gl_PointCoord);
	out_Color.rgb *= emission * u_LightIntensityMultiplier * 10.0;
}