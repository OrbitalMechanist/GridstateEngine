extern "C"{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#include "graphics/Renderer.h"
#include "Constants.h"
//#include "mapbuilder/MapGrid.cpp"
#include "audio/SoundDevice.h"
#include "audio/SoundBuffer.h"
#include "audio/SoundSource.h"

#include "glm/gtx/intersect.hpp"

#include "gamemaster/GameMaster.h"
#include "gamemaster/EnemyTurnCalculator.h"

#include "ecs/entity/EntityManager.h"



#include "Universe.h"

#include <chrono>
#include <functional>

//Noesis stuff, not all of this may be needed
#define NS_LICENSE_NAME "OrbitalMechanist"
#define NS_LICENSE_KEY "egazOJZKszhcgEWAByyi6qR5C0lQah8MCo95rksg472ePAcY"
#include <NsRender/RenderContext.h>
#include <NsCore/HighResTimer.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/UserControl.h>
#include <NsGui/CheckBox.h>
#include <NsGui/TextBlock.h>
#include <NsGui/Button.h>
#include <NsGui/IRenderer.h>
#include <NsGui/IView.h>
#include <NsGui/ResourceDictionary.h>
#include <NsApp/EntryPoint.h>
#include <NsApp/Launcher.h>
#include <NsApp/Display.h>
#include <NsApp/LocalXamlProvider.h>
#include <NsApp/LocalFontProvider.h>
#include <NsApp/ThemeProviders.h>

#include <NsRender/GLFactory.h>
#include <NsRender/GLRenderDeviceApi.h>

// AI inlucde 
#include "Systems/AISystem.h"
/*
	This file is just for testing, to be removed once we have our graphical engine ready.
	The code here currently lives in main.cpp for testing purposes, I'm keeping a double of
	this file for when we've built testing infrastructure for the engine. - Joe
*/

//NsMain is a lot like like main but Noesis-flavoured and platform-agnostic
int NsMain(int argc, char** argv) {
	NS_UNUSED(argc, argv);

	//Timer for testing audio
	int gunshotTimer = 0;

	//Audio
	SoundDevice* sounddevice = SoundDevice::get();

	uint32_t gunA = SoundBuffer::get()->addSoundEffect("assets/audio/gunshot2.wav");
	uint32_t gunB = SoundBuffer::get()->addSoundEffect("assets/audio/gunshot1.aiff");

	SoundSource SourceA(1.f, 1.f, {0.0f,0.0f,0.0f}, {0,0,0}, false, true);
	SoundSource SourceB(1.f, 1.f, {0.0f,0.0f,0.0f}, { 0,0,0 }, false, true);

	
	

	try {
		if (!glfwInit()) {
			throw std::runtime_error("Failed GLFW Init.");
		}

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

		GLFWwindow* window;
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Window", 0, nullptr);

		if (window == NULL) {
			glfwTerminate();
			throw std::runtime_error("Failed to create window.");
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::runtime_error("Failed to initialize GLAD.");
		}

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		Renderer renderer = Renderer(window, WINDOW_WIDTH, WINDOW_HEIGHT);

		renderer.loadTexture("assets/textures/stone_simple.png", "stone");
		renderer.loadTexture("assets/textures/surface_simple.png", "surface");
		renderer.loadTexture("assets/textures/AK74.png", "ak_texture");

		renderer.loadModel("assets/models/ak74.fbx", "ak");
		renderer.loadModel("assets/models/cone45.obj", "cone");

		renderer.loadShaderProgram("shaders/basic.vert", "", "shaders/basic.frag", "basic");
		renderer.loadShaderProgram("shaders/secondary.vert", "", "shaders/secondary.frag", "secondary");

		renderer.setBackgroundColor({ 0.1f, 0.075f, 0.1f, 1.0f });

		glm::vec3 camRot{ 0.0f, 0.0f, 0.0f };
		glm::vec3 camPos{ 0.0f, 0.0f, 10.0f };

		renderer.setAmbientLight("basic", glm::vec3(0.15f, 0.15f, 0.15f));

		renderer.setLightState("basic", 0, 2, { 0.0f, 5.0f, 1.0f }, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
			{ 0.0f, 1.0f, 0.0f }, 1.0f, 0, 5.0f, 5.0f);

		renderer.setLightState("basic", 1, 3, { 0.0f, 2.0f, 3.0f }, glm::vec3(0.0f, -0.45f, -1.0f),
			{ 1.0f, 1.0f, 0.65f }, 1.0f, glm::radians(90.0f), 10.0f, 10.0f);

		renderer.setLightState("basic", 5, 1, { 0.0f, 0.0f, 0.0f }, glm::vec3(-0.2f, -1.0f, -0.2f), { 0.6f, 0.6f, 1.0f }, 1.0f,
			0, -1, -1);

		renderer.setLightState("basic", 2, 1, { 0.0f, 0.0f, 0.0f }, glm::vec3(-0.2f, 0.0f, -0.2f), { 1.0f, 1.0f, 1.0f }, 1.0f,
			0, -1, -1);

		renderer.setLightState("basic", 3, 3, { 10.0f, 0.0f, 0.0f }, glm::vec3(0.0f, 1.0f, 0.0f), { 1.0f, 1.0f, 1.0f }, 1.0f,
			glm::radians(20.0f), 100.0f, 11.0f);

		renderer.setLightState("basic", 4, 3, { 0.0f, 0.0f, 4.0f }, glm::vec3(0.5f, 0.0f, -1.0f),
			{ 0.0f, 0.0f, 1.0f }, 1.0f, glm::radians(45.0f), -1.0f, -1.0f);

		//gameobject testing stuff

		EntityManager entityManager;

		Universe universe = Universe({ 0, 0, 0 }, { 1.0f, 1.0f }, renderer, entityManager);

		entityManager.registerComponentType<TransformComponent>();
		entityManager.registerComponentType<StaticMeshComponent>();
		entityManager.registerComponentType<AIComponent>();
		entityManager.registerComponentType<HealthComponent>();
		entityManager.registerComponentType<AttackComponent>();
		entityManager.registerComponentType<MoveComponent>();
		
		EnemyTurnCalculator* calc = new EnemyTurnCalculator(&entityManager);
		GameMaster* gm = new GameMaster(calc);

		//map
		const int MAP_SIZE = 11; // Size of the map
		std::array<std::array<int, MAP_SIZE>, MAP_SIZE> map = {};
		// set all to 1
		for (int i = 0; i <= 10; ++i) {
			for (int j = 0; j <= 10; ++j) {
				map[i][j] = 1;
				//std::cout << map[i][j] << " ";
			}
			//std::cout << std::endl;
		}

		Entity newEntity = entityManager.createEntity();
		Entity entity2 = entityManager.createEntity();

		TransformComponent trans;
		trans.pos = { 1, 5 };

		renderer.createMaterial("surfaceMaterial", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0);

		StaticMeshComponent stat;
		stat.modelName = "ak";
		stat.textureName = "stone";
		stat.shaderName = "basic";
		stat.materialName = "surfaceMaterial";

		entityManager.addComponent<TransformComponent>(newEntity, trans);
		entityManager.addComponent<StaticMeshComponent>(newEntity, stat);

		stat.modelName = "cube";
		trans.pos = { 2, 7 };
		

		entityManager.addComponent<TransformComponent>(entity2, trans);
		entityManager.addComponent<StaticMeshComponent>(entity2, stat);

		

		//World setup
		int count = 0;
		for (int x = -5; x <= 5; x++) {
			count++;
			for (int y = -5; y <= 5; y++) {
				Entity fresh = entityManager.createEntity();
				trans.pos = { x, y };
				stat.modelName = "cube";
				stat.textureName = "surface";
				stat.shaderName = "basic";
				entityManager.addComponent<TransformComponent>(fresh, trans);
				entityManager.addComponent<StaticMeshComponent>(fresh, stat);
			}
		}
		std::cout << "Cout: " << count << std::endl;

		
		

		/*
		for (int y = 0; y <= 12; y += 2) {
			renderer.addRenderObject(RenderObject("cube", "stone", "basic", { 10.0f, y + 1.0f, 0.0f },
				{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }));
		}
		*/

		Entity diag1 = entityManager.createEntity();
		trans.pos = { 5, 5 };
		map[0][10] = 0; // occupied 
		stat.posOffset = { 0.0f, 0.0f, 1.0f };
		stat.textureName = "stone";
		entityManager.addComponent<TransformComponent>(diag1, trans);
		entityManager.addComponent<StaticMeshComponent>(diag1, stat);

		Entity ctr = entityManager.createEntity();
		trans.pos = { 0, 0 };
		map[5][5] = 0; // occupied 
		stat.rotOffset = { 0.0f, 0.0f, glm::radians(45.0f) };
		entityManager.addComponent<TransformComponent>(ctr, trans);
		entityManager.addComponent<StaticMeshComponent>(ctr, stat);

		Entity diag2 = entityManager.createEntity();
		trans.pos = { -5, -5 };
		map[10][0] = 0;// occupied 
		stat.rotOffset = { 0.0f, 0.0f, 0.0f };
		entityManager.addComponent<TransformComponent>(diag2, trans);
		entityManager.addComponent<StaticMeshComponent>(diag2, stat);

		Entity mob = entityManager.createEntity();
		trans.pos = { 0, 4 };
		map[1][5] = 0;// occupied 
		entityManager.addComponent<TransformComponent>(mob, trans);
		entityManager.addComponent<StaticMeshComponent>(mob, stat);

		Entity turnBlock = entityManager.createEntity();
		trans.pos = { 0, 10 };
		entityManager.addComponent<TransformComponent>(turnBlock, trans);
		entityManager.addComponent<StaticMeshComponent>(turnBlock, stat);

		Entity block = entityManager.createEntity();
		trans.pos = { 1, 5 };
		map[0][6] = 0;// occupied 
		entityManager.addComponent<TransformComponent>(block, trans);
		entityManager.addComponent<StaticMeshComponent>(block, stat);

		Entity ak = entityManager.createEntity();
		trans.pos = { 0, 0 };
		map[5][5] = 0;// occupied 
		stat.posOffset.z += 0.6f;
		stat.rotOffset.y = glm::radians(90.0f);
		stat.modelName = "ak";
		stat.textureName = "ak_texture";
		entityManager.addComponent<TransformComponent>(ak, trans);
		entityManager.addComponent<StaticMeshComponent>(ak, stat);

		/*for (int i = 0; i <= 10; ++i) {
			for (int j = 0; j <= 10; ++j) {
				
				std::cout << map[i][j] << " ";
			}
			std::cout << std::endl;
		}*/

		// AI setup
		MessageBus bus;
		AISystem aiSystem(entityManager, bus, gm, map);
		aiSystem.spawnEnemy();
		//aiSystem.spawnEnemy();
		//aiSystem.spawnEnemy();
		// Setup AI's transform and staciMesh
		int countAI = 1;
		for (auto aiEntity : entityManager.getEntitiesWithComponent<AIComponent>()) {
			std::cout << "ai spawned: " << entityManager.getComponent<HealthComponent>(aiEntity).health << std::endl;
			
			if (countAI == 0) {
				trans.pos = { 4, 5 };
				//map[0][9] = 2;
			}
			else if (countAI == 1) {
				trans.pos = { -4, 4 };
				//map[0][9] = 2;
			}
			else if (countAI == 2) {
				trans.pos = { 4, -4 };
				//map[0][9] = 2;
			}
			countAI++;
			stat.posOffset.z += 0.6f;
			stat.rotOffset.y = glm::radians(90.0f);
			stat.modelName = "ak";  // replace this with actual model
			stat.textureName = "ak_texture";
			entityManager.addComponent<TransformComponent>(aiEntity, trans);
			entityManager.addComponent<StaticMeshComponent>(aiEntity, stat);
		}
		
		// Setup Player's transform and staciMesh
		 /*test - remove this after we have player*/
		Entity playerEntity = entityManager.createEntity(); // test
		Entity playerEntity2 = entityManager.createEntity(); // test

		// player 1
		PlayerComponent player(1);// test 
		HealthComponent hp(bus, 0, 100, 2); // assume health starts at 100 , armor 2
		AttackComponent att(1, 2, 1); // damage range and attackModifier = 1
		MoveComponent movement(2, false);
		entityManager.addComponent<PlayerComponent>(playerEntity, player);
		entityManager.addComponent<HealthComponent>(playerEntity, hp);
		entityManager.addComponent<AttackComponent>(playerEntity, att);
		entityManager.addComponent<MoveComponent>(playerEntity, movement);

		// player 2
		//PlayerComponent player2(2);// test 
		//entityManager.addComponent<PlayerComponent>(playerEntity2, player2);
		//entityManager.addComponent<HealthComponent>(playerEntity2, hp);
		//entityManager.addComponent<AttackComponent>(playerEntity2, att);
		//entityManager.addComponent<MoveComponent>(playerEntity2, movement);
		int countP = 0;
		for (auto player : entityManager.getEntitiesWithComponent<PlayerComponent>()) {
			if (countP == 0) {
				trans.pos = { -3 , -3 };
				//map[8][2] = 2;
			}
			else if(countP == 1) {
				trans.pos = { 1 , -3 };
				//map[8][2] = 2;
			}
			//countP++;
			
			stat.posOffset.z += 0.6f;
			stat.rotOffset.y = glm::radians(90.0f);
			stat.modelName = "ak";
			stat.textureName = "ak_texture";
			entityManager.addComponent<TransformComponent>(player, trans);
			entityManager.addComponent<StaticMeshComponent>(player, stat);
		}

		//NoesisGUI setup, seems to need to happen after the GLFW system is done setting up
		Noesis::GUI::SetLicense(NS_LICENSE_NAME, NS_LICENSE_KEY);

		Noesis::GUI::SetLogHandler([](const char*, uint32_t, uint32_t level, const char*, const char* msg)
			{
				const std::string levelText[] = { "Trace", "Debug", "Info", "Warning", "Error" };
				if (level < 3) {
					std::cout << "Noesis " << levelText[level] << " : " << msg << std::endl;
				}
				else {
					std::cerr << "Noesis " << levelText[level] << " : " << msg << std::endl;
				}
			});

		Noesis::GUI::Init();

		//We aren't really *basing* our engine entirely on the Application Framework but we will use
		//some of its features, at least for now.

		NoesisApp::Launcher::RegisterAppComponents();

		Noesis::Ptr<NoesisApp::LocalXamlProvider> xamlProvider = Noesis::MakePtr<NoesisApp::LocalXamlProvider>("./assets/ui");
		Noesis::Ptr<NoesisApp::LocalFontProvider> fontProvider = Noesis::MakePtr<NoesisApp::LocalFontProvider>("./assets/fonts");

		NoesisApp::SetThemeProviders(xamlProvider, fontProvider);

		Noesis::GUI::LoadApplicationResources("Theme/NoesisTheme.DarkBlue.xaml");

		Noesis::Ptr<Noesis::UserControl> uiElement = Noesis::GUI::LoadXaml<Noesis::UserControl>("everything.xaml");

		Noesis::Ptr<Noesis::IView> nsguiView = Noesis::GUI::CreateView(uiElement);
		nsguiView->SetFlags(Noesis::RenderFlags_PPAA | Noesis::RenderFlags_LCD);
		nsguiView->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

		nsguiView->GetRenderer()->Init(NoesisApp::GLFactory::CreateDevice(false));
		//End Noesis setup (actually ending noesis happens at the very end)

		static auto startTime = std::chrono::high_resolution_clock::now();
		static float prevTime = 0;

		int prevWidth = WINDOW_WIDTH;
		int	prevHeight = WINDOW_HEIGHT;

		//This is how we get the XAML elements in the UI to change them, or get their state.
		//Important note: FindName will probably still succeed and return the element even if
		//you give it the wrong type, but the parameters you could get/set would not necessarily
		//correspond to what the element actually has and thus not work as expected.
		auto targetText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("textTarget");
		auto targetBtn = nsguiView->GetContent()->FindName<Noesis::Button>("btn");
		auto turnBtn = nsguiView->GetContent()->FindName<Noesis::Button>("turnBtn");
		auto turnText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("turnText");

		bool lightOn = true;

		Renderer* rendPtr = &renderer; //things seem to get copied around so referencing the actual object
		//causes problems
		void* emPtr = &entityManager;
		Entity* entPtr = &entity2;
		bool isPlayerTurn = true;
		void* gmPtr = &gm;

		targetBtn->Click() += [rendPtr, lightOn,targetText](Noesis::BaseComponent* sender, 
			const Noesis::RoutedEventArgs& args) mutable {
			if (lightOn) {
				lightOn = false;
				rendPtr->setLightState("basic", 0, 0, { 0.0f, 5.0f, 1.0f }, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
					{ 0.0f, 1.0f, 0.0f }, 1.0f, 0, 5.0f, 5.0f);
				targetText->SetText("off");
			}
			else {
				lightOn = true;
				rendPtr->setLightState("basic", 0, 2, { 0.0f, 5.0f, 1.0f }, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
					{ 0.0f, 1.0f, 0.0f }, 1.0f, 0, 5.0f, 5.0f);
				targetText->SetText("on");
			}
		};
		
		//Looks like each callback has a limit on how much memory it can involve. On the bright size,
		//you can have multiple callbacks.
		targetBtn->Click() += [emPtr, newEntity, mob, lightOn](Noesis::BaseComponent* sender,
			const Noesis::RoutedEventArgs& args) mutable {
				((EntityManager*)emPtr)->getComponent<TransformComponent>(mob).pos.x -= 1;
				if (lightOn) {
					lightOn = false;
					((EntityManager*)emPtr)->getComponent<StaticMeshComponent>(newEntity).textureName = "surface";
				}
				else {
					lightOn = true;
					((EntityManager*)emPtr)->getComponent<StaticMeshComponent>(newEntity).textureName = "stone";
				}

		};
		turnBtn->Click() += [turnText, gm, turnBlock](Noesis::BaseComponent* sender,
			const Noesis::RoutedEventArgs& args) mutable {
				if (gm->currentTurn == playerTurn) {
					turnText->SetText("Enemy");
					gm->endTurn();
					//((EntityManager*)gm->entityManager)->getComponent<TransformComponent>(turnBlock).pos.x -= 1;
				}
				else if(gm->currentTurn == enemyTurn) {
					turnText->SetText("Player");
					//gm->endTurn();
					
					//((EntityManager*)gm->entityManager)->getComponent<TransformComponent>(turnBlock).pos.x += 1;
				}
				else {

				}
			};
		//Without using its rather limited callbacks, GLFW will only let you know if a button is currently down or up.
		//This is for finding out if it was released on this frame.
		bool lmbDownPrevFrame = false;

		Entity clicked = entityManager.createEntity(); // for clicked entity

		while (!glfwWindowShouldClose(window)) {
			auto currentTime = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
			float deltaTime = time - prevTime;

			nsguiView->Update(time); //this should happen early so that the time value is as exact as possible


			//The correct way to update framebuffer size is with callbacks.
			//However, callbacks are hard because we have multiple member objects that need
			//to be changed when that happens, so I check it like this instead.
			//This is probably something I should fix, but performance impact seems negligible, at least.
			int cWidth, cHeight;
			glfwGetFramebufferSize(window, &cWidth, &cHeight);
			
			// fixed Crashed when minimized the window -> cWidth == cHeight == 0;
			if ((cWidth != prevWidth || cHeight != prevHeight) && !(cWidth == 0 || cHeight == 0)) {
				renderer.updateWindowSize(window, cWidth, cHeight);
				nsguiView->SetSize(cWidth, cHeight);
				prevWidth = cWidth;
				prevHeight = cHeight;
			}

			renderer.setCameraPosition(camPos);
			renderer.setCameraRotation(camRot);

			entityManager.getComponent<StaticMeshComponent>(ak).rotOffset.z += deltaTime;

			universe.update(deltaTime);

			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
				camRot.x += 1.0f * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
				camRot.x -= 1.0f * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
				camRot.z -= 1.0f * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
				camRot.z += 1.0f * deltaTime;
			}

			glm::mat4 movementRotation = glm::rotate(glm::mat4(1.0f), camRot.z, { 0.0f, 0.0f, 1.0f });
			movementRotation = glm::rotate(movementRotation, camRot.x, { 1.0f, 0.0f, 0.0f });
			movementRotation = glm::rotate(movementRotation, camRot.y, { 0.0f, 1.0f, 0.0f });

			//The camera point down by default, so the initial forward is -Z
			glm::vec4 trueFwd = movementRotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
			glm::vec4 trueRight = movementRotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			glm::vec4 trueUp = movementRotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);


			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				camPos += glm::vec3(trueFwd.x, trueFwd.y, trueFwd.z) * 2.5f * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				camPos += glm::vec3(trueFwd.x, trueFwd.y, trueFwd.z) * -2.5f * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				camPos += glm::vec3(trueRight.x, trueRight.y, trueRight.z) * 2.5f * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				camPos += glm::vec3(trueRight.x, trueRight.y, trueRight.z) * -2.5f * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				camPos += glm::vec3(trueUp.x, trueUp.y, trueUp.z) * 2.5f * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
				camPos += glm::vec3(trueUp.x, trueUp.y, trueUp.z) * -2.5f * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				camPos.z += 2.5f * deltaTime;
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
				camPos.z += -2.5f * deltaTime;
			}

			//Send mouse events to NSGUI
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			nsguiView->MouseMove(x, y);
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
				if (!lmbDownPrevFrame) {
					lmbDownPrevFrame = true;
					nsguiView->MouseButtonDown(x, y, Noesis::MouseButton_Left);
					std::cout << "\n" << x << " : " << y;
					//vec3
					//mat4
					//mat4
					//vec4

					glm::mat4 cam = glm::translate(glm::mat4(1.0f), camPos);
					cam = glm::rotate(cam, camRot.z, { 0.0f, 0.0f, 1.0f });
					cam = glm::rotate(cam, camRot.x, { 1.0f, 0.0f, 0.0f });

					glm::mat4 view = glm::inverse(cam);

					//glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
					//glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
					//

					glm::mat4 projection = glm::perspective(glm::radians(60.0f),
						cWidth / (float)cHeight, 0.1f, 100.0f);

					glm::vec3 farPlaneClickPos = glm::unProject(glm::vec3(x, cHeight - y, 1.0f),
						view,
						projection,
						glm::vec4(0.0f, 0.0f, cWidth, cHeight));
					std::cout << "\n" << farPlaneClickPos.x << " : "
						<< farPlaneClickPos.y << " : " << farPlaneClickPos.z << std::endl;

					auto v = normalize(farPlaneClickPos - camPos);

					glm::vec3 posOnPlane{ 0, 0, 0 };
					glm::vec3 planeOrig{ 0, 0, 0 };
					glm::vec3 planeNorm{ 0, 0, 1 };
					float res = 0;
					glm::intersectRayPlane(camPos, v, planeOrig,planeNorm, res);
					posOnPlane = camPos + v * res;
					//std::cout << "test1: " << posOnPlane.x << ", " << posOnPlane.y << ", " << posOnPlane.z << std::endl;
					int gridPositionX = (int)posOnPlane.x;
					int gridPositionY = (int)posOnPlane.y;
					//std::cout << "test2: " << gridPositionX << ", " << gridPositionY << ", " << std::endl;
					std::vector<Entity> entitiesWithAI = entityManager.getEntitiesWithComponent<AIComponent>();
					std::vector<Entity> entitiesWithPlayers = entityManager.getEntitiesWithComponent<PlayerComponent>();
					bool clickedFound = false;
					for (auto entity : entitiesWithPlayers) {
						if (static_cast<int>(entityManager.getComponent<TransformComponent>(entity).x) == static_cast<int>(gridPositionX && entityManager.getComponent<TransformComponent>(entity).y) == gridPositionY) {
							clicked = entity;
							clickedFound = true; //Get the clicked entity based on the unit clicked, ensuring that you can only click on entities with the player component
						}
					}
					if (clickedFound && entityManager.getComponent<MoveComponent>(clicked).moved == false) { //If you clicked on a clickable entity and it has not moved yet...
						bool playerFound = false;
						bool enemyDead = false;
						bool emptyGrid = true;
						std::cout << "Clicked" << std::endl;
						for (auto entity : entitiesWithPlayers) { //Check if you are trying to move your clicked entity onto a grid that already contains one of your allies
							if (entityManager.getComponent<TransformComponent>(entity).x == gridPositionX &&
								entityManager.getComponent<TransformComponent>(entity).y == gridPositionY && !entityManager.getComponent<MoveComponent>(entity).moved) {
								playerFound = true; //If clicking on a player that hasn't moved, select that entity instead.
								emptyGrid = false;
								clicked = entity;
								break;
							}
							else if (entityManager.getComponent<TransformComponent>(entity).x == gridPositionX &&
								entityManager.getComponent<TransformComponent>(entity).y == gridPositionY && entityManager.getComponent<MoveComponent>(entity).moved) {
								playerFound = true; //else, if clicking on a player that has moved, do nothing but declare grid space as non-empty and you found a player
								emptyGrid = false;
								break;
							}
						}
						if (!playerFound) { //If you did not try to move onto a grid that contains an ally...
							for (auto entity : entitiesWithAI) {
								if (entityManager.getComponent<TransformComponent>(entity).x == gridPositionX &&
									entityManager.getComponent<TransformComponent>(entity).y == gridPositionY) {
									emptyGrid = false; //If grid space has an enemy
									float distanceX = abs(entityManager.getComponent<TransformComponent>(clicked).x - entityManager.getComponent<TransformComponent>(entity).x);
									float distanceY = abs(entityManager.getComponent<TransformComponent>(clicked).y - entityManager.getComponent<TransformComponent>(entity).y); //Check if enemy is within range
									if ((distanceX == entityManager.getComponent<MoveComponent>(clicked).moveRange && distanceY == 0) || (distanceX == 0 && distanceY == entityManager.getComponent<MoveComponent>(clicked).moveRange)) {
										entityManager.getComponent<HealthComponent>(entity).health -= entityManager.getComponent<AttackComponent>(clicked).damage; //If within range, subtract enemy's HP by clicked entity's damage
										if (entityManager.getComponent<HealthComponent>(entity).health <= 0) { //If enemy health is equal or less than 0, destroy it and move the player onto its grid spot. 
											entityManager.destroyEntity(entity); //Destroys the entity, removeComponent is not implemented yet so that's not called
											entityManager.getComponent<TransformComponent>(clicked).pos = { gridPositionX, gridPositionY };
											entityManager.getComponent<MoveComponent>(clicked).moved = true;
										}
									}
								}
							}
						}
						//move entity
						if (emptyGrid) { //If no player or enemy is on the grid spot...
							float distanceX = abs(entityManager.getComponent<TransformComponent>(clicked).x - gridPositionX);
							float distanceY = abs(entityManager.getComponent<TransformComponent>(clicked).y - gridPositionY); //Get the distance of the grid point 
							if ((distanceX == entityManager.getComponent<MoveComponent>(clicked).moveRange && distanceY == 0) || (distanceX == 0 && distanceY == entityManager.getComponent<MoveComponent>(clicked).moveRange)) {
								entityManager.getComponent<TransformComponent>(clicked).pos = { gridPositionX, gridPositionY }; //If grid spot is within moving range, move the player onto the spot.
								entityManager.getComponent<MoveComponent>(clicked).moved = true;
							}
						}
					}
				}
				else {
					/*
					glm::mat4 cam = glm::translate(glm::mat4(1.0f), camPos);
					cam = glm::rotate(cam, camRot.z, { 0.0f, 0.0f, 1.0f });
					cam = glm::rotate(cam, camRot.x, { 1.0f, 0.0f, 0.0f });

					glm::mat4 view = glm::inverse(cam);

					//glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
					//glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
					//

					glm::mat4 projection = glm::perspective(glm::radians(60.0f),
						cWidth / (float)cHeight, 0.1f, 100.0f);

					glm::vec3 farPlaneClickPos = glm::unProject(glm::vec3(x, cHeight - y, 1.0f),
						view,
						projection,
						glm::vec4(0.0f, 0.0f, cWidth, cHeight));
					std::cout << "\n" << farPlaneClickPos.x << " : "
						<< farPlaneClickPos.y << " : " << farPlaneClickPos.z << std::endl;

					auto v = normalize(farPlaneClickPos - camPos);

					glm::vec3 posOnPlane{ 0, 0, 0 };
					glm::vec3 planeOrig{ 0, 0, 0 };
					glm::vec3 planeNorm{ 0, 0, 1 };
					float res = 0;
					glm::intersectRayPlane(camPos, v, planeOrig, planeNorm, res);
					posOnPlane = camPos + v * res;
					//std::cout << "test1: " << posOnPlane.x << ", " << posOnPlane.y << ", " << posOnPlane.z << std::endl;
					int gridPositionX = (int)posOnPlane.x;
					int gridPositionY = (int)posOnPlane.y;
					float distanceX = abs(entityManager.getComponent<TransformComponent>(clicked).x - gridPositionX);
					float distanceY = abs(entityManager.getComponent<TransformComponent>(clicked).y - gridPositionY); //Get the distance of the grid point 
					if (5 >= gridPositionX >= -5 && 5 >= gridPositionY >= -5) {
						entityManager.getComponent<TransformComponent>(clicked).pos = { gridPositionX, gridPositionY }; //If grid spot is within moving range, move the player onto the spot.
						entityManager.getComponent<MoveComponent>(clicked).moved = true;
					}
					*/
				}
			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
				if (lmbDownPrevFrame) {
					lmbDownPrevFrame = false;
					nsguiView->MouseButtonUp(x, y, Noesis::MouseButton_Left);
				}
			}


			prevTime = time;

			//NSGUI rendering stuff
			bool nsguiTreeDirty = nsguiView->GetRenderer()->UpdateRenderTree();
			if (nsguiTreeDirty) {
				//Rendering to offscreen buffer (I think) only needs to happen
 			    //if something changed in the UI. If not, the previous one is kept and reused this frame.
				nsguiView->GetRenderer()->RenderOffscreen();
			}

			//NS docs say the 3D scene should happen after RenderOffscreen() occurs.
			//RenderOffscreen changes the GL state. Renderer.prepareForOperation() restores it to what the Renderer likes.
			renderer.prepareForOperation();
			renderer.renderFromQueue(true);

			//Back to NSGUI. This needs to happen whether the UI actually changed or not
			//because UI has to go on top of the scene in the otput framebuffer.
			nsguiView->GetRenderer()->Render();

			glfwSwapBuffers(window);
			glfwPollEvents();

			//Audio Test
			sounddevice->SetPosition(camPos);
			sounddevice->SetOrientation(trueFwd, trueUp);

			if (gunshotTimer > 0) {
				gunshotTimer--;
			}
			else {
				int j = rand() % 2;
				if (j == 0) {
					SourceA.Play(gunA);
				}
				if (j == 1) {
					SourceA.Play(gunB);
				}
				gunshotTimer = 600;
			}

			// AI test
			aiSystem.update(); // check this later - not sure if deltatime is 'time'
			


		} //End of operation loop. Everything after this is cleanup.

		//NSGUI stuff should be manually shut down before exiting the program.
		nsguiView->GetRenderer()->Shutdown();
		//All Noesis::Ptr objects must be reset to free them because they are, in fact, pointers.
		nsguiView.Reset();
		xamlProvider.Reset();
		fontProvider.Reset();
		uiElement.Reset();
		Noesis::GUI::Shutdown();

		glfwTerminate();
		return 0;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}