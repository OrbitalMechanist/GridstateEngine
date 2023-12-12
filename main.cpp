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

		renderer.loadTexture("assets/textures/stone_simple.png", "stone");
		renderer.loadTexture("assets/textures/surface_simple.png", "surface");
		renderer.loadTexture("assets/textures/AK74.png", "ak_texture");
		renderer.loadTexture("assets/textures/tree_texture.png", "tree_texture");
		renderer.loadTexture("assets/textures/white.png", "white");
		renderer.loadTexture("assets/textures/light_rock_texture.jpg", "light_rock_texture");
		renderer.loadTexture("assets/textures/red_blue_texture.jpg", "red_blue_texture");
		renderer.loadTexture("assets/textures/grass 2.jpg", "grass");
		renderer.loadTexture("assets/textures/bush_texture.png", "bush_texture");
		renderer.loadTexture("assets/textures/rock_texture.jpg", "rock_texture");

		renderer.loadModel("assets/models/ak74.fbx", "ak");
		renderer.loadModel("assets/models/bushTree.fbx", "tree");
		renderer.loadModel("assets/models/leafyTree.fbx", "pine_tree");
		renderer.loadModel("assets/models/leafyTree2.fbx", "pine_tree2");
		renderer.loadModel("assets/models/lightColorRock.fbx", "rock_light");
		renderer.loadModel("assets/models/rocks.fbx", "rocks");
		renderer.loadModel("assets/models/singleBigRock.fbx", "rock_big");
		renderer.loadModel("assets/models/bush.fbx", "bush");
		renderer.loadModel("assets/models/character.fbx", "character");
		renderer.loadModel("assets/models/cursor.obj", "cursor");
		renderer.loadModel("assets/models/enemy.fbx", "enemy");


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
		entityManager.registerComponentType<NameComponent>();

		GameMaster* gm = new GameMaster(&entityManager, &audioManager);


		Entity newEntity = entityManager.createEntity();
		Entity entity2 = entityManager.createEntity();

		TransformComponent trans;
		trans.pos = { 15, 0 };

		renderer.createMaterial("surfaceMaterial", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0);
		renderer.createMaterial("cursorMaterial", glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0);

		StaticMeshComponent stat;
		stat.modelName = "cursor";
		stat.textureName = "white";
		stat.shaderName = "basic";
		stat.materialName = "cursorMaterial";
		stat.posOffset = { 0.0f, 0.0f, 0.5f };

		Entity cursorEnt = entityManager.createEntity();
		entityManager.addComponent<TransformComponent>(cursorEnt, trans);
		entityManager.addComponent<StaticMeshComponent>(cursorEnt, stat);


		stat.modelName = "ak";
		stat.textureName = "stone";
		stat.shaderName = "basic";
		stat.materialName = "surfaceMaterial";
		stat.posOffset = { 0.0f, 0.0f, 0.0f };

		//stat.materialName = "surfaceMaterial"

		stat.modelName = "cube";
		bool swapTex = false;

		//World setup
		for (int x = 0; x <= 10; x++) {
			for (int y = 0; y <= 10; y++) {
				Entity fresh = entityManager.createEntity();
				trans.pos = { x, y };
				stat.modelName = "cube";
				stat.textureName = "grass";
				stat.shaderName = "basic";
				//stat.posOffset = { 0.0f, 0.0f, -0.5f };
				entityManager.addComponent<TransformComponent>(fresh, trans);
				entityManager.addComponent<StaticMeshComponent>(fresh, stat);
			}
		}

		//Initalize all components
		PlayerComponent playComp(0);
		MoveComponent moveComp;
		AttackComponent atkComp;
		HealthComponent hpComp;
		ObstacleComponent obComp;
		AudioComponent audio;
		NameComponent allyNameComp;
		//NameComponent enemyNameComp;

		//Array containing all the entities names 
		std::string entityName[] = { "Player", "Player", "Rock", "Rock", "Rock", "Rock",
							  "Rock", "Rock", "Rock", "Rock", "Rock", "Rock", "Tree",
							  "Tree", "Tree", "Bush", "Bush", "Bush", "Bush", "Bush" };
		//Array containing all the positions of the entities in same order as entityName
		glm::ivec2 positions[] = { {0,3}, {2,1}, {1,4}, {2,4}, {3,4}, {5,4}, {1,6},
								   {3,7}, {3,8}, {6,7}, {6,2}, {9,5}, {1,8}, {8,8},
								   {9,2}, {0,4}, {3,2}, {4,9}, {6,3}, {9,7} };
		//Array containing all the textures of the entities in same order as entityName
		std::string tex[] = { "red_blue_texture", "red_blue_texture", "rock_texture", "rock_texture","rock_texture",
							   "rock_texture", "rock_texture", "rock_texture", "rock_texture","rock_texture",
							   "rock_texture", "rock_texture", "tree_texture", "tree_texture","tree_texture",
							   "bush_texture","bush_texture","bush_texture", "bush_texture", "bush_texture",
							   "bush_texture"};
		//Array containing all the models of the entities in same order as entityName
		std::string modelName[] = { "character","character","rock_big","rock_big","rock_big",
									"rock_big","rock_big","rock_big","rock_big","rock_big",
									"rock_big","rock_big", "tree","tree","tree","bush",
									"bush", "bush", "bush", "bush" };
		//Array with damage of the 2 players
		int damage[] = { 3, 4 };
		//Array with attack ranges of the 2 players
		int atkRanges[] = { 3,2 };
		//Array with the move ranges of the 2 players
		int moveRanges[] = { 2,1 };
		//Array holding all the health of the entities in same order as entityName
		int health[] = { 5, 8, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8, 8, 8, 1, 1, 1, 1, 1 };
		//Array holding all the armor of the 2 players
		int armor[] = { 1, 2 };

		//stat.posOffset.z += 0f;
		//Independent counting variable for player entities array specific variables
		int playerCount = 0;
		//Move component moved is intially false for all players
		moveComp.moved = false;

		//Ceates every entity with the arrays above in a for loop
		for (int i = 0; i < 20; i++) {
			Entity newEntity = entityManager.createEntity();
			//Position of entity
			trans.pos = positions[i];
			//Model and texture of the entity
			stat.modelName = modelName[i];
			stat.textureName = tex[i];
			//Health of the entity
			hpComp.health = health[i];
			hpComp.maxHealth = health[i];

			//Adds all components that are the same for player and objects into the manager
			entityManager.addComponent<TransformComponent>(newEntity, trans);
			entityManager.addComponent<StaticMeshComponent>(newEntity, stat);
			entityManager.addComponent<AudioComponent>(newEntity, audio);

			//Adds player specific components
			if (entityName[i] == "Player") {
				stat.posOffset.z = 0.05f;
				//Player damage component
				atkComp.damage = damage[playerCount];
				atkComp.range = atkRanges[playerCount];
				//Player move range component
				moveComp.moveRange = moveRanges[playerCount];
				//Name of the player entity
				allyNameComp.name = "Ally Wizard";
				//Armor of the player entity
				hpComp.armor = armor[playerCount];
				//Adds player specific components to the entity into the manager
				entityManager.addComponent<AttackComponent>(newEntity, atkComp);
				entityManager.addComponent<MoveComponent>(newEntity, moveComp);
				entityManager.addComponent<PlayerComponent>(newEntity, playComp);
				entityManager.addComponent<NameComponent>(newEntity, allyNameComp);
				//Increments player specific array count
				playerCount++;
			}
			else {
				//If not player its an obstacle so add obstacle component
				entityManager.addComponent<ObstacleComponent>(newEntity, obComp);
			}
			//Adds hp + armour to the hpcomp if player and only hp if obstacle
			entityManager.addComponent<HealthComponent>(newEntity, hpComp);
			hpComp.armor = 0;
		}

		// AI setup
		MessageBus bus;
		AISystem aiSystem(entityManager, bus, *gm);
		glm::vec2 aiPos[] = { {1,9}, {6,8}, {10,3} };
		const int numOfEnemy = 3;
		// Generate Enemy
		for (int i = 0; i < numOfEnemy; i++) {
			trans.pos = aiPos[i];
			stat.modelName = "enemy";  // replace this with actual model
			stat.textureName = "red_blue_texture";
			aiSystem.spawnEnemy(trans, stat);
		}
		
		UIController ui("everything.xaml", gm, entityManager);
		//End Noesis setup (actually ending noesis happens at the very end)

		auto nsguiView = ui.GetNsguiView();

		static auto startTime = std::chrono::high_resolution_clock::now();
		static float prevTime = 0;

		int prevWidth = WINDOW_WIDTH;
		int	prevHeight = WINDOW_HEIGHT;

		auto turnBtn = ui.GetturnBtn();

		Renderer* rendPtr = &renderer; //things seem to get copied around so referencing the actual object
		//causes problems
		void* emPtr = &entityManager;
		Entity* entPtr = &entity2;
		bool isPlayerTurn = true;
		//void* gmPtr = &gm;

		//This is for finding out if LMB was released on this frame.
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
			audioManager.setDevicePosition(camPos);
			audioManager.setDeviceOrientation(trueFwd, trueUp);

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

			//Set grid cursor position
			entityManager.getComponent<TransformComponent>(cursorEnt).pos = { gridPositionX, gridPositionY };

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
				if (!lmbDownPrevFrame) {
					lmbDownPrevFrame = true;
					nsguiView->MouseButtonDown(x, y, Noesis::MouseButton_Left);

					Noesis::HitTestResult uiHitTest = Noesis::VisualTreeHelper::HitTest(
						Noesis::VisualTreeHelper::GetRoot(turnBtn), Noesis::Point{static_cast<float>(x), static_cast<float>(y)});

					if (uiHitTest.visualHit == nullptr) {
						if (gm->currentMode == select) {
							gm->selectUnit(gridPositionX, gridPositionY);
							if (gm->selected != NULL) {
								std::cout << "\nNOT NULL";
								
								ui.DisplayInfoPanel(gm->selected);
								
								if (!gm->botSelected) {
									if (entityManager.getComponent<MoveComponent>(gm->selected).moved) {
										ui.SetMoveIcon(true);
									}
									else {
										ui.SetMoveIcon(false);
									}
								}
								else {
									ui.HideMoveIcon();
								}
							}
						}
						else if (gm->currentMode == move) {
							bool moved = gm->moveSelected(gridPositionX, gridPositionY);
							if (moved) {
								ui.SetMoveIcon(true);
								ui.HighlightSelectMode();
								gm->currentMode = select;
							}
						}
						else {
							bool hit = gm->attackSelected(gridPositionX, gridPositionY);
							if (hit) {
								ui.SetMoveIcon(true);
								ui.HighlightSelectMode();
								gm->currentMode = select;
							}
						}
						if (gm->selected == NULL) {
							ui.HideInfoPanel();
						}
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
			aiSystem.update();



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