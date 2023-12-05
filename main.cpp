extern "C" {
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

#include "ecs/entity/EntityManager.h"


#include "Universe.h"

#include <chrono>
#include <functional>

//Noesis stuff, not all of this may be needed
#define NS_LICENSE_NAME "Yunaan"
#define NS_LICENSE_KEY "aUm+v2UEvr8OruLsSdY8RDX82J70l50/EZrDDIIoDtocqYXH"
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
#include "../headers/systems/AISystem.h"
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

	SoundSource SourceA(1.f, 1.f, { 0.0f,0.0f,0.0f }, { 0,0,0 }, false, true);
	SoundSource SourceB(1.f, 1.f, { 0.0f,0.0f,0.0f }, { 0,0,0 }, false, true);

	

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
		renderer.loadTexture("assets/textures/grass.jpg", "grass");

		renderer.loadModel("assets/models/ak74.fbx", "ak");
		renderer.loadModel("assets/models/cone45.obj", "cone");

		renderer.loadShaderProgram("shaders/basic.vert", "", "shaders/basic.frag", "basic");
		renderer.loadShaderProgram("shaders/secondary.vert", "", "shaders/secondary.frag", "secondary");

		renderer.setBackgroundColor({ 0.1f, 0.075f, 0.1f, 1.0f });

		glm::vec3 camRot{ 0.0f, 0.0f, 0.0f };
		glm::vec3 camPos{ 5.0f, 5.0f, 10.0f };

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

		GameMaster* gm = new GameMaster(&entityManager);


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
		stat.posOffset = { 0.0f, 0.0f, 0.0f };

		stat.modelName = "cube";
		bool swapTex = false;

		//World setup
		for (int x = 0; x <= 10; x++) {
			for (int y = 0; y <= 10; y++) {
				Entity fresh = entityManager.createEntity();
				trans.pos = { x, y };
				stat.modelName = "cube";
				if (swapTex) {
					stat.textureName = "grass";
				}
				else {
					stat.textureName = "surface";
				}
				stat.shaderName = "basic";
				entityManager.addComponent<TransformComponent>(fresh, trans);
				entityManager.addComponent<StaticMeshComponent>(fresh, stat);
				if (swapTex) {
					swapTex = false;
				}
				else {
					swapTex = true;
				}
			}
		}

		PlayerComponent playComp(0);
		MoveComponent moveComp;
		AttackComponent atkComp;
		HealthComponent hpComp;


		//TODO: player1
		Entity ak = entityManager.createEntity();
		trans.pos = { 5, 5 };
		stat.posOffset.z += 0.6f;
		stat.rotOffset.y = glm::radians(90.0f);
		stat.modelName = "ak";
		stat.textureName = "ak_texture";
		atkComp.damage = 2;
		atkComp.range = 3;
		moveComp.moved = false;
		moveComp.moveRange = 2;
		hpComp.health = 5;
		entityManager.addComponent<TransformComponent>(ak, trans);
		entityManager.addComponent<StaticMeshComponent>(ak, stat);
		entityManager.addComponent<PlayerComponent>(ak, playComp);
		entityManager.addComponent<MoveComponent>(ak, moveComp);
		entityManager.addComponent<AttackComponent>(ak, atkComp);
		entityManager.addComponent<HealthComponent>(ak, hpComp);


		//TODO: player2
		Entity ak2 = entityManager.createEntity();
		trans.pos = { 5, 6 };
		moveComp.moved = false;
		moveComp.moveRange = 1;
		atkComp.damage = 3;
		hpComp.health = 7;
		atkComp.range = 2;
		entityManager.addComponent<TransformComponent>(ak2, trans);
		entityManager.addComponent<StaticMeshComponent>(ak2, stat);
		entityManager.addComponent<PlayerComponent>(ak2, playComp);
		entityManager.addComponent<MoveComponent>(ak2, moveComp);
		entityManager.addComponent<AttackComponent>(ak2, atkComp);
		entityManager.addComponent<HealthComponent>(ak2, hpComp);

		// AI setup
		MessageBus bus;
		AISystem aiSystem(entityManager, bus, *gm);
		const int numOfEnemy = 2;
		// Generate Enemy
		for (int i = 0; i < numOfEnemy; i++) {
			trans.pos = { 7 + i, 7 };
			stat.modelName = "ak";  // replace this with actual model
			stat.textureName = "ak_texture";
			aiSystem.spawnEnemy(trans, stat);
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
		auto turnBtn = nsguiView->GetContent()->FindName<Noesis::Button>("turnBtn");
		auto turnText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("turnText");
		auto modeBtn = nsguiView->GetContent()->FindName<Noesis::Button>("actionBtn");
		auto modeText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("modeText");

		auto healthText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("healthText");
		auto moveText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("moveText");
		auto attackRangeText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("attackRangeText");
		auto attackText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("attackText");
		auto canMoveText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("canMoveText");

		bool lightOn = true;

		Renderer* rendPtr = &renderer; //things seem to get copied around so referencing the actual object
		//causes problems
		void* emPtr = &entityManager;
		Entity* entPtr = &entity2;
		bool isPlayerTurn = true;
		//void* gmPtr = &gm;


		turnBtn->Click() += [turnText, gm, modeText](Noesis::BaseComponent* sender,
			const Noesis::RoutedEventArgs& args) mutable {
				if (gm->currentTurn == playerTurn) {
					turnText->SetText("Enemy Turn");
					gm->endTurn();
				}
				else if (gm->currentTurn == enemyTurn) {
					turnText->SetText("Player Turn");
					gm->endTurn();
					gm->selected = NULL;
					gm->switchMode(select);
					modeText->SetText("Select Mode");
				}
				else {

				}
			};
		modeBtn->Click() += [modeText, gm](Noesis::BaseComponent* sender,
			const Noesis::RoutedEventArgs& args) mutable {
				if (gm->currentMode == select) {
					gm->switchMode(move);
					modeText->SetText("Move Mode");
					std::cout << "Move";
				}
				else if (gm->currentMode == move) {
					gm->switchMode(attack);
					modeText->SetText("Attack Mode");
					std::cout << "Attack";
				}
				else if (gm->currentMode == attack) {
					gm->switchMode(select);
					modeText->SetText("Select Mode");
					std::cout << "Select";
				}
			};
		//Without using its rather limited callbacks, GLFW will only let you know if a button is currently down or up.
		//This is for finding out if it was released on this frame.
		bool lmbDownPrevFrame = false;

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

			//entityManager.getComponent<StaticMeshComponent>(ak).rotOffset.z += deltaTime;

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

			//Audio Test
			sounddevice->SetPosition(camPos);
			sounddevice->SetOrientation(trueFwd, trueUp);

			//Send mouse events to NSGUI
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			nsguiView->MouseMove(x, y);
			Entity clicked;
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
				if (!lmbDownPrevFrame) {
					lmbDownPrevFrame = true;
					nsguiView->MouseButtonDown(x, y, Noesis::MouseButton_Left);

					glm::mat4 cam = glm::translate(glm::mat4(1.0f), camPos);
					cam = glm::rotate(cam, camRot.z, { 0.0f, 0.0f, 1.0f });
					cam = glm::rotate(cam, camRot.x, { 1.0f, 0.0f, 0.0f });

					glm::mat4 view = glm::inverse(cam);

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
					glm::intersectRayPlane(camPos, v, planeOrig,
						planeNorm, res);
					posOnPlane = camPos + v * res;
					std::cout << posOnPlane.x << ", " << posOnPlane.y << ", " << posOnPlane.z << std::endl;

					int gridPositionX = std::round(posOnPlane.x);
					int gridPositionY = std::round(posOnPlane.y);

					if (gm->currentMode == select) {
						gm->selectUnit(gridPositionX, gridPositionY);
						if (gm->selected != NULL) {
							std::cout << "\nNOT NULL";
							std::string stringVar = "Health " + std::to_string(entityManager.getComponent<HealthComponent>(gm->selected).health);
							healthText->SetText(stringVar.c_str());
							stringVar = "Move Range " + std::to_string(entityManager.getComponent<MoveComponent>(gm->selected).moveRange);
							moveText->SetText(stringVar.c_str());
							stringVar = "Attack Range " + std::to_string(entityManager.getComponent<AttackComponent>(gm->selected).range);
							attackRangeText->SetText(stringVar.c_str());
							stringVar = "Damage " + std::to_string(entityManager.getComponent<AttackComponent>(gm->selected).damage);
							attackText->SetText(stringVar.c_str());
							if (!gm->botSelected) {
								if (entityManager.getComponent<MoveComponent>(gm->selected).moved) {
									canMoveText->SetText("Moved True");
								}
								else {
									canMoveText->SetText("Moved False");
								}
							}
						}
					}
					else if (gm->currentMode == move) {
						bool moved = gm->moveSelected(gridPositionX, gridPositionY);
						if (moved) {
							canMoveText->SetText("Moved True");
							modeText->SetText("Select");
						}
					}
					else {
						bool hit = gm->attackSelected(gridPositionX, gridPositionY);
						if (hit) {
							SourceA.Play(gunA);
							canMoveText->SetText("Moved True");
							modeText->SetText("Select");
						}
					}
					if (gm->selected == NULL) {
						healthText->SetText("");
						moveText->SetText("");
						attackRangeText->SetText("");
						attackText->SetText("");
						canMoveText->SetText("");
					}
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

			// AI test
			aiSystem.update(nsguiView->GetContent()->FindName<Noesis::TextBlock>("turnText"));



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