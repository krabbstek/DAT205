#include "Globals.h"
#include "graphics/Light.h"
#include "graphics/Material.h"
#include "graphics/Renderer.h"
#include "graphics/RenderTechnique.h"
#include "graphics/opengl/OpenGL.h"
#include "graphics/renderpasses/BackgroundPass.h"
#include "graphics/renderpasses/BloomPass.h"
#include "graphics/renderpasses/Blur2DPass.h"
#include "graphics/renderpasses/ComputeLightTilesPass.h"
#include "graphics/renderpasses/LightingPass.h"
#include "graphics/renderpasses/MotionBlurPass.h"
#include "graphics/renderpasses/OutputSelectionPass.h"
#include "graphics/renderpasses/Prepass.h"
#include "graphics/renderpasses/SSAOPass.h"
#include "graphics/renderpasses/StartTimerPass.h"
#include "graphics/renderpasses/StopTimerPass.h"
#include "graphics/renderpasses/PlotTimersPass.h"
#include "math/math.h"
#include "meshes/TessellationCube.h"
#include "meshes/PlaneMesh.h"
#include "model/Model.h"
#include "particle/GlowingParticleSystem.h"

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
std::shared_ptr<TessellationCube> tessellationCube;

std::shared_ptr<GlowingParticleSystem> glowingParticleSystem;

std::shared_ptr<GLShader> prepassShader;
std::shared_ptr<GLShader> ssaoShader;
std::shared_ptr<GLShader> ssaoBilateralBlurShader;
std::shared_ptr<GLShader> backgroundShader;
std::shared_ptr<GLShader> computeLightTilesShader;
std::shared_ptr<GLShader> lightingPassShader;
std::shared_ptr<GLShader> glowingParticleShader;
std::shared_ptr<GLShader> depthShader;
std::shared_ptr<GLShader> fullscreenShader;
std::shared_ptr<GLShader> blur1DShader;
std::shared_ptr<GLShader> motionBlurShader;
std::shared_ptr<GLShader> bloomShader;
std::shared_ptr<GLShader> lightTilesOverlayShader;
std::shared_ptr<GLShader> cubeTessellationShader;

std::shared_ptr<GLShaderStorageBuffer> lightSSBO;
std::shared_ptr<GLShaderStorageBuffer> lightIndexSSBO;
std::shared_ptr<GLShaderStorageBuffer> tileIndexSSBO;

std::shared_ptr<GLTexture2D> environmentMap;
std::shared_ptr<GLTexture2D> irradianceMap;
std::shared_ptr<GLTexture2D> reflectionMap;
std::shared_ptr<GLTexture2D> fireSphereTexture;

OUTPUT_SELECTION outputSelection = OUTPUT_SELECTION_LIGHTING;

int Init();
void InitTiledForwardRendering();
void LoadShaders();
void UpdateLights();
void HandleKeyInput(float deltaTime);
void MouseButtonCallback(GLFWwindow* window, int key, int action, int mods);
void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
void ImGuiRender();
void AddGLTimedRenderPass(std::shared_ptr<RenderTechnique> renderTechnique, std::shared_ptr<RenderPass> renderPass, std::shared_ptr<PlotTimersPass> plotTimersPass, const char* renderPassName);
void Update();
void Render();

int main()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> prevTime, currentTime;
	prevTime = std::chrono::high_resolution_clock::now();
	float accumulator = 0.0f;

	int initResult = Init();
	if (initResult)
		return initResult;

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glCullFace(GL_BACK));
	GLCall(glDepthFunc(GL_LEQUAL));
	
	{
		renderer.camera.projectionMatrix = mat4::Perspective(g_FOV, g_AspectRatio, g_NearPlaneDepth, g_FarPlaneDepth);
		renderer.camera.position = vec3(0.0f, 10.0f, 20.0f);
		renderer.camera.rotation = vec3(-0.4f, 0.0f, 0.0f);
		mat4 V = renderer.camera.GetViewMatrix();

		{
			int width, height, numChannels;
			float* data;

			stbi_set_flip_vertically_on_load(false);

			environmentMap = std::make_shared<GLTexture2D>();
			data = stbi_loadf("res/textures/envmaps/001.hdr", &width, &height, &numChannels, 3);
			environmentMap->Load(GL_RGB16F, data, width, height, GL_RGB, GL_FLOAT, 0);
			environmentMap->SetWrapST(GL_REPEAT);
			environmentMap->SetMagFilter(GL_LINEAR);
			environmentMap->SetMinFilter(GL_LINEAR);
			stbi_image_free(data);
			
			irradianceMap = std::make_shared<GLTexture2D>();
			irradianceMap->SetWrapST(GL_REPEAT);
			irradianceMap->SetMagFilter(GL_LINEAR);
			irradianceMap->SetMinFilter(GL_LINEAR);
			data = stbi_loadf("res/textures/envmaps/001_irradiance.hdr", &width, &height, &numChannels, 0);
			irradianceMap->Load(GL_RGB16F, data, width, height, GL_RGB, GL_FLOAT, 0);
			stbi_image_free(data);
		
			reflectionMap = std::make_shared<GLTexture2D>();
			reflectionMap->SetWrapST(GL_REPEAT);
			reflectionMap->SetMagFilter(GL_LINEAR);
			reflectionMap->SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
			reflectionMap->Bind();
			data = stbi_loadf("res/textures/envmaps/001_dl_0.hdr", &width, &height, &numChannels, 0);
			reflectionMap->Load(GL_RGB16F, data, width, height, GL_RGB, GL_FLOAT, 0);
			stbi_image_free(data);

			GLCall(glGenerateMipmap(GL_TEXTURE_2D));
			{
				char filename[256];
				for (int i = 1; i < 8; i++)
				{
					sprintf(filename, "res/textures/envmaps/001_dl_%d.hdr", i);
					float* data = stbi_loadf(filename, &width, &height, &numChannels, 0);
					reflectionMap->Load(GL_RGB16F, data, width, height, GL_RGB, GL_FLOAT, i);
					stbi_image_free(data);
				}
			}

			stbi_set_flip_vertically_on_load(true);
		}

		/// Render technique
		InitTiledForwardRendering();

		fighterModel = std::shared_ptr<Model>(Model::LoadModelFromOBJ("res/models/NewShip.obj", prepassShader, lightingPassShader));
		fighterModel->modelMatrix = mat4(1.0f);

		tessellationCube = std::make_shared<TessellationCube>(cubeTessellationShader, cubeTessellationShader);

		fireSphereTexture = std::make_shared<GLTexture2D>();
		fireSphereTexture->LoadFromFile("res/textures/FireSphere.png");
		fireSphereTexture->SetWrapST(GL_CLAMP_TO_EDGE);
		fireSphereTexture->SetMinMagFilter(GL_LINEAR);
		fireSphereTexture->Bind();
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));

		glowingParticleSystem = std::make_shared<GlowingParticleSystem>(10000, glowingParticleShader);
		glowingParticleSystem->AddTexture(fireSphereTexture);

		while (!glfwWindowShouldClose(window))
		{
			currentTime = std::chrono::high_resolution_clock::now();
			accumulator += float((currentTime - prevTime).count()) * 1e-9f;
			g_Time += float((currentTime - prevTime).count()) * 1e-9f;
			prevTime = currentTime;

			if (accumulator >= 0.1f)
				accumulator = 0.1f;

			while (accumulator >= g_DeltaTime)
			{
				accumulator -= g_DeltaTime;

				glfwPollEvents();

				Update();
			}

			Render();

			glfwSwapBuffers(window);
		}
	}

	fighterModel.reset();
	tessellationCube.reset();
	glowingParticleSystem.reset();

	renderTechnique.reset();

	environmentMap.reset();
	irradianceMap.reset();
	reflectionMap.reset();
	fireSphereTexture.reset();

	lightSSBO.reset();
	lightIndexSSBO.reset();
	tileIndexSSBO.reset();

	prepassShader.reset();
	ssaoShader.reset();
	ssaoBilateralBlurShader.reset();
	backgroundShader.reset();
	computeLightTilesShader.reset();
	lightingPassShader.reset();
	glowingParticleShader.reset();
	depthShader.reset();
	fullscreenShader.reset();
	blur1DShader.reset();
	motionBlurShader.reset();
	bloomShader.reset();
	lightTilesOverlayShader.reset();
	cubeTessellationShader.reset();

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

	GLCall(std::printf("%s\n", glGetString(GL_VERSION)));

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


void UpdateLights()
{
	int numParticles = glowingParticleSystem->NumParticles();
	int size = numParticles + 1;
	Light* lights = new Light[size];

	mat4 V = renderer.camera.GetViewMatrix();

	for (int i = 0; i < numParticles; i++)
	{
		Particle& particle = glowingParticleSystem->GetParticle(i);
		lights[i].viewSpacePosition.xyz = V * particle.position;
		lights[i].viewSpacePosition.w = 1.0f;
		lights[i].color.rgb = g_GlowingParticleLightIntensityMultiplier * particle.emission * std::min(1.0f, 2.0f * (0.5f - abs(particle.lifetime / particle.lifelength - 0.5f)));
		float colorIntensityMultiplier = std::max(particle.emission.r, std::max(particle.emission.g, particle.emission.b));
		lights[i].color.a = g_LightFalloffThreshold / (g_GlowingParticleLightIntensityMultiplier * colorIntensityMultiplier);
	}

	lights[numParticles] = g_GlobalLight;

	if (lightSSBO->Size() < size * sizeof(Light))
		lightSSBO->SetData(lights, size * sizeof(Light));
	else
		lightSSBO->SetSubData(lights, 0, size * sizeof(Light));
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
	ImGui::RadioButton("Motion blur", (int*)&outputSelection, OUTPUT_SELECTION_MOTION_BLUR);
	ImGui::RadioButton("Bloom", (int*)&outputSelection, OUTPUT_SELECTION_BLOOM);
	ImGui::Checkbox("Light tiles overlay", &g_DisplayLightTilesOverlay);

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
	g_GlobalLight.color.a = g_LightFalloffThreshold / globalLightIntensity;
	g_GlobalLight.viewSpacePosition = renderer.camera.GetViewMatrix() * globalLightPosition;

	ImGui::Separator();

	ImGui::Text("Number of particles in scene: %d", glowingParticleSystem->NumParticles());
	ImGui::Text("Glowing particles");
	ImGui::SliderFloat("Particle light intensity multiplier", &g_GlowingParticleLightIntensityMultiplier, 1.0f, 1000.0f, "%.1f", 3.0f);
	ImGui::SliderFloat("Light falloff threshold", &g_LightFalloffThreshold, 0.001f, 0.1f, "%.4f", 3.0f);

	ImGui::Separator();

	ImGui::Text("SSAO");
	ImGui::SliderFloat("SSAO bias", &g_SSAOBias, 0.01f, 1.0f, "%.3f", 2.0f);
	ImGui::SliderFloat("SSAO radius", &g_SSAORadius, 0.1f, 10.0f, "%.2f", 2.0f);
	ImGui::SliderInt("Bilateral blur sample size", &g_SSAOBlurSampleSize, 0, 20);
	ImGui::SliderFloat("SSAO blur sigma", &g_SSAOBilateralBlurSigma, 1.0f, 100.0f, "%.1f", 2.0f);
	ImGui::SliderFloat("SSAO depth sigma", &g_SSAODepthSigma, 0.01f, 1.0f);

	ImGui::Separator();

	ImGui::Text("Motion blur");
	ImGui::SliderFloat("Velocity scale", &g_MotionBlurVelocityScale, 0.01f, 100.0f, "%.3f", 3.0f);

	ImGui::Separator();

	ImGui::Text("Bloom");
	ImGui::SliderFloat("Bloom alpha", &g_BloomAlpha, 0.0f, 1.0f);
	ImGui::SliderFloat("Bloom threshold", &g_BloomThreshold, 1.0f, 100.0f, "%.2f", 3.0f);

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

	ImGui::Separator();

	if (ImGui::Button("Reload shaders", ImVec2(120.0f, 20.0f)))
		LoadShaders();

	ImGui::End();
}


void LoadShaders()
{
	prepassShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/prepass_vs.glsl");
	prepassShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/prepass_fs.glsl");
	prepassShader->CompileShaders();

	constexpr int numSamples = 64;
	vec3 s[numSamples];
	for (int i = 0; i < sizeof(s) / sizeof(vec3); i++)
		s[i] = CosineSampleHemisphere() * RandF();
	ssaoShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/ssao_vs.glsl");
	ssaoShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/ssao_fs.glsl");
	ssaoShader->CompileShaders();
	ssaoShader->SetUniform3fv("u_Samples", s, numSamples);
	ssaoShader->SetUniform1i("u_NumSamples", numSamples);
	ssaoShader->SetUniform2f("u_ViewportSize", vec2(float(g_WindowWidth), float(g_WindowHeight)));
	ssaoShader->SetUniformMat4("u_ProjMatrix", renderer.camera.projectionMatrix);
	ssaoShader->SetUniformMat4("u_InverseProjMatrix", mat4::Inverse(renderer.camera.projectionMatrix));

	ssaoBilateralBlurShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/ssao_bilateral_blur_vs.glsl");
	ssaoBilateralBlurShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/ssao_bilateral_blur_fs.glsl");
	ssaoBilateralBlurShader->CompileShaders();
	ssaoBilateralBlurShader->SetUniform2f("u_InvViewportSize", 1.0f / float(g_WindowWidth), 1.0f / float(g_WindowHeight));

	backgroundShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/background_vs.glsl");
	backgroundShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/background_fs.glsl");
	backgroundShader->CompileShaders();

	computeLightTilesShader->AddShaderFromFile(GL_COMPUTE_SHADER, "res/shaders/compute_light_tiles_cs.glsl");
	computeLightTilesShader->CompileShaders();

	lightingPassShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/lighting_pass_vs.glsl");
	lightingPassShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/lighting_pass_fs.glsl");
	lightingPassShader->CompileShaders();
	lightingPassShader->SetUniform4f("u_Light.viewSpacePosition", renderer.camera.GetViewMatrix() * vec4(10.0f));
	lightingPassShader->SetUniform4f("u_Light.color", vec4(1000.0f));

	glowingParticleShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/glowing_particle_vs.glsl");
	glowingParticleShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/glowing_particle_fs.glsl");
	glowingParticleShader->CompileShaders();

	depthShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/depth_vs.glsl");
	depthShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/depth_fs.glsl");
	depthShader->CompileShaders();
	depthShader->SetUniformMat4("u_ProjectionMatrix", renderer.camera.projectionMatrix);

	fullscreenShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/fullscreen_vs.glsl");
	fullscreenShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/fullscreen_fs.glsl");
	fullscreenShader->CompileShaders();

	constexpr int numWeights = 7;
	float weights[numWeights];
	weights[0] = 1.0f;
	float totalWeight = 1.0f;
	float sigma = 3.0f;
	for (int i = 1; i < numWeights; i++)
	{
		weights[i] = expf(-i * i / (2 * sigma * sigma));
		totalWeight += 2.0f * weights[i];
	}
	for (int i = 0; i < numWeights; i++)
		weights[i] /= totalWeight;
	blur1DShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/blur1d_vs.glsl");
	blur1DShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/blur1d_fs.glsl");
	blur1DShader->CompileShaders();
	blur1DShader->SetUniform2f("u_ViewportSize", float(g_WindowWidth), float(g_WindowHeight));
	blur1DShader->SetUniform1fv("u_Weights", weights, numWeights);
	blur1DShader->SetUniform1i("u_NumSamples", numWeights);

	motionBlurShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/motion_blur_vs.glsl");
	motionBlurShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/motion_blur_fs.glsl");
	motionBlurShader->CompileShaders();

	bloomShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/bloom_vs.glsl");
	bloomShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/bloom_fs.glsl");
	bloomShader->CompileShaders();

	lightTilesOverlayShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/light_tiles_overlay_vs.glsl");
	lightTilesOverlayShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/light_tiles_overlay_fs.glsl");
	lightTilesOverlayShader->CompileShaders();
	lightTilesOverlayShader->SetUniform1i("u_TileSize", g_TileSize);
	lightTilesOverlayShader->SetUniform1i("u_NumTileCols", g_NumTileCols);
	lightTilesOverlayShader->SetUniform1f("u_MaxNumLightsPerTile", g_MaxNumLightsPerTile);

	cubeTessellationShader->AddShaderFromFile(GL_VERTEX_SHADER, "res/shaders/cube_tessellation_vs.glsl");
	cubeTessellationShader->AddShaderFromFile(GL_TESS_CONTROL_SHADER, "res/shaders/cube_tessellation_tcs.glsl");
	cubeTessellationShader->AddShaderFromFile(GL_TESS_EVALUATION_SHADER, "res/shaders/cube_tessellation_tes.glsl");
	cubeTessellationShader->AddShaderFromFile(GL_FRAGMENT_SHADER, "res/shaders/cube_tessellation_fs.glsl");
	cubeTessellationShader->CompileShaders();
	cubeTessellationShader->SetUniformMat4("u_ProjMatrix", renderer.camera.projectionMatrix);
	cubeTessellationShader->SetUniformMat4("u_ModelMatrix", mat4::Translate(0.0f, 10.0f, 0.0f));
	cubeTessellationShader->SetUniformMat4("u_ModelMatrix_normal", mat4::Transpose(mat4::Inverse(mat4::Translate(0.0f, 10.0f, 0.0f))));

}

void InitTiledForwardRendering()
{
	GLCall(glPatchParameteri(GL_PATCH_VERTICES, 4));

	// Textures
	std::shared_ptr<GLTexture2D> viewSpacePositionTexture = std::make_shared<GLTexture2D>();
	viewSpacePositionTexture->Load(GL_RGB16F, nullptr, g_WindowWidth, g_WindowHeight, GL_RGB, GL_UNSIGNED_BYTE);
	viewSpacePositionTexture->SetMinMagFilter(GL_NEAREST);
	viewSpacePositionTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	std::shared_ptr<GLTexture2D> viewSpaceNormalTexture = std::make_shared<GLTexture2D>();
	viewSpaceNormalTexture->Load(GL_RGB16F, nullptr, g_WindowWidth, g_WindowHeight, GL_RGB, GL_UNSIGNED_BYTE);
	viewSpaceNormalTexture->SetMinMagFilter(GL_NEAREST);
	viewSpaceNormalTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	std::shared_ptr<GLTexture2D> clipSpaceVelocityTexture = std::make_shared<GLTexture2D>();
	clipSpaceVelocityTexture->Load(GL_RG16F, nullptr, g_WindowWidth, g_WindowHeight, GL_RG, GL_UNSIGNED_BYTE);
	clipSpaceVelocityTexture->SetMinMagFilter(GL_NEAREST);
	clipSpaceVelocityTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	std::shared_ptr<GLTexture2D> lightingPassColorTexture = std::make_shared<GLTexture2D>();
	lightingPassColorTexture->Load(GL_RGB16F, nullptr, g_WindowWidth, g_WindowHeight, GL_RGB, GL_UNSIGNED_BYTE);
	lightingPassColorTexture->SetMinMagFilter(GL_LINEAR);
	lightingPassColorTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	std::shared_ptr<GLTexture2D> ssaoTexture = std::make_shared<GLTexture2D>();
	ssaoTexture->Load(GL_R16F, nullptr, g_WindowWidth, g_WindowHeight, GL_RED, GL_UNSIGNED_BYTE);
	ssaoTexture->SetMinMagFilter(GL_NEAREST);
	ssaoTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	std::shared_ptr<GLTexture2D> motionBlurredTexture = std::make_shared<GLTexture2D>();
	motionBlurredTexture->Load(GL_RGB16F, nullptr, g_WindowWidth, g_WindowHeight, GL_RGB, GL_UNSIGNED_BYTE);
	motionBlurredTexture->SetMinMagFilter(GL_NEAREST);
	motionBlurredTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	std::shared_ptr<GLTexture2D> bloomInputTexture = std::make_shared<GLTexture2D>();
	bloomInputTexture->Load(GL_RGB16F, nullptr, g_WindowWidth, g_WindowHeight, GL_RGB, GL_UNSIGNED_BYTE);
	bloomInputTexture->Bind();
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	bloomInputTexture->SetMagFilter(GL_LINEAR);
	bloomInputTexture->SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	bloomInputTexture->SetWrapST(GL_CLAMP_TO_BORDER);

	std::shared_ptr<GLTexture2D> bloomOutputTexture = std::make_shared<GLTexture2D>();
	bloomOutputTexture->Load(GL_RGB16F, nullptr, g_WindowWidth, g_WindowHeight, GL_RGB, GL_UNSIGNED_BYTE);
	bloomOutputTexture->SetMinMagFilter(GL_LINEAR);
	bloomOutputTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	// SSBOs
	lightSSBO = std::make_shared<GLShaderStorageBuffer>(nullptr, sizeof(g_GlobalLight));
	lightIndexSSBO = std::make_shared<GLShaderStorageBuffer>(nullptr, BIT(24));
	tileIndexSSBO = std::make_shared<GLShaderStorageBuffer>(nullptr, g_NumTileCols * g_NumTileRows * 2 * sizeof(int));

	// Shaders
	prepassShader = std::make_shared<GLShader>();
	ssaoShader = std::make_shared<GLShader>();
	ssaoBilateralBlurShader = std::make_shared<GLShader>();
	backgroundShader = std::make_shared<GLShader>();
	computeLightTilesShader = std::make_shared<GLShader>();
	lightingPassShader = std::make_shared<GLShader>();
	glowingParticleShader = std::make_shared<GLShader>();
	depthShader = std::make_shared<GLShader>();
	fullscreenShader = std::make_shared<GLShader>();
	blur1DShader = std::make_shared<GLShader>();
	motionBlurShader = std::make_shared<GLShader>();
	bloomShader = std::make_shared<GLShader>();
	lightTilesOverlayShader = std::make_shared<GLShader>();
	cubeTessellationShader = std::make_shared<GLShader>();
	LoadShaders();

	// Render passes
	std::shared_ptr<PlotTimersPass> plotTimersPass = std::make_shared<PlotTimersPass>(renderer);
	std::shared_ptr<GLTimer> totalRenderTimer = std::make_shared<GLTimer>();
	plotTimersPass->AddLargeTimer("Total render time", totalRenderTimer);
	std::shared_ptr<StartTimerPass> startTotalRenderTimePass = std::make_shared<StartTimerPass>(renderer, totalRenderTimer);
	std::shared_ptr<StopTimerPass> stopTotalRenderTimePass = std::make_shared<StopTimerPass>(renderer, totalRenderTimer);
	std::shared_ptr<Prepass> prepass = std::make_shared<Prepass>(
		renderer,
		viewSpacePositionTexture,
		viewSpaceNormalTexture,
		clipSpaceVelocityTexture);
	std::shared_ptr<SSAOPass> ssaoPass = std::make_shared<SSAOPass>(
		renderer,
		ssaoShader,
		ssaoBilateralBlurShader,
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
		renderer,
		lightingPassShader,
		prepass->GetDepthbuffer(),
		lightingPassColorTexture,
		irradianceMap,
		reflectionMap,
		ssaoTexture,
		bloomInputTexture,
		lightSSBO,
		lightIndexSSBO,
		tileIndexSSBO);
	std::shared_ptr<BackgroundPass> backgroundPass = std::make_shared<BackgroundPass>(
		renderer, backgroundShader,
		lightingPass->GetFramebuffer(),
		environmentMap);
	std::shared_ptr<MotionBlurPass> motionBlurPass = std::make_shared<MotionBlurPass>(
		renderer, motionBlurShader,
		lightingPassColorTexture,
		clipSpaceVelocityTexture,
		motionBlurredTexture);
	std::shared_ptr<BloomPass> bloomPass = std::make_shared<BloomPass>(
		renderer, bloomShader,
		blur1DShader,
		lightingPassColorTexture,
		bloomInputTexture,
		bloomOutputTexture);
	std::shared_ptr<OutputSelectionPass> outputSelectionPass = std::make_shared<OutputSelectionPass>(
		renderer,
		outputSelection,
		depthShader,
		fullscreenShader,
		lightTilesOverlayShader,
		tileIndexSSBO,
		viewSpacePositionTexture,
		viewSpaceNormalTexture,
		ssaoTexture,
		lightingPassColorTexture,
		motionBlurredTexture,
		bloomOutputTexture);

	// Render technique
	renderTechnique = std::make_shared<RenderTechnique>();
	renderTechnique->AddRenderPass(plotTimersPass);
	renderTechnique->AddRenderPass(startTotalRenderTimePass);
	AddGLTimedRenderPass(renderTechnique, prepass, plotTimersPass, "Prepass");
	AddGLTimedRenderPass(renderTechnique, ssaoPass, plotTimersPass, "SSAO pass");
	AddGLTimedRenderPass(renderTechnique, computeLightTilesPass, plotTimersPass, "Compute light tiles pass");
	AddGLTimedRenderPass(renderTechnique, backgroundPass, plotTimersPass, "Background pass");
	AddGLTimedRenderPass(renderTechnique, lightingPass, plotTimersPass, "Lighting pass");
	AddGLTimedRenderPass(renderTechnique, motionBlurPass, plotTimersPass, "Motion blur pass");
	AddGLTimedRenderPass(renderTechnique, bloomPass, plotTimersPass, "Bloom pass");
	AddGLTimedRenderPass(renderTechnique, outputSelectionPass, plotTimersPass, "Output selection pass");
	renderTechnique->AddRenderPass(stopTotalRenderTimePass);
}


void AddGLTimedRenderPass(std::shared_ptr<RenderTechnique> renderTechnique, std::shared_ptr<RenderPass> renderPass, std::shared_ptr<PlotTimersPass> plotTimersPass, const char* renderPassName)
{
	std::shared_ptr<GLTimer> timer = std::make_shared<GLTimer>();

	std::shared_ptr<StartTimerPass> startTimerPass = std::make_shared<StartTimerPass>(renderer, timer);
	std::shared_ptr<StopTimerPass> stopTimerPass = std::make_shared<StopTimerPass>(renderer, timer);

	renderTechnique->AddRenderPass(startTimerPass);
	renderTechnique->AddRenderPass(renderPass);
	renderTechnique->AddRenderPass(stopTimerPass);

	plotTimersPass->AddSmallTimer(renderPassName, timer);
}


void Update()
{
	HandleKeyInput(g_DeltaTime);

	glowingParticleSystem->UpdateParticles(g_DeltaTime);
	
	for (int i = 0; i < 2; i++)
	{
		Particle particle;
		particle.position = 40.0f * (vec3(RandF(), RandF(), RandF()) - 0.5f);
		particle.velocity = 4.0f * (vec3(RandF(), RandF(), RandF()) - 0.5f);
		particle.emission = vec3(RandF(), RandF(), RandF());
		particle.lifelength = 2.0f;
		particle.scale = 0.1f * RandF();
		glowingParticleSystem->SpawnParticle(particle);
	}
}

void Render()
{
#ifdef USE_IMGUI
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
#endif

	UpdateLights();
	renderTechnique->Render(*fighterModel);
	renderTechnique->Render(*tessellationCube);
	renderTechnique->Render(*glowingParticleSystem);
	renderTechnique->Render();

	renderer.camera.Update();
	fighterModel->Update();

#ifdef USE_IMGUI
	ImGuiRender();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}