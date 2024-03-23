#pragma once

#include "Renderer.h"
#include "opengl/GLShader.h"

class Renderable
{
public:
	Renderable(std::shared_ptr<GLShader> prepassShader, std::shared_ptr<GLShader> mainShader)
		: m_PrepassShader(prepassShader), m_MainShader(mainShader) {}
	virtual ~Renderable() {}

	virtual void PrepassRender(const Renderer& renderer) const = 0;
	virtual void Render(const Renderer& renderer) const = 0;

	virtual inline bool IsTransparent() const { return false; }

	inline void SetPrepassShader(std::shared_ptr<GLShader> prepassShader) { m_PrepassShader = prepassShader; }
	inline void SetMainShader(std::shared_ptr<GLShader> mainShader) { m_MainShader = mainShader; }

protected:
	std::shared_ptr<GLShader> m_PrepassShader;
	std::shared_ptr<GLShader> m_MainShader;
};