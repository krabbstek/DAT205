#include <iostream>

#include "../Core.h"

using namespace core;

// Objects for rendering rectangle textures in full window
GLIndexBuffer* fullscreenTextureIBO;
GLShader* fullscreenTextureShader;
GLVertexArray* fullscreenTextureVAO;
GLVertexBuffer* fullscreenTextureVBO;

GLFramebuffer* velocityFramebuffer;
GLTexture2D* velocityFramebufferColorTexture;
GLTexture2D* velocityFramebufferVelocityTexture;

GLIndexBuffer* ibo;
GLVertexBuffer* vbo;
GLVertexArray* vao;
GLShader* shader;
GLTexture2D* texture;
GLTexture2D* colorOverlayTexture;

float t = 0.0f;
mat4 translate(1.0f);
mat4 MVP, prevMVP, projection = mat4::Perspective(DegToRad(90.0f), 16.0f / 9.0f, 0.01f, 1000.0f);
mat4 view = mat4::Translate(0.0f, 0.0f, -2.0f) * mat4::RotateY(0.5f);

Mesh* cube;

void core::OnStart()
{
	//Model::LoadOBJModelFromFile("../Core/res/models/house/cottage_obj.obj");
	cube = Mesh::Cube();

	MVP = projection * view;
	prevMVP = projection * view;

	// Create fullscreen texture objects
	{
		float fullscreenTextureVertices[] =
		{
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f,
			-1.0f,  1.0f,  0.0f, 1.0f,
		};
		unsigned int fullscreenTextureIndices[] =
		{
			0, 1, 2,
			0, 2, 3,
		};

		fullscreenTextureIBO = new GLIndexBuffer(fullscreenTextureIndices, sizeof(fullscreenTextureIndices) / sizeof(unsigned int));
		fullscreenTextureVBO = new GLVertexBuffer(fullscreenTextureVertices, sizeof(fullscreenTextureVertices));
		GLVertexBufferLayout fullscreenTextureLayout;
		fullscreenTextureLayout.Push(GL_FLOAT, 2);
		fullscreenTextureLayout.Push(GL_FLOAT, 2);
		fullscreenTextureVBO->SetVertexBufferLayout(fullscreenTextureLayout);
		fullscreenTextureVAO = new GLVertexArray();
		fullscreenTextureVAO->AddVertexBuffer(*fullscreenTextureVBO);

		fullscreenTextureShader = new GLShader();
		fullscreenTextureShader->AddShaderFromFile(GL_VERTEX_SHADER, "../Core/res/shaders/fullscreen_motion_blur_texture_vs.glsl");
		fullscreenTextureShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "../Core/res/shaders/fullscreen_motion_blur_texture_fs.glsl");
		fullscreenTextureShader->CompileShaders();
	}

	// Create velocity framebuffer
	{
		velocityFramebuffer = new GLFramebuffer(1280, 720);
		velocityFramebufferColorTexture = velocityFramebuffer->AttachTexture(GL_RGB32F, 0);
		velocityFramebufferVelocityTexture = velocityFramebuffer->AttachTexture(GL_RGB32F, 1);
		velocityFramebuffer->SetDrawBufferAttachments(2);
		velocityFramebuffer->GenerateDepthStencilRenderbuffer();
		velocityFramebuffer->ClearColor(0.0f, 0.0f, 0.1f, 1.0f);

		GLFramebuffer::Unbind();
	}

	// Create moving square buffers
	{
		float v[] =
		{
			-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		};
		unsigned int i[] =
		{
			0, 1, 2,
			0, 2, 3,
		};
		vbo = new GLVertexBuffer(v, sizeof(v));
		GLVertexBufferLayout layout;
		layout.Push(GL_FLOAT, 2);
		layout.Push(GL_FLOAT, 3);
		layout.Push(GL_FLOAT, 2);
		vbo->SetVertexBufferLayout(layout);
		vao = new GLVertexArray();
		vao->AddVertexBuffer(*vbo);
		vao->Bind();
		ibo = new GLIndexBuffer(i, sizeof(i) / sizeof(unsigned int));
		ibo->Bind();

		shader = new GLShader();
		//shader->AddShaderFromFile(GL_VERTEX_SHADER, "../Core/res/shaders/basic_motion_blur_vs.glsl");
		//shader->AddShaderFromFile(GL_FRAGMENT_SHADER, "../Core/res/shaders/basic_motion_blur_fs.glsl");
		shader->AddShaderFromFile(GL_VERTEX_SHADER, "../Core/res/shaders/basic_vert.glsl");
		shader->AddShaderFromFile(GL_FRAGMENT_SHADER, "../Core/res/shaders/basic_frag.glsl");
		shader->CompileShaders();
		shader->Bind();

		texture = new GLTexture2D();
		texture->LoadFromFile("../Core/res/textures/Test.png");
		texture->SetWrapST(GL_CLAMP_TO_EDGE);
		texture->SetMinMagFilter(GL_LINEAR);
		texture->Bind(0);
	}
}

void core::OnUpdate(float deltaTime)
{
	t += deltaTime;
	float x = sin(t * PI);

	prevMVP = MVP;
	translate = mat4::Translate(x, 0.0f, 0.0f);
	MVP = projection * view * translate;

	//shader->SetUniformMat4("MVP", MVP);
	//shader->SetUniformMat4("prevMVP", prevMVP);
	shader->SetUniformMat4("transformation", MVP);
}

void core::OnRender()
{
#if 0
	velocityFramebuffer->Bind();
	GLFramebuffer::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	vao->Bind();
	ibo->Bind();
	shader->Bind();
	texture->Bind();
	glDrawElements(GL_TRIANGLES, ibo->Count(), GL_UNSIGNED_INT, 0);
	
	GLFramebuffer::SetDefaultFramebuffer();
	fullscreenTextureShader->Bind();
	fullscreenTextureVAO->Bind();
	fullscreenTextureIBO->Bind();
	velocityFramebufferColorTexture->Bind(0);
	velocityFramebufferVelocityTexture->Bind(1);
	glDrawElements(GL_TRIANGLES, fullscreenTextureIBO->Count(), GL_UNSIGNED_INT, 0);
#endif

	GLFramebuffer::SetDefaultFramebuffer();
	cube->GetVertexArray()->Bind();
	cube->GetIndexBuffer()->Bind();
	shader->Bind();
	texture->Bind();
	glDrawElements(GL_TRIANGLES, cube->GetIndexBuffer()->Count(), GL_UNSIGNED_INT, 0);
}