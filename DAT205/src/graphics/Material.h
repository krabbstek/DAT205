#pragma once

#include "math/math.h"
#include "graphics/opengl/OpenGL.h"

struct Material
{
public:
	std::string name;
	vec4 albedo;
	float emission;
	float reflectivity;
	float shininess;
	float metalness;
	float fresnel;
	float transparency;
	std::shared_ptr<GLTexture2D> albedoTexture;
	std::shared_ptr<GLTexture2D> emissionTexture;
	std::shared_ptr<GLTexture2D> reflectivityTexture;
	std::shared_ptr<GLTexture2D> shininessTexture;
	std::shared_ptr<GLTexture2D> metalnessTexture;
	std::shared_ptr<GLTexture2D> fresnelTexture;

	void Bind(GLShader& shader) const;
};