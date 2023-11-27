#include "graphics/Renderer.h"
#include "Constants.h"
#include "MapGrid.cpp"

#include <chrono>

/*
	This file is just for testing, to be removed once we have our graphical engine ready.
	The code here currently lives in main.cpp for testing purposes, I'm keeping a double of
	this file for when we've built testing infrastructure for the engine. - Joe
*/

static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Much of this code is what you'd originally see in main, just isolated to what's needed for this test
int MapGridRenderTest() {
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
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

		Renderer renderer = Renderer(window, WINDOW_WIDTH, WINDOW_HEIGHT);

		renderer.loadTexture("assets/textures/stone_simple.png", "stone");
		renderer.loadTexture("assets/textures/surface_simple.png", "surface");

		renderer.loadShaderProgram("shaders/basic.vert", "", "shaders/basic.frag", "basic");
		renderer.loadShaderProgram("shaders/secondary.vert", "", "shaders/secondary.frag", "secondary");

		renderer.setBackgroundColor({ 0.1f, 0.1f, 0.1f, 1.0f });

		glm::vec3 camRot{ 0.0f, 0.0f, 0.0f };
		glm::vec3 camPos{ 0.0f, 0.0f, 10.0f };

		renderer.setAmbientLight("basic", glm::vec3(0.15f, 0.15f, 0.15f));

		// initialize the MapGrid and fill it with things to draw later
		int mapRows = 10;
		int mapCols = 10;
		
		MapGrid testMap = MapGrid(mapRows, mapCols);

		for (int i = 0; i < mapRows; ++i) {
			for (int j = 0; j < mapCols; ++j) {
				if ((i + j) % 2 == 1) {
					testMap.setCellContent(i, j, "X");
				}
			}
		}

		//print grid output for testing
		/*
		for (int i = 0; i < mapRows; ++i) {
			for (int j = 0; j < mapCols; ++j) {
				std::cout << testMap.getCellValue(i, j) << "  ";
			}
		}
		*/

		static auto startTime = std::chrono::high_resolution_clock::now();
		while (!glfwWindowShouldClose(window)) {
			auto currentTime = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

			renderer.clearFrame();

			renderer.setCameraPosition(camPos);
			renderer.setCameraRotation(camRot);
			
			// Loop over every cell on the map, and draw the according tile
			for (int i = 0; i < mapRows; ++i) {
				for (int j = 0; j < mapCols; ++j) {
					if (testMap.getCellValue(i, j) == "X") {
						renderer.drawByNames("cube", "stone", "singleMaterial", "basic",
							{ i,j,0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
					}
					else {
						renderer.drawByNames("cube", "surface", "surfaceMaterial", "basic",
							{ i,j,0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
					}
				}
			}

			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
				camRot.x += glm::radians(1.0f);
			}
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
				camRot.x -= glm::radians(1.0f);
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
				camRot.z -= glm::radians(1.0f);
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
				camRot.z += glm::radians(1.0f);
			}

			glm::mat4 movementRotation = glm::rotate(glm::mat4(1.0f), camRot.z, { 0.0f, 0.0f, 1.0f });
			movementRotation = glm::rotate(movementRotation, camRot.x, { 1.0f, 0.0f, 0.0f });
			movementRotation = glm::rotate(movementRotation, camRot.y, { 0.0f, 1.0f, 0.0f });

			//The camera point down by default, so the initial forward is -Z
			glm::vec4 trueFwd = movementRotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
			glm::vec4 trueRight = movementRotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			glm::vec4 trueUp = movementRotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);


			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				camPos += glm::vec3(trueFwd.x, trueFwd.y, trueFwd.z) * 0.05f;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				camPos += glm::vec3(trueFwd.x, trueFwd.y, trueFwd.z) * -0.05f;
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				camPos += glm::vec3(trueRight.x, trueRight.y, trueRight.z) * 0.05f;
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				camPos += glm::vec3(trueRight.x, trueRight.y, trueRight.z) * -0.05f;
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				camPos += glm::vec3(trueUp.x, trueUp.y, trueUp.z) * 0.05f;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
				camPos += glm::vec3(trueUp.x, trueUp.y, trueUp.z) * -0.05f;
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				camPos.z += 0.05f;
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
				camPos.z += -0.05f;
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}