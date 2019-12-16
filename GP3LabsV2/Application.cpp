#include "pch.h"
#include "Application.h"
#include "Common.h"
#include "Log.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Resources.h"
#include "Physics.h"
#include "BoxShape.h"
#include "CapsuleShape.h"
#include "ConeShape.h"
#include "CylinderShape.h"
#include "SphereShape.h"
#include "RigidBody.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>



Application* Application::m_application = nullptr;
RigidBody* r = new RigidBody();
BoxPush* bp = new BoxPush();
ImVec4 lightColour1 = ImVec4(0.95f, 0.05f, 0.05f, 1.00f);
ImVec4 lightColour2 = ImVec4(0.05f, 0.95f, 0.05f, 1.00f);
ImVec4 lightColour3 = ImVec4(0.05f, 0.05f, 0.95f, 1.00f);

Application::Application()
{
}
void Application::Init()
{
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError(), logType::ERROR);
		exit(-1);
	}

	printf("%i joysticks were found.\n\n", SDL_NumJoysticks());
	printf("The names of the joysticks are:\n");

	joystick = nullptr;
	gamepadController = nullptr;
	gamepadHaptic = nullptr;

	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		joystick = SDL_JoystickOpen(i);
		gamepadHaptic = SDL_HapticOpenFromJoystick(joystick);
		SDL_HapticRumbleInit(gamepadHaptic);
		break;
	}

	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		gamepadController = SDL_GameControllerOpen(i);
		break;
	}

	SDL_JoystickEventState(SDL_ENABLE);
	
	joystick = SDL_JoystickOpen(0);



	//setting openGL version to 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	//creating window
	m_window = SDL_CreateWindow("GP3-GAME", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight,
		SDL_WINDOW_OPENGL);
	SDL_CaptureMouse(SDL_TRUE);
	OpenGlInit();

	
	//GameInit();

}
void Application::OpenGlInit()
{
	//creating context (our opengl statemachine in which all our GL calls
	//will refer to)
	m_glContext = SDL_GL_CreateContext(m_window);

	ImGui::CreateContext();
	ImGui_ImplSdlGL3_Init(m_window);
	ImGui::StyleColorsLight();

	CHECK_GL_ERROR();
	SDL_GL_SetSwapInterval(1);
	//initialsing glew (do this after establishing context!)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) <<
			std::endl;
		exit(-1);
	}

	//Smooth shading
	GL_ATTEMPT(glShadeModel(GL_SMOOTH));
	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));
	//set less or equal func for depth testing
	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));
	//enabling blending
	glEnable(GL_BLEND);
	GL_ATTEMPT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//turn on back face culling
	GL_ATTEMPT(glEnable(GL_CULL_FACE));
	GL_ATTEMPT(glEnable(GL_FRAMEBUFFER_SRGB));
	GL_ATTEMPT(glCullFace(GL_BACK));

	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);
}

void Application::GameInit()
{
	//loading all resources
	Resources::GetInstance()->AddModel("cube.obj");
	Resources::GetInstance()->AddModel("sphere.obj");
	Resources::GetInstance()->AddModel("Cylinder.obj");
	Resources::GetInstance()->AddModel("monkey3.obj");
	Resources::GetInstance()->AddModel("TrafficCone.obj");
	Resources::GetInstance()->AddModel("SpinningTop.obj");
	Resources::GetInstance()->AddModel("Skull.obj");

	Resources::GetInstance()->AddTexture("Wood.jpg");
	Resources::GetInstance()->AddTexture("FloorTexture.jpg");
	Resources::GetInstance()->AddTexture("BeachBallTexture.jpg");
	Resources::GetInstance()->AddTexture("Red.png");
	Resources::GetInstance()->AddTexture("MonkeyTexture.png");
	Resources::GetInstance()->AddTexture("SpinningTopTexture.jpg");
	Resources::GetInstance()->AddTexture("BoneTexture.jpg");
	Resources::GetInstance()->AddTexture("TrafficConeTexture.jpg");
	Resources::GetInstance()->AddTexture("CylinderTexture.jpg");

	Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>("assets/simpleVert.glsl", "assets/simpleFrag.glsl"), "simple");
	Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>("assets/simple_VERT.glsl", "assets/simple_FRAG.glsl"), "simpler");

	static float skullX = 10.0f;
	static float skullY = 5.0f;
	static float skullZ = -40.f;
	Entity* a = new Entity();	
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("FloorTexture.jpg"))
	);
	MeshRenderer* m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(0, 0.f, 0.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 1.f,
		100.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(100.f, 1.f, 100.f));

	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("FloorTexture.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(0, 100.f, 0.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 1.f,
		100.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());

	a->GetTransform()->SetScale(glm::vec3(100.f, 1.f, 100.f));

	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("FloorTexture.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(0.f, 50.f, -50.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 100.f, 1.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(100.f, 100.f, 1.f));

	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("FloorTexture.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(0.f, 50.f, 50.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 100.f, 1.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(100.f, 100.f, 1.f));

	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("FloorTexture.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(50.f, 50.f, 0.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 100.f, 100.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(1.f, 100.f, 100.f));
	
	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("FloorTexture.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(-50.f, 50.f, 0.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 100.f, 100.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(1.f, 100.f, 100.f));

	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Cylinder.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("CylinderTexture.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(5.f, 10.f, -40.f));
	a->AddComponent<BoxPush>();
	a->GetComponent<BoxPush>()->Init();
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new CylinderShape(glm::vec3(1.0f, 1.0f,1.0f)));
	a->GetTransform()->SetScale(glm::vec3(10.f, 10.f, 10.f));

	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Skull.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("BoneTexture.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(5.0f, 10.0f, -40.0f));
	a->GetTransform()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("monkey3.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("MonkeyTexture.png"))
	);
	a->GetTransform()->SetPosition(glm::vec3(-5.f, 5.f, -40.f));
	a->GetTransform()->SetScale(glm::vec3(5.f, 5.f, 5.f));

	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("sphere.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("BeachBallTexture.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(-10.f, 10.f, -40.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new SphereShape(1.f));
	a->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));

	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("TrafficCone.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("TrafficConeTexture.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(-20.f, 10.f, -40.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new CylinderShape(glm::vec3(1.f,1.f,1.f)));
	a->GetTransform()->SetScale(glm::vec3(5.f, 5.f, 5.f));


	a = new Entity();
	m_entities.push_back(a);
	CameraComp* cc = new CameraComp();
	a->AddComponent(cc);
	a->GetTransform()->SetPosition(glm::vec3(0.f, 10.f, 0.f));
	cc->Start();


	
}

void Application::Loop()
{
	m_appState = AppState::RUNNING;

	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;

	glm::vec3 xAxis;
	glm::vec3 yAxis;
	glm::vec3 zAxis;
	xAxis = glm::vec3(1, 0, 0);
	yAxis = glm::vec3(0, 1, 0);
	zAxis = glm::vec3(0, 0, 1);
	xAxis = glm::normalize(xAxis);
	yAxis = glm::normalize(yAxis);
	zAxis = glm::normalize(zAxis);
	glm::quat rot;
	float rotationA = 5.0f;
	float rotationD = -5.0f;
	glm::quat currentRot;
	int JOYSTICK_DEAD_ZONE = 2000;
	bool ballsSpawned = false;
	bool cubesSpawned = false;

	glm::quat startingRotation = m_entities.at(10)->GetTransform()->GetRotation();

	while (m_appState != AppState::QUITTING)
	{
		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks - prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;


		Resources::GetInstance()->GetShader("simple")->Use();
		Resources::GetInstance()->GetShader("simple")->setVec3("lightPos", glm::vec3(sinf(counter), sinf(counter), sinf(counter)));
		Resources::GetInstance()->GetShader("simple")->setVec3("lightColor", glm::vec3(0.1f, sinf(counter*0.1f), sinf(counter*0.7f)));
		Resources::GetInstance()->GetShader("simple")->setVec3("viewPos", m_mainCamera->GetParentTransform()->GetPosition());
		counter += deltaTime * 0.1f;
	

		ImGui_ImplSdlGL3_NewFrame(m_window);
		//poll SDL events
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				m_appState = AppState::QUITTING;
				break;
			case SDL_KEYDOWN:
				INPUT->GetKeyDown(event.key.keysym.sym);
				INPUT->SetKey(event.key.keysym.sym, true);

				break;
			case SDL_KEYUP:
				INPUT->GetKeyUp(event.key.keysym.sym);
				INPUT->SetKey(event.key.keysym.sym, false);
				break;
			
			case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
				
					if (event.jaxis.axis == 1)
					{
						if (event.jaxis.value < -JOYSTICK_DEAD_ZONE) {
							rot = glm::angleAxis(glm::radians(rotationA), xAxis);
							m_entities.at(11)->GetTransform()->AddRotation((rot));

						}
						else if (event.jaxis.value > JOYSTICK_DEAD_ZONE) {
							
							rot = glm::angleAxis(glm::radians(rotationD), xAxis);
							m_entities.at(11)->GetTransform()->AddRotation((rot));
						}
					}

					if (event.jaxis.axis == 0)
					{
						if (event.jaxis.value < -JOYSTICK_DEAD_ZONE) {
							
							rot = glm::angleAxis(glm::radians(rotationA), yAxis);
							m_entities.at(11)->GetTransform()->AddRotation((rot));
						}
						else if (event.jaxis.value > JOYSTICK_DEAD_ZONE) {
							
							
							rot = glm::angleAxis(glm::radians(rotationD), yAxis);
							m_entities.at(11)->GetTransform()->AddRotation((rot));
						}
					}
				
				break;
		
			}

			switch (event.cbutton.button) {
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				m_entities.at(11)->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, -0.5f));
				SDL_HapticRumblePlay(GetInstance()->gamepadHaptic, 0.4, 50);
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				m_entities.at(11)->GetTransform()->AddPosition(glm::vec3(0.f, 0.f, 0.5f));
				SDL_HapticRumblePlay(GetInstance()->gamepadHaptic, 0.4, 50);
				
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				m_entities.at(11)->GetTransform()->AddPosition(glm::vec3(0.5f, 0.f, 0.f));
				SDL_HapticRumblePlay(GetInstance()->gamepadHaptic, 0.4, 50);
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				m_entities.at(11)->GetTransform()->AddPosition(glm::vec3(-0.5f, 0.f, 0.f));
				SDL_HapticRumblePlay(GetInstance()->gamepadHaptic, 0.4, 50);
				
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
				m_entities.at(11)->GetTransform()->AddPosition(glm::vec3(0.f, 1.f, 0.f));
				SDL_HapticRumblePlay(GetInstance()->gamepadHaptic, 0.4, 50);
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
				m_entities.at(11)->GetTransform()->AddPosition(glm::vec3(0.f, -1.f, 0.f));
				SDL_HapticRumblePlay(GetInstance()->gamepadHaptic, 0.4, 50);
				break;
			case SDL_CONTROLLER_BUTTON_START:
				if (ballsSpawned == false)
				{
					for (int i = 0; i < 10; i++)
					{
						Entity* a = new Entity();
						m_entities.push_back(a);
						a->AddComponent(
							new MeshRenderer(
								Resources::GetInstance()->GetModel("sphere.obj"),
								Resources::GetInstance()->GetShader("simple"),
								Resources::GetInstance()->GetTexture("Red.png"))
						);
						a->GetTransform()->SetPosition(glm::vec3(1.0f * i, 5.f, -20.f));
						a->AddComponent<RigidBody>();
						a->GetComponent<RigidBody>()->Init(new SphereShape(1.f));
						a->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));
					}
					ballsSpawned = true;
				}
				break;

			}
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				Physics::GetInstance()->AddForceUp(deltaTime);
				break;
			case SDLK_d:
				Physics::GetInstance()->AddForceRight(deltaTime);
				break;
			case SDLK_a:
				Physics::GetInstance()->AddForceLeft(deltaTime);
				break;
			case SDLK_s:
				Physics::GetInstance()->AddForceDown(deltaTime);
				break;
			case SDLK_q:
				Physics::GetInstance()->AddJump(deltaTime);
				break;
			case SDLK_e:
				Physics::GetInstance()->AddTorque(deltaTime);
				break;
			}
		}
		Physics::GetInstance()->Update(deltaTime);
		//update and render all entities
		Update(deltaTime);
		Render();
	
		if (ImGui::Button("Jump"))
		{
			Physics::GetInstance()->AddJump(deltaTime);
		}
		ImGui::SameLine();
		if (ballsSpawned == false)
		{
			if (ImGui::Button("Spawn Balls"))
			{
		
				for (int i = 0; i < 10; i++)
				{
					Entity* a = new Entity();
					m_entities.push_back(a);
					a->AddComponent(
						new MeshRenderer(
							Resources::GetInstance()->GetModel("sphere.obj"),
							Resources::GetInstance()->GetShader("simple"),
							Resources::GetInstance()->GetTexture("Red.png"))
					);
					a->GetTransform()->SetPosition(glm::vec3(1.0f * i, 5.f, -20.f));
					a->AddComponent<RigidBody>();
					a->GetComponent<RigidBody>()->Init(new SphereShape(1.f));
					a->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));
				}
				ballsSpawned = true;
			}
		}

		if (cubesSpawned == false)
		{
			if (ImGui::Button("Spawn Cubes"))
			{

				for (int i = 0; i < 10; i++)
				{
					Entity* a = new Entity();
					m_entities.push_back(a);
					a->AddComponent(
						new MeshRenderer(
							Resources::GetInstance()->GetModel("cube.obj"),
							Resources::GetInstance()->GetShader("simple"),
							Resources::GetInstance()->GetTexture("Wood.jpg"))
					);
					a->GetTransform()->SetPosition(glm::vec3(1.0f * i, 5.f, -20.f));
					a->AddComponent<RigidBody>();
					a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 1.f, 1.f)));
					a->GetTransform()->SetScale(glm::vec3(1.f, 1.f,1.f));
				}
				cubesSpawned = true;
			}
		}

		static float skull_X = 10.0f;
		ImGui::SliderFloat("SkullX", &skull_X, -10.0f, 10.0f);
		static float skull_Y = 5.0f;
		ImGui::SliderFloat("SkullY", &skull_Y, 0.0f, 20.0f);
		static float skull_Z = -40.0f;
		ImGui::SliderFloat("SkullZ", &skull_Z, -50.0f, 0.0f);
		
		m_entities.at(7)->GetTransform()->SetPosition(glm::vec3(skull_X, skull_Y, skull_Z));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("W - Move Ball forward\r\nS - Move Ball backward\r\nA - Move Ball left\r\nD - Move Ball Right\r\nQ - Make Ball Jump\r\nE - Make Ball Spin");
		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(m_window);
	}


}
void Application::Quit()
{
	//Close SDL
	Physics::GetInstance()->Quit();
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();

	ImGui_ImplSdlGL3_Shutdown();
	ImGui::DestroyContext();

	
}
Application::~Application()
{
}
Application* Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}
void Application::Run()
{
	Init();
	GameInit();
	Loop();
	Quit();
}
void Application::Update(float deltaTime)
{
	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}
}
void Application::Render()
{
	/* Clear context */
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_mainCamera->Recalculate();

	for (auto& a : m_entities)
	{
		a->OnRender();
	}
}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}
