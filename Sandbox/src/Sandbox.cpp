#include <iostream>

#include "Core.h"
#include "imgui.h"

using namespace core;

GLIndexBuffer* ibo;
GLVertexBuffer* vbo;
GLVertexArray* vao;
GLShader* shader;
GLTexture2D* texture;
GLTexture2D* colorOverlayTexture;

Mesh* cube;

Entity* testEntity;

float t = 0.0f;
mat4 translate(1.0f);
mat4 MVP, prevMVP, projection = mat4::Perspective(DegToRad(90.0f), 16.0f / 9.0f, 0.01f, 1000.0f);
mat4 view = mat4::Translate(0.0f, 0.0f, -2.0f) * mat4::RotateY(0.5f);

float rx, ry, rz;

void core::OnStart()
{
	//Model::LoadDAEModelFromFile("../res/models/mannequin/mannequin.dae");
	cube = Mesh::Cube();

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
		shader->AddShaderFromFile(GL_VERTEX_SHADER, "../res/shaders/basic_vert.glsl");
		shader->AddShaderFromFile(GL_FRAGMENT_SHADER, "../res/shaders/basic_frag.glsl");
		shader->CompileShaders();
		shader->Bind();
		cube->SetShader(shader);

		texture = new GLTexture2D();
		texture->LoadFromFile("../res/textures/Test.png");
		texture->SetWrapST(GL_CLAMP_TO_EDGE);
		texture->SetMinMagFilter(GL_LINEAR);
		texture->Bind(0);
	}

	testEntity = new Entity();
	testEntity->AddComponent(new Transform());
	testEntity->AddComponent(new Camera());
	testEntity->GetComponent<Camera>()->SetPerspective(DegToRad(90.0f), 16.0f / 9.0f, 0.01f, 1000.0f);
}

void core::OnUpdate(float deltaTime)
{
	
}

void core::OnRender()
{
	ImGui::Begin("SandBox1");
	ImGui::SliderAngle("RX", &rx, -180.0f, 180.0f);
	ImGui::SliderAngle("RY", &ry, -180.0f, 180.0f);
	ImGui::SliderAngle("RZ", &rz, -180.0f, 180.0f);
	ImGui::End();
	
	Quaternion qx = Quaternion::Rotation(vec3(1.0f, 0.0f, 0.0f), rx);
	Quaternion qy = Quaternion::Rotation(vec3(0.0f, 1.0f, 0.0f), ry);
	Quaternion qz = Quaternion::Rotation(vec3(0.0f, 0.0f, 1.0f), rz);
	Quaternion q = qx * qy * qz;
	projection = testEntity->GetComponent<Camera>()->GetProjectionMatrix();
	MVP = projection * view * q.Matrix();
	shader->SetUniformMat4("transformation", MVP);

	GLFramebuffer::SetDefaultFramebuffer();
	cube->Render();
}

void core::OnQuit()
{
	delete ibo;
	delete vbo;
	delete vao;
	delete shader;
	delete texture;
	delete colorOverlayTexture;

	delete testEntity;
	delete cube;
}