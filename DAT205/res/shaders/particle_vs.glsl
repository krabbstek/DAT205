#version 430 core

layout (location = 0) in vec4 in_Position;
layout (location = 2) in float in_Scale;
layout (location = 3) in vec3 in_Emission;
layout (location = 4) in float in_LifeTime;
layout (location = 5) in float in_LifeLength;

out vec2 clipSpaceVelocity;
out vec3 emission;
out float lifeTime;
out float lifeLength;

uniform mat4 u_P = mat4(1.0);
uniform mat4 u_PrevVVP = mat4(1.0);
uniform vec2 u_ViewportSize;

void main()
{
	vec4 currentClipSpacePosition = u_P * in_Position;
	vec4 previousClipSpacePosition = u_PrevVVP * in_Position;
	clipSpaceVelocity = (currentClipSpacePosition.xy / currentClipSpacePosition.w) - (previousClipSpacePosition.xy / previousClipSpacePosition.w);

	gl_Position = currentClipSpacePosition;

	emission = in_Emission;
	lifeTime = in_LifeTime;
	lifeLength = in_LifeLength;

    // Calculate one projected corner of a quad at the particles view space depth.
    vec4 projQuad = u_P * vec4(1.0, 1.0, in_Position.z, in_Position.w);
    // Calculate the projected pixel size.
    vec2 projPixel = u_ViewportSize * projQuad.xy / projQuad.w;
    // Use scale factor as sum of x and y sizes.
    float scaleFactor = (projPixel.x + projPixel.y);
    // Scale the point with regard to the previosly defined scale_factor
    // and the life (it will get larger the older it is)
    gl_PointSize = scaleFactor * in_Scale;
}