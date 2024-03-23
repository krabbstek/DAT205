#include "Globals.h"
#include "graphics/Light.h"
#include "graphics/Material.h"
#include "graphics/Renderer.h"
#include "graphics/RenderTechnique.h"
#include "graphics/opengl/OpenGL.h"
#include "graphics/renderpasses/ClearDefaultFramebufferPass.h"
#include "graphics/renderpasses/StartTimerPass.h"
#include "graphics/renderpasses/StopTimerPass.h"
#include "graphics/renderpasses/PlotTimersPass.h"
#include "graphics/renderpasses/forward/ForwardLightingPass.h"
#include "math/math.h"
#include "meshes/Cube.h"
#include "meshes/PlaneMesh.h"
#include "model/Model.h"

#include <chrono>
#include <iostream>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <stb_image.h>

#define USE_IMGUI

GLFWwindow* window;

Renderer renderer;

Material material;

enum RENDER_MODE
{
	NONE = 0,
	DEFAULT,
	NUM_RENDER_MODES,
};

RENDER_MODE currentRenderMode = DEFAULT;
std::pair<std::shared_ptr<RenderTechnique>, std::string> renderModes[NUM_RENDER_MODES];

int Init();
void InitNoRendering();
void InitDefaultRendering();
void ImGuiRender();

int main()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> prevTime, currentTime;
	prevTime = std::chrono::high_resolution_clock::now();

	int initResult = Init();
	if (initResult)
		return initResult;

	{
		renderer.camera.projectionMatrix = mat4::Perspective(g_FOV, g_AspectRatio, g_NearPlaneDepth, g_FarPlaneDepth);
		renderer.camera.position = vec3(0.0f, 4.0f, 12.0f);
		renderer.camera.rotation = vec3(-0.4f, 0.0f, 0.0f);
		mat4 V = renderer.camera.GetViewMatrix();

		PlaneMesh planeMesh({ 0.0f, -0.5f, 0.0f }, { 24.0f, 24.0f });

		material.albedo = vec4(0.2f, 0.3f, 0.8f);
		material.emission = 0.0f;
		material.reflectivity = 1.0f;
		material.shininess = 1.0f;
		material.metalness = 0.5f;
		material.fresnel = 0.5f;

		std::shared_ptr<Model> model = std::shared_ptr<Model>(Model::LoadModelFromOBJ("res/models/NewShip.obj"));

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glCullFace(GL_BACK));
		GLCall(glDepthFunc(GL_LEQUAL));

		/// Render techniques
		{
			// No rendering
			InitNoRendering();

			// Default rendering
			InitDefaultRendering();
		}
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

#ifdef USE_IMGUI
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
#endif

			currentTime = std::chrono::high_resolution_clock::now();
			float dt = float((currentTime - prevTime).count()) * 1e-9f;
			prevTime = currentTime;

			// Render
			std::shared_ptr<RenderTechnique> currentRenderTechnique = renderModes[currentRenderMode].first;
			//currentRenderTechnique->Render(planeMesh);
			currentRenderTechnique->Render(*model);
			currentRenderTechnique->Render();
			
#ifdef USE_IMGUI
			ImGuiRender();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
			
			glfwSwapBuffers(window);
		}
	}

	for (int i = 0; i < NUM_RENDER_MODES; i++)
		renderModes[i].first.reset();

	ImGui_ImplGlfw_Shutdown();

	glfwTerminate();

	return 0;
}


int Init()
{
	int glfwResult = glfwInit();
	if (!glfwResult)
	{
		std::printf("Failed to initialize GLFW!\n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, "DAT205", NULL, NULL);
	if (!window)
	{
		std::printf("Failed to create glfwWindow!\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

#ifdef USE_IMGUI
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 150");
	ImGui_ImplGlfw_InitForOpenGL(window, true);
#endif

	stbi_set_flip_vertically_on_load(1);

	return 0;
}


void ImGuiRender()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	ImGui::Begin("Config");

	ImGui::Text("Render mode");
	for (int i = 0; i < NUM_RENDER_MODES; i++)
		ImGui::RadioButton(renderModes[i].second.c_str(), (int*)&currentRenderMode, i);

	ImGui::Separator();

	ImGui::Text("Global light");
	ImGui::SliderFloat("Global light intensity multiplier", &g_GlobalLightIntensity, 0.0f, 10000.0f, "%.2f", 3.0f);
	ImGui::SliderFloat3("Global light position ", &g_GlobalLightPosition.x, -25.0f, 25.0f, "%.1f", 1.0f);

	ImGui::Separator();

	ImGui::Separator();

	ImGui::Text("Material");
	ImGui::ColorEdit3("Albedo", &material.albedo.r);
	ImGui::SliderFloat("Reflectivity", &material.reflectivity, 0.0f, 1.0f, "%.2f", 1.0f);
	ImGui::SliderFloat("Shininess", &material.shininess, 0.0f, 1000.0f, "%.2f", 3.0f);
	ImGui::SliderFloat("Metalness", &material.metalness, 0.0f, 1.0f, "%.2f", 1.0f);
	ImGui::SliderFloat("Fresnel", &material.fresnel, 0.0f, 1.0f, "%.2f", 1.0f);

	ImGui::End();
}


void InitNoRendering()
{
	std::shared_ptr<ClearDefaultFramebufferPass> clearDefaultFramebufferPass = std::make_shared<ClearDefaultFramebufferPass>(renderer, std::shared_ptr<GLShader>());

	std::shared_ptr<RenderTechnique> noRendering = std::make_shared<RenderTechnique>();
	noRendering->AddRenderPass(clearDefaultFramebufferPass);

	renderModes[NONE].first = noRendering;
	renderModes[NONE].second = "No rendering";
}

void InitDefaultRendering()
{
	// Shaders
	std::shared_ptr<GLShader> forwardShader = std::make_shared<GLShader>();
	forwardShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/forward/forward_vs.glsl");
	forwardShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/forward/forward_fs.glsl");
	forwardShader->CompileShaders();

	// Render passes
	std::shared_ptr<ClearDefaultFramebufferPass> clearDefaultFramebufferPass = std::make_shared<ClearDefaultFramebufferPass>(renderer, std::shared_ptr<GLShader>());
	std::shared_ptr<ForwardLightingPass> forwardLightingPass = std::make_shared<ForwardLightingPass>(renderer, forwardShader);

	// Render technique
	std::shared_ptr<RenderTechnique> defaultRendering = std::make_shared<RenderTechnique>();
	defaultRendering->AddRenderPass(clearDefaultFramebufferPass);
	defaultRendering->AddRenderPass(forwardLightingPass);

	// Add render technique to array of render techniques
	renderModes[DEFAULT].first = defaultRendering;
	renderModes[DEFAULT].second = "Default rendering";
}