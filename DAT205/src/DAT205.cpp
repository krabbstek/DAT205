#include "Globals.h"
#include "graphics/Light.h"
#include "graphics/Material.h"
#include "graphics/Renderer.h"
#include "graphics/RenderTechnique.h"
#include "graphics/opengl/OpenGL.h"
#include "graphics/renderpasses/BackgroundPass.h"
#include "graphics/renderpasses/ComputeLightTilesPass.h"
#include "graphics/renderpasses/LightingPass.h"
#include "graphics/renderpasses/OutputSelectionPass.h"
#include "graphics/renderpasses/Prepass.h"
#include "graphics/renderpasses/SSAOPass.h"
#include "graphics/renderpasses/StartTimerPass.h"
#include "graphics/renderpasses/StopTimerPass.h"
#include "graphics/renderpasses/PlotTimersPass.h"
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

float cameraSpeed = 20.0f;
vec3 cameraMovement;
float mouseSensitivity = 0.005f;
vec2 mousePrevPos(-1.0f);

std::shared_ptr<RenderTechnique> renderTechnique;

std::shared_ptr<Model> fighterModel;

std::shared_ptr<GLTexture2D> environmentMap;
std::shared_ptr<GLTexture2D> irradianceMap;
std::shared_ptr<GLTexture2D> reflectionMap;

OUTPUT_SELECTION outputSelection = OUTPUT_SELECTION_LIGHTING;

int Init();
void HandleKeyInput(float deltaTime);
void MouseButtonCallback(GLFWwindow* window, int key, int action, int mods);
void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
void InitTiledForwardRendering();
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
		renderer.camera.position = vec3(0.0f, 10.0f, 20.0f);
		renderer.camera.rotation = vec3(-0.4f, 0.0f, 0.0f);
		mat4 V = renderer.camera.GetViewMatrix();

		PlaneMesh planeMesh({ 0.0f, -10.0f, 0.0f }, { 240.0f, 240.0f });

		fighterModel = std::shared_ptr<Model>(Model::LoadModelFromOBJ("res/models/NewShip.obj"));
		fighterModel->modelMatrix = mat4(1.0f);

		{
			int width, height, numChannels;
			float* data;

			stbi_set_flip_vertically_on_load(false);

			environmentMap = std::make_shared<GLTexture2D>();
			data = stbi_loadf("res/textures/envmaps/001.hdr", &width, &height, &numChannels, 3);
			environmentMap->Load(GL_RGB32F, data, width, height, GL_RGB, GL_FLOAT, 0);
			environmentMap->SetWrapST(GL_REPEAT);
			environmentMap->SetMagFilter(GL_LINEAR);
			environmentMap->SetMinFilter(GL_LINEAR);
			stbi_image_free(data);
			
			irradianceMap = std::make_shared<GLTexture2D>();
			irradianceMap->SetWrapST(GL_REPEAT);
			irradianceMap->SetMagFilter(GL_LINEAR);
			irradianceMap->SetMinFilter(GL_LINEAR);
			data = stbi_loadf("res/textures/envmaps/001_irradiance.hdr", &width, &height, &numChannels, 0);
			irradianceMap->Load(GL_RGB32F, data, width, height, GL_RGB, GL_FLOAT, 0);
			stbi_image_free(data);
		
			reflectionMap = std::make_shared<GLTexture2D>();
			reflectionMap->SetWrapST(GL_REPEAT);
			reflectionMap->SetMagFilter(GL_LINEAR);
			reflectionMap->SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
			reflectionMap->Bind();
			data = stbi_loadf("res/textures/envmaps/001_dl_0.hdr", &width, &height, &numChannels, 0);
			reflectionMap->Load(GL_RGB32F, data, width, height, GL_RGB, GL_FLOAT, 0);
			stbi_image_free(data);

			GLCall(glGenerateMipmap(GL_TEXTURE_2D));
			{
				char filename[256];
				for (int i = 1; i < 8; i++)
				{
					sprintf(filename, "res/textures/envmaps/001_dl_%d.hdr", i);
					float* data = stbi_loadf(filename, &width, &height, &numChannels, 0);
					reflectionMap->Load(GL_RGB32F, data, width, height, GL_RGB, GL_FLOAT, i);
					stbi_image_free(data);
				}
			}

			stbi_set_flip_vertically_on_load(true);
		}

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glCullFace(GL_BACK));
		GLCall(glDepthFunc(GL_LEQUAL));

		/// Render technique
		InitTiledForwardRendering();

		while (!glfwWindowShouldClose(window))
		{
			currentTime = std::chrono::high_resolution_clock::now();
			g_DeltaTime = float((currentTime - prevTime).count()) * 1e-9f;
			g_Time += g_DeltaTime;
			prevTime = currentTime;

			glfwPollEvents();

			HandleKeyInput(g_DeltaTime);

#ifdef USE_IMGUI
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
#endif

			// Render
			renderTechnique->Render(*fighterModel);
			renderTechnique->Render();
			
#ifdef USE_IMGUI
			ImGuiRender();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

glfwSwapBuffers(window);
		}
	}

	fighterModel.reset();

	environmentMap.reset();
	irradianceMap.reset();
	reflectionMap.reset();

	renderTechnique.reset();

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
	ImGui_ImplOpenGL3_Init("#version 430");
	ImGui_ImplGlfw_InitForOpenGL(window, true);
#endif
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);

	stbi_set_flip_vertically_on_load(1);

	return 0;
}


void HandleKeyInput(float deltaTime)
{
	vec3 forward = renderer.camera.GetWorldSpaceForward();
	vec3 up(0.0f, 1.0f, 0.0f);
	vec3 right = vec3::Normalize(vec3::Cross(forward, up));

	vec3 direction;

	if (glfwGetKey(window, GLFW_KEY_W))
		direction += forward;
	if (glfwGetKey(window, GLFW_KEY_S))
		direction -= forward;

	if (glfwGetKey(window, GLFW_KEY_A))
		direction -= right;
	if (glfwGetKey(window, GLFW_KEY_D))
		direction += right;

	if (glfwGetKey(window, GLFW_KEY_Q))
		direction -= up;
	if (glfwGetKey(window, GLFW_KEY_E))
		direction += up;

	renderer.camera.position += cameraSpeed * deltaTime * direction;
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS && mousePrevPos == vec2(-1.0f) && !ImGui::IsMouseHoveringAnyWindow())
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		mousePrevPos.x = float(xpos);
		mousePrevPos.y = float(ypos);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
	{
		mousePrevPos = vec2(-1.0f);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (mousePrevPos != vec2(-1.0f))
	{
		vec2 mousePos = vec2(float(xpos), float(ypos));
		renderer.camera.rotation.x -= mouseSensitivity * (mousePos.y - mousePrevPos.y);
		renderer.camera.rotation.y -= mouseSensitivity * (mousePos.x - mousePrevPos.x);
		renderer.camera.rotation.x = Clamp(renderer.camera.rotation.x, -g_MaxPitchAngle, g_MaxPitchAngle);
		mousePrevPos = mousePos;
	}
}


void ImGuiRender()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	ImGui::Begin("Config");

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Separator();

	ImGui::Text("Output");
	ImGui::RadioButton("Depth", (int*)&outputSelection, OUTPUT_SELECTION_DEPTH);
	ImGui::RadioButton("Normals", (int*)&outputSelection, OUTPUT_SELECTION_NORMAL);
	ImGui::RadioButton("SSAO", (int*)&outputSelection, OUTPUT_SELECTION_SSAO);
	ImGui::RadioButton("Lighting", (int*)&outputSelection, OUTPUT_SELECTION_LIGHTING);

	ImGui::Separator();

	ImGui::Text("Environment");
	ImGui::SliderFloat("Environment multiplier", &g_EnvironmentMultiplier, 0.0f, 2.0f);

	static float globalLightIntensity = g_GlobalLight.color.r;
	static vec4 globalLightPosition = vec4(11.0f, 40.0f, 38.0f, 1.0f);
	globalLightPosition.w = 1.0f;
	ImGui::Text("Global light");
	ImGui::SliderFloat("Global light intensity multiplier", &globalLightIntensity, 0.0f, 10000.0f, "%.2f", 3.0f);
	ImGui::SliderFloat3("Global light position ", &globalLightPosition.x, -25.0f, 25.0f, "%.1f", 1.0f);
	g_GlobalLight.color.rgb = vec3(globalLightIntensity);
	g_GlobalLight.viewSpacePosition = renderer.camera.GetViewMatrix() * globalLightPosition;

	ImGui::Separator();

	ImGui::Text("SSAO");
	ImGui::SliderFloat("SSAO kernel size", &g_SSAOKernelSize, 0.1f, 10.0f, "%.2f", 2.0f);
	ImGui::SliderFloat("SSAO radius", &g_SSAORadius, 0.1f, 10.0f, "%.2f", 2.0f);

	ImGui::Separator();

	static int meshIndex = 0;
	static int materialIndex = fighterModel->GetMeshes()[meshIndex].materialIndex;
	static auto meshGetter = [](void* vec, int index, const char** text)
	{
		auto& vector = *static_cast<std::vector<Mesh>*>(vec);
		if (index < 0 || index >= static_cast<int>(vector.size()))
			return false;
		*text = vector[index].name.c_str();
		return true;
	};
	static auto materialGetter = [](void* vec, int index, const char** text)
	{
		auto& vector = *static_cast<std::vector<Material>*>(vec);
		if (index < 0 || index >= static_cast<int>(vector.size())) 
			return false;
		*text = vector[index].name.c_str();
		return true;
	};

	if (ImGui::CollapsingHeader("Meshes", true))
	{
		Mesh& mesh = fighterModel->GetMeshes()[meshIndex];

		if (ImGui::ListBox("Meshes", &meshIndex, meshGetter, (void*)&fighterModel->GetMeshes(), int(fighterModel->GetMeshes().size()), 8))
			materialIndex = mesh.materialIndex;

		char name[256];
		strcpy(name, mesh.name.c_str());
		if (ImGui::InputText("Mesh Name", name, 256)) 
			mesh.name = name;

		Material& selectedMaterial = fighterModel->GetMaterials()[materialIndex];
		if (ImGui::Combo("Material", &materialIndex, materialGetter, (void *)&fighterModel->GetMaterials(), int(fighterModel->GetMaterials().size())))
			mesh.materialIndex = materialIndex;
	}

	if (ImGui::CollapsingHeader("Materials", true))
	{
		ImGui::ListBox("Materials", &materialIndex, materialGetter, (void*)&fighterModel->GetMaterials(), int(fighterModel->GetMaterials().size()), 8);
		Material& material = fighterModel->GetMaterials()[materialIndex];
		char name[256];
		strcpy(name, material.name.c_str());
		if (ImGui::InputText("Material Name", name, 256)) 
			material.name = name;
		ImGui::ColorEdit3("Color", &material.albedo.x);
		ImGui::SliderFloat("Reflectivity", &material.reflectivity, 0.0f, 1.0f);
		ImGui::SliderFloat("Metalness", &material.metalness, 0.0f, 1.0f);
		ImGui::SliderFloat("Fresnel", &material.fresnel, 0.0f, 1.0f);
		ImGui::SliderFloat("shininess", &material.shininess, 0.0f, 25000.0f);
		ImGui::SliderFloat("Emission", &material.emission, 0.0f, 10.0f);
	}

	ImGui::End();
}


void InitTiledForwardRendering()
{
	// Textures
	std::shared_ptr<GLTexture2D> viewSpacePositionTexture = std::make_shared<GLTexture2D>();
	viewSpacePositionTexture->Load(GL_RGB32F, nullptr, g_WindowWidth, g_WindowHeight, GL_RGB, GL_UNSIGNED_BYTE);
	viewSpacePositionTexture->SetMinMagFilter(GL_NEAREST);
	viewSpacePositionTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	std::shared_ptr<GLTexture2D> viewSpaceNormalTexture = std::make_shared<GLTexture2D>();
	viewSpaceNormalTexture->Load(GL_RGB32F, nullptr, g_WindowWidth, g_WindowHeight, GL_RGB, GL_UNSIGNED_BYTE);
	viewSpaceNormalTexture->SetMinMagFilter(GL_NEAREST);
	viewSpaceNormalTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	std::shared_ptr<GLTexture2D> clipSpaceVelocityTexture = std::make_shared<GLTexture2D>();
	clipSpaceVelocityTexture->Load(GL_RG32F, nullptr, g_WindowWidth, g_WindowHeight, GL_RG, GL_UNSIGNED_BYTE);
	clipSpaceVelocityTexture->SetMinMagFilter(GL_NEAREST);
	clipSpaceVelocityTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	std::shared_ptr<GLTexture2D> lightingPassColorTexture = std::make_shared<GLTexture2D>();
	lightingPassColorTexture->Load(GL_RGB32F, nullptr, g_WindowWidth, g_WindowHeight, GL_RGB, GL_UNSIGNED_BYTE);
	lightingPassColorTexture->SetMinMagFilter(GL_NEAREST);
	lightingPassColorTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	std::shared_ptr<GLTexture2D> ssaoTexture = std::make_shared<GLTexture2D>();
	ssaoTexture->Load(GL_R32F, nullptr, g_WindowWidth, g_WindowHeight, GL_RED, GL_UNSIGNED_BYTE);
	ssaoTexture->SetMinMagFilter(GL_NEAREST);
	ssaoTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	// SSBOs
	std::shared_ptr<GLShaderStorageBuffer> lightSSBO = std::make_shared<GLShaderStorageBuffer>(nullptr, sizeof(g_GlobalLight));
	std::shared_ptr<GLShaderStorageBuffer> lightIndexSSBO = std::make_shared<GLShaderStorageBuffer>(nullptr, BIT(24));
	std::shared_ptr<GLShaderStorageBuffer> tileIndexSSBO = std::make_shared<GLShaderStorageBuffer>(nullptr, g_NumTileCols * g_NumTileRows * 2 * sizeof(int));

	// Shaders
	std::shared_ptr<GLShader> prepassShader = std::make_shared<GLShader>();
	prepassShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/prepass_vs.glsl");
	prepassShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/prepass_fs.glsl");
	prepassShader->CompileShaders();

	std::shared_ptr<GLShader> ssaoShader = std::make_shared<GLShader>();
	ssaoShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/ssao_vs.glsl");
	ssaoShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/ssao_fs.glsl");
	ssaoShader->CompileShaders();

	std::shared_ptr<GLShader> backgroundShader = std::make_shared<GLShader>();
	backgroundShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/background_vs.glsl");
	backgroundShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/background_fs.glsl");
	backgroundShader->CompileShaders();

	std::shared_ptr<GLShader> computeLightTilesShader = std::make_shared<GLShader>();
	computeLightTilesShader->AddShaderFromFile(GL_COMPUTE_SHADER, "res/shaders/compute_light_tiles_cs.glsl");
	computeLightTilesShader->CompileShaders();

	std::shared_ptr<GLShader> lightingPassShader = std::make_shared<GLShader>();
	lightingPassShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/lighting_pass_vs.glsl");
	lightingPassShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/lighting_pass_fs.glsl");
	lightingPassShader->CompileShaders();
	lightingPassShader->SetUniform4f("u_Light.viewSpacePosition", renderer.camera.GetViewMatrix() * vec4(10.0f));
	lightingPassShader->SetUniform4f("u_Light.color", vec4(1000.0f));

	std::shared_ptr<GLShader> depthShader = std::make_shared<GLShader>();
	depthShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/depth_vs.glsl");
	depthShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/depth_fs.glsl");
	depthShader->CompileShaders();
	depthShader->SetUniformMat4("u_ProjectionMatrix", renderer.camera.projectionMatrix);

	std::shared_ptr<GLShader> fullscreenShader = std::make_shared<GLShader>();
	fullscreenShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/fullscreen_vs.glsl");
	fullscreenShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/fullscreen_fs.glsl");
	fullscreenShader->CompileShaders();

	Material material;
	material.albedo = vec4(1.0f);
	material.Bind(*lightingPassShader);

	// Render passes
	std::shared_ptr<Prepass> prepass = std::make_shared<Prepass>(
		renderer, prepassShader,
		viewSpacePositionTexture,
		viewSpaceNormalTexture,
		clipSpaceVelocityTexture);
	std::shared_ptr<SSAOPass> ssaoPass = std::make_shared<SSAOPass>(
		renderer, ssaoShader,
		viewSpacePositionTexture,
		viewSpaceNormalTexture,
		ssaoTexture);
	std::shared_ptr<ComputeLightTilesPass> computeLightTilesPass = std::make_shared<ComputeLightTilesPass>(
		renderer, computeLightTilesShader,
		viewSpacePositionTexture,
		lightSSBO,
		lightIndexSSBO,
		tileIndexSSBO);
	std::shared_ptr<LightingPass> lightingPass = std::make_shared<LightingPass>(
		renderer, lightingPassShader,
		prepass->GetDepthbuffer(),
		lightingPassColorTexture,
		irradianceMap,
		reflectionMap,
		ssaoTexture,
		lightSSBO,
		lightIndexSSBO,
		tileIndexSSBO);
	std::shared_ptr<BackgroundPass> backgroundPass = std::make_shared<BackgroundPass>(
		renderer, backgroundShader,
		lightingPass->GetFramebuffer(),
		environmentMap);
	std::shared_ptr<OutputSelectionPass> outputSelectionPass = std::make_shared<OutputSelectionPass>(
		renderer, std::make_shared<GLShader>(),
		outputSelection,
		depthShader,
		fullscreenShader,
		viewSpacePositionTexture,
		viewSpaceNormalTexture,
		ssaoTexture,
		lightingPassColorTexture);

	// Render technique
	renderTechnique = std::make_shared<RenderTechnique>();
	renderTechnique->AddRenderPass(prepass);
	renderTechnique->AddRenderPass(ssaoPass);
	renderTechnique->AddRenderPass(computeLightTilesPass);
	renderTechnique->AddRenderPass(backgroundPass);
	renderTechnique->AddRenderPass(lightingPass);
	renderTechnique->AddRenderPass(outputSelectionPass);
}