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
mat4 MVP, projection, view;

float rx, ry, rz;

vec2 direction;
float speed = 2.0f;

void KeyCallback(KeyEvent& event)
{
	switch (event.GetEventType())
	{
	case EventType::KeyPressed:
		switch (event.GetKeyCode())
		{
		case 'W':
			direction.y = 1.0f;
			break;

		case 'S':
			direction.y = -1.0f;
			break;

		case 'A':
			direction.x = -1.0f;
			break;

		case 'D':
			direction.x = 1.0f;
			break;
		}
		break;

	case EventType::KeyReleased:
		switch (event.GetKeyCode())
		{
		case 'W':
		case 'S':
			direction.y = 0.0f;
			break;

		case 'A':
		case 'D':
			direction.x = 0.0f;
			break;
		}
	}
}

void core::OnStart()
{
	Application::SetKeyPressedEventCallback(KeyCallback, 'W');
	Application::SetKeyPressedEventCallback(KeyCallback, 'A');
	Application::SetKeyPressedEventCallback(KeyCallback, 'S');
	Application::SetKeyPressedEventCallback(KeyCallback, 'D');
	Application::SetKeyReleasedEventCallback(KeyCallback, 'W');
	Application::SetKeyReleasedEventCallback(KeyCallback, 'A');
	Application::SetKeyReleasedEventCallback(KeyCallback, 'S');
	Application::SetKeyReleasedEventCallback(KeyCallback, 'D');

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
	testEntity->GetComponent<Camera>()->SetPerspective(DegToRad(60.0f), 16.0f / 9.0f, 0.01f, 1000.0f);

	vec3 move = -2.0f * testEntity->GetComponent<Camera>()->GetForward();
	testEntity->GetComponent<Transform>()->Move(move);
}

void core::OnUpdate(float deltaTime)
{
	if (direction.x != 0.0f || direction.y != 0.0f)
	{
		vec3 forward = testEntity->GetComponent<Camera>()->GetForward();
		vec3 right = forward.Cross(vec3::Y()).Normalize();

		vec3 move = vec3::Normalize(forward * direction.y + right * direction.x);
		testEntity->GetComponent<Transform>()->Move((speed * deltaTime) * move);
	}
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
	view = testEntity->GetComponent<Camera>()->GenerateViewMatrix();
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