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
int main() {
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
		renderer.loadTexture("assets/textures/AK74.png", "ak_texture");

		renderer.loadModel("assets/models/ak74.fbx", "ak");
		renderer.loadModel("assets/models/cone45.obj", "cone");

		renderer.loadShaderProgram("shaders/basic.vert", "", "shaders/basic.frag", "basic");
		renderer.loadShaderProgram("shaders/secondary.vert", "", "shaders/secondary.frag", "secondary");

		renderer.setBackgroundColor({ 0.1f, 0.1f, 0.1f, 1.0f });

		glm::vec3 camRot{ 0.0f, 0.0f, 0.0f };
		glm::vec3 camPos{ 0.0f, 0.0f, 10.0f };

		renderer.setAmbientLight("basic", glm::vec3(0.15f, 0.15f, 0.15f));

		renderer.setLightState("basic", 0, 2, { 0.0f, 4.5f, 1.0f }, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
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

		static auto startTime = std::chrono::high_resolution_clock::now();
		static float prevTime = 0;
		while (!glfwWindowShouldClose(window)) {
			auto currentTime = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
			float deltaTime = time - prevTime;

			//renderer.clearFrame();

			renderer.setCameraPosition(camPos);
			renderer.setCameraRotation(camRot);

			//Visible
			renderer.addRenderObject(RenderObject("cube", "stone", "basic",
				{ 5.0f, 5.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }));

			renderer.addRenderObject(RenderObject("cube", "stone", "basic",
				{ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, glm::radians(45.0f) }, { 1.0f, 1.0f, 1.0f }));

			renderer.addRenderObject(RenderObject("cube", "stone", "secondary",
				{ 0.0f, 0.0f, -3.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }));

			renderer.addRenderObject(RenderObject("cube", "stone", "basic",
				{ -5.0f, -5.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }));

			renderer.addRenderObject(RenderObject("cube", "stone", "basic",
				{ 0.5f * sin(time * 5.0f), 3.5f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 1.0f }));

			renderer.addRenderObject(RenderObject("cube", "stone", "basic",
				{ 1.0f, 4.5f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }));

			renderer.addRenderObject(RenderObject("ak", "ak_texture", "basic",
				{ 0.0f, 0.0f, 1.53f }, { 0.0f, glm::radians(90.0f),
				glm::radians(117.0f + time * 90.0f) }, { 1.0f, 1.0f, 1.0f }));

			for (int x = 0; x < 10; x++) {
				for (int y = 0; y < 10; y++) {
					renderer.addRenderObject(RenderObject("cube", "surface", "basic",
						{ x - 4.5f, y - 4.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }));
				}
			}

			for (int y = 0; y <= 12; y += 2) {
				renderer.addRenderObject(RenderObject("cube", "stone", "basic", { 10.0f, y + 1.0f, 0.0f },
					{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }));
			}

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


			prevTime = time;

			renderer.renderFromQueue(true);

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