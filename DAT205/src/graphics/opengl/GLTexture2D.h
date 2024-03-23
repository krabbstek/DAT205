#pragma once

#include <glad/glad.h>

#define MAX_TEXTURES 16

class GLTexture2D
{
public:
	GLTexture2D();
	~GLTexture2D();

	/* Loads image in data with format dataFormat and type type to texture with format openGLFormat. */
	void Load(GLuint openGLFormat, const void* data, unsigned int width, unsigned int height, GLuint dataFormat, GLuint type);
	/* Loads image in data with format dataFormat and type type to texture with format openGLFormat into specified level. */
	void Load(GLuint openGLFormat, const void* data, unsigned int width, unsigned int height, GLuint dataFormat, GLuint type, int level);
	/* Loads image from file into texture. */
	bool LoadFromFile(const char* filePath);
	/* Loads image from file into texture in specified level. */
	bool LoadFromFile(const char* filePath, int level);

	void SetWrapS(GLuint parameter) const;
	void SetWrapT(GLuint parameter) const;
	void SetWrapST(GLuint parameter) const;
	void SetMinFilter(GLuint parameter) const;
	void SetMagFilter(GLuint parameter) const;
	void SetMinMagFilter(GLuint parameter) const;

	void GenerateMipmap();

	void Bind(unsigned int slot = 0) const;
	static void Unbind(unsigned int slot = 0);

	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }
	inline GLuint RendererID() const { return m_RendererID; }

private:
	GLuint m_RendererID;
	unsigned int m_Width, m_Height;

	friend class GLFramebuffer;
};