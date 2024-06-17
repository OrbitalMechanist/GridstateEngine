extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#include "graphics/Renderer.h"
#include "Constants.h"
//#include "mapbuilder/MapGrid.cpp"
#include "audio/AudioManager.h"

#include "glm/gtx/intersect.hpp"

#include "gamemaster/GameMaster.h"

#include "ecs/entity/EntityManager.h"

#include "Universe.h"

#include <chrono>
#include <functional>

//Noesis stuff, not all of this may be needed
#include <NsRender/RenderContext.h>
#include <NsCore/HighResTimer.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/UserControl.h>
#include <NsGui/CheckBox.h>
#include <NsGui/TextBlock.h>
#include <NsGui/Button.h>
#include <NsGui/Grid.h>
#include <NsGui/IRenderer.h>
#include <NsGui/IView.h>
#include <NsGui/ResourceDictionary.h>
#include <NsApp/EntryPoint.h>
#include <NsApp/Launcher.h>
#include <NsApp/Display.h>
#include <NsApp/LocalXamlProvider.h>
#include <NsApp/LocalFontProvider.h>
#include <NsApp/ThemeProviders.h>
#include <NsGui/VisualTreeHelper.h>
#include <NsRender/GLFactory.h>
#include <NsRender/GLRenderDeviceApi.h>

// AI inlucde 
#include "../headers/systems/AISystem.h"

// UI
#include "../headers/ui/UIController.h"

//NsMain is a lot like like main but Noesis-flavoured and evidently platform-agnostic
int NsMain(int argc, char** argv) {
	NS_UNUSED(argc, argv);

	//Timer for testing audio
	int gunshotTimer = 0;

	//Audio
	AudioManager audioManager;
	uint32_t gunA = audioManager.getSoundEffect("spellHit");
	uint32_t gunB = audioManager.getSoundEffect("spellCast");

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

		//Texture loading
		renderer.loadTexture("assets/textures/tree_texture.png", "tree_texture");
		renderer.loadTexture("assets/textures/white.png", "white");
		renderer.loadTexture("assets/textures/red_blue_texture.jpg", "red_blue_texture");
		renderer.loadTexture("assets/textures/grass.jpg", "grass");
		renderer.loadTexture("assets/textures/bush_texture.png", "bush_texture");
		renderer.loadTexture("assets/textures/rock_texture.jpg", "rock_texture");

		renderer.loadTexture("assets/textures/stone_simple.png", "stone");
		renderer.loadTexture("assets/textures/surface_simple.png", "surface");
		renderer.loadTexture("assets/textures/AK74.png", "ak_texture");

		renderer.loadModel("assets/models/ak74.fbx", "ak");

		//Model loading
		renderer.loadModel("assets/models/bushTree.fbx", "tree");
		renderer.loadModel("assets/models/singleBigRock.fbx", "rock_big");
		renderer.loadModel("assets/models/bush.fbx", "bush");
		renderer.loadModel("assets/models/character.fbx", "character");
		renderer.loadModel("assets/models/cursor.obj", "cursor");
		renderer.loadModel("assets/models/enemy.fbx", "enemy");

		//Shader loading
		renderer.loadShaderProgram("shaders/basic.vert", "", "shaders/basic.frag", "basic");
		renderer.loadShaderProgram("shaders/secondary.vert", "", "shaders/secondary.frag", "secondary");

		//Background color
		renderer.setBackgroundColor({ 0.1f, 0.075f, 0.1f, 1.0f });

		//Initial camera position
		glm::vec3 camRot{ 0.0f, 0.0f, 0.0f };
		glm::vec3 camPos{ 0.0f, 0.0f, 10.0f };

		renderer.setAmbientLight("basic", glm::vec3(0.15f, 0.15f, 0.15f));

		renderer.setLightState("basic", 0, 2, { 0.0f, 5.0f, 1.0f }, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
			{ 0.0f, 1.0f, 0.0f }, 1.0f, 0, 5.0f, 5.0f);

		renderer.setLightState("basic", 1, 3, { 0.0f, 2.0f, 3.0f }, glm::vec3(0.0f, -0.45f, -1.0f),
			{ 1.0f, 1.0f, 0.65f }, 1.0f, glm::radians(90.0f), 10.0f, 10.0f);

		renderer.setLightState("basic", 5, 1, { 0.0f, 0.0f, 0.0f }, glm::vec3(-0.2f, -1.0f, -0.2f), { 0.3f, 0.3f, 0.7f }, 1.0f,
			0, -1, -1);

		renderer.setLightState("basic", 2, 1, { 0.0f, 0.0f, 0.0f }, glm::vec3(-0.2f, 0.0f, -0.2f), { 0.6f, 0.6f, 0.6f }, 1.0f,
			0, -1, -1);

		renderer.setLightState("basic", 3, 3, { 10.0f, 0.0f, 0.0f }, glm::vec3(0.0f, 1.0f, 0.0f), { 1.0f, 1.0f, 1.0f }, 1.0f,
			glm::radians(20.0f), 100.0f, 11.0f);

		renderer.setLightState("basic", 4, 3, { 0.0f, 0.0f, 4.0f }, glm::vec3(0.5f, 0.0f, -1.0f),
			{ 0.0f, 0.0f, 1.0f }, 1.0f, glm::radians(45.0f), -1.0f, -1.0f);

		//Game Objects Initalized and Components registered

		EntityManager entityManager;

		Universe universe = Universe({ 0, 0, 0 }, { 1.0f, 1.0f }, renderer, entityManager);

		entityManager.registerComponentType<TransformComponent>();
		entityManager.registerComponentType<StaticMeshComponent>();
		entityManager.registerComponentType<AIComponent>();
		entityManager.registerComponentType<HealthComponent>();
		entityManager.registerComponentType<AttackComponent>();
		entityManager.registerComponentType<MoveComponent>();
		entityManager.registerComponentType<NameComponent>();

		// Game master to handle gameplay
		GameMaster* gm = new GameMaster(&entityManager, &audioManager);

		// Transform Component initialized
		TransformComponent trans;
		trans.pos = { 15, 0 };

		renderer.createMaterial("surfaceMaterial", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0);
		renderer.createMaterial("cursorMaterial", glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0);

		// Static Mesh component for models, textures and offsets of entities.
		// It then gets copied for each actual instance.
		StaticMeshComponent stat;
		stat.modelName = "cursor";
		stat.textureName = "white";
		stat.shaderName = "basic";
		stat.materialName = "surfaceMaterial";
		stat.posOffset = { 0.0f, 0.0f, 0.0f };

		//Objects setup using the "ECS"
		for (int x = -5; x <= 5; x++) {
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

		Entity ppCube = entityManager.createEntity();
		trans.pos = { 5, 5 };
		stat.posOffset = { 0.0f, 0.0f, 1.0f };
		stat.textureName = "stone";
		entityManager.addComponent<TransformComponent>(ppCube, trans);
		entityManager.addComponent<StaticMeshComponent>(ppCube, stat);

		Entity pnCube = entityManager.createEntity();
		trans.pos = { 5, -5 };
		entityManager.addComponent<TransformComponent>(pnCube, trans);
		entityManager.addComponent<StaticMeshComponent>(pnCube, stat);

		Entity nnCube = entityManager.createEntity();
		trans.pos = { -5, -5 };
		entityManager.addComponent<TransformComponent>(nnCube, trans);
		entityManager.addComponent<StaticMeshComponent>(nnCube, stat);

		Entity npCube = entityManager.createEntity();
		trans.pos = { -5, 5 };
		entityManager.addComponent<TransformComponent>(npCube, trans);
		entityManager.addComponent<StaticMeshComponent>(npCube, stat);

		Entity ctrCube = entityManager.createEntity();
		trans.pos = { 0, 0 };
		entityManager.addComponent<TransformComponent>(ctrCube, trans);
		entityManager.addComponent<StaticMeshComponent>(ctrCube, stat);

		Entity orbitCube = entityManager.createEntity();
		trans.pos = { 0, 5 };
		stat.scale = { 0.5f, 0.5f, 1.0f };
		entityManager.addComponent<TransformComponent>(orbitCube, trans);
		entityManager.addComponent<StaticMeshComponent>(orbitCube, stat);

		Entity slideCube = entityManager.createEntity();
		trans.pos = { 1, 4 };
		entityManager.addComponent<TransformComponent>(slideCube, trans);
		entityManager.addComponent<StaticMeshComponent>(slideCube, stat);

		Entity ak = entityManager.createEntity();
		trans.pos = { 0 , 0 };
		stat.modelName = "ak";
		stat.textureName = "ak_texture";
		stat.scale = { 1.0f, 1.0f, 1.0f };
		stat.rotOffset = { 0.0f, glm::radians(90.0f), 0.0f };
		stat.posOffset = { 0.0f, 0.0f, 1.53f };
		entityManager.addComponent<TransformComponent>(ak, trans);
		entityManager.addComponent<StaticMeshComponent>(ak, stat);


		UIController ui("everything.xaml", gm, entityManager);

		auto nsguiView = ui.GetNsguiView();

		static auto startTime = std::chrono::high_resolution_clock::now();
		static float prevTime = 0;

		int prevWidth = WINDOW_WIDTH;
		int	prevHeight = WINDOW_HEIGHT;

		// Gets turn button from UI controller
		auto turnBtn = ui.GetturnBtn();

		//This is for finding out if LMB was released on this frame.
		bool lmbDownPrevFrame = false;

		while (!glfwWindowShouldClose(window)) {
			auto currentTime = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
			float deltaTime = time - prevTime;

			nsguiView->Update(time); //this should happen early so that the time value is as exact as possible

			int cWidth, cHeight;
			glfwGetFramebufferSize(window, &cWidth, &cHeight);


			if ((cWidth != prevWidth || cHeight != prevHeight) && !(cWidth == 0 || cHeight == 0)) {
				renderer.updateWindowSize(window, cWidth, cHeight);
				nsguiView->SetSize(cWidth, cHeight);
				prevWidth = cWidth;
				prevHeight = cHeight;
			}

			renderer.setCameraPosition(camPos);
			renderer.setCameraRotation(camRot);

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

			//The camera points down by default, so the initial forward is -Z
			glm::vec4 trueFwd = movementRotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
			glm::vec4 trueRight = movementRotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			glm::vec4 trueUp = movementRotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

			// Camera controls when pressing a key on keyboard
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

			//Object Movement Events
			entityManager.getComponent<StaticMeshComponent>(ak).rotOffset 
				+= glm::vec3{0.0f, 0.0f, 0.5f * deltaTime};
			entityManager.getComponent<StaticMeshComponent>(orbitCube).posOffset 
				= glm::vec3{ sin(time * 1.3), cos(time * 1.3), 1.0f };
			entityManager.getComponent<StaticMeshComponent>(slideCube).posOffset 
				= glm::vec3{ 0, sin(time * 2), 1.0f };

			//Audio Test
			audioManager.setDevicePosition(camPos);
			audioManager.setDeviceOrientation(trueFwd, trueUp);

			if (!(cWidth <= 0 || cHeight <= 0)) {
				//Mouse stuff, including sending to NSGUI
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				nsguiView->MouseMove(x, y);
				Entity clicked;
				GLint viewport[4];
				glGetIntegerv(GL_VIEWPORT, viewport);

				//Find Grid Position of mouse
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

				auto v = normalize(farPlaneClickPos - camPos);

				glm::vec3 posOnPlane{ 0, 0, 0 };
				glm::vec3 planeOrig{ 0, 0, 0 };
				glm::vec3 planeNorm{ 0, 0, 1 };
				float res = 0;
				glm::intersectRayPlane(camPos, v, planeOrig,
					planeNorm, res);
				posOnPlane = camPos + v * res;

				int gridPositionX = std::round(posOnPlane.x);
				int gridPositionY = std::round(posOnPlane.y);

				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
					if (!lmbDownPrevFrame) {
						lmbDownPrevFrame = true;
						nsguiView->MouseButtonDown(x, y, Noesis::MouseButton_Left);

						Noesis::HitTestResult uiHitTest = Noesis::VisualTreeHelper::HitTest(
							Noesis::VisualTreeHelper::GetRoot(turnBtn), Noesis::Point{ static_cast<float>(x),
							static_cast<float>(y) });
						//If it hits UI don't click into game world

					}
				}
				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
					if (lmbDownPrevFrame) {
						lmbDownPrevFrame = false;
						nsguiView->MouseButtonUp(x, y, Noesis::MouseButton_Left);
					}
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
			
		} //End of operation loop. Everything after this is cleanup.

		//NSGUI stuff should be manually shut down before exiting the program.
		//All Noesis::Ptr objects must be reset to free them because they are, in fact, pointers.
		nsguiView.Reset();
		//Most of the resetting happens in here.
		ui.UIReset();

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