#include "Material.h"

#include "Globals.h"

void Material::Bind(GLShader& shader) const
{
	bool hasAlbedoTexture = albedoTexture ? albedoTexture->RendererID() : false;
	bool hasReflectivityTexture = reflectivityTexture ? reflectivityTexture->RendererID() : false;
	bool hasMetalnessTexture = metalnessTexture ? metalnessTexture->RendererID() : false;
	bool hasFresnelTexture = fresnelTexture ? fresnelTexture->RendererID() : false;
	bool hasShininessTexture = shininessTexture ? shininessTexture->RendererID() : false;
	bool hasEmissionTexture = emissionTexture ? emissionTexture->RendererID() : false;

	if (hasAlbedoTexture)
		albedoTexture->Bind(0);
	if (hasReflectivityTexture)
		reflectivityTexture->Bind(1);
	if (hasMetalnessTexture)
		metalnessTexture->Bind(2);
	if (hasFresnelTexture)
		fresnelTexture->Bind(3);
	if (hasShininessTexture)
		shininessTexture->Bind(4);
	if (hasEmissionTexture)
		emissionTexture->Bind(5);

	shader.SetUniform1i("u_HasAlbedoTexture", hasAlbedoTexture);
	shader.SetUniform1i("u_HasReflectivityTexture", hasReflectivityTexture);
	shader.SetUniform1i("u_HasMetalnessTexture", hasMetalnessTexture);
	shader.SetUniform1i("u_HasFresnelTexture", hasFresnelTexture);
	shader.SetUniform1i("u_HasShininessTexture", hasShininessTexture);
	shader.SetUniform1i("u_HasEmissionTexture", hasEmissionTexture);

	shader.SetUniform4f("u_Material.albedo", albedo);
	shader.SetUniform1f("u_Material.reflectivity", reflectivity);
	shader.SetUniform1f("u_Material.metalness", metalness);
	shader.SetUniform1f("u_Material.fresnel", fresnel);
	shader.SetUniform1f("u_Material.shininess", shininess);
	shader.SetUniform1f("u_Material.emission", emission);
}