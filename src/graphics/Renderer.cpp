#include "graphics/Renderer.h"

Renderer::Renderer(GLFWwindow* creatorWindow, uint32_t windowWidth, uint32_t windowHeight) {
	window = creatorWindow;
	windowXsize = windowWidth;
	windowYsize = windowHeight;

	if (window == NULL) {
		throw std::runtime_error("Unable to initialize renderer due to null window.");
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TODO: This is scuffed and needs refactoring. Shader program loading needs to be split into
	//a creator function and a map setter.
	//Maybe make the ShaderProgram class responsible for its own loading, but I'd prefer this Renderer class
	//be responsible for as much GL interaction as possible for ease of maintenance and explaining to others.
	loadShaderProgram("shaders/SYSTEM_shadow.vert", "", "shaders/SYSTEM_shadow.frag", "SYSTEMSHADOW");
	shadowShader = shaderPrograms["SYSTEMSHADOW"];
	shaderPrograms.erase("SYSTEMSHADOW");

	loadShaderProgram("shaders/SYSTEM_shadow.vert", "shaders/SYSTEM_cubeShadow.geom",
		"shaders/SYSTEM_cubeShadow.frag", "SYSTEMCUBESHADOW");
	cubeShadowShader = shaderPrograms["SYSTEMCUBESHADOW"];
	shaderPrograms.erase("SYSTEMCUBESHADOW");

	for (int i = 0; i < NUM_LIGHTS; i++) {
		shadowMaps[i] = createShadowMap();
		shadowCubemaps[i] = createShadowCubeMap();
	}

	createCubeModel();
}

void Renderer::updateWindowSize(int newX, int newY) {
	windowXsize = newX;
	windowYsize = newY;
}

void Renderer::createCubeModel() {
	std::vector<Vertex> cubeBuffer {
		//
		{ {-0.5f, 0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
		{ {0.5f, 0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
		{ {-0.5f, -0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
		{ {0.5f, -0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
		//
		{ {-0.5f, 0.5f, -0.5f}, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } },
		{ {0.5f, -0.5f, -0.5f}, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
		{ {-0.5f, -0.5f, -0.5f}, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
		{ {0.5f, 0.5f, -0.5f}, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },
		//
		{ {-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },
		{ {-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },
		{ {-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
		{ {-0.5f, -0.5f, -0.5f }, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
		//
		{ {0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },
		{ {0.5f, -0.5f, -0.5f }, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
		{ {0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
		{ {0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },
		//
		{ {-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f} },
		{ {0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f} },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, { 0.0f, 0.0f} },
		{ {0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f} },
		//
		{ {-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} },
		{ {0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
		{ {-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, { 0.0f, 0.0f} },
		{ {0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} }
	};

	std::vector<GLuint> indexBuffer{
			0, 2, 1, 2, 3, 1,
			4, 5, 6, 5, 4, 7,
			9, 8, 10, 10, 11, 9,
			12, 13, 14, 13, 12, 15,
			19, 17, 18, 18, 17, 16,
			20, 21, 22, 20, 23, 21
	};

	GL_VertexArrayObject vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GL_VertexBufferObject vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, cubeBuffer.size() * sizeof(Vertex), cubeBuffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * 2 * sizeof(float)));
	glEnableVertexAttribArray(2);

	GL_ElementBufferObject ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(GLuint),
		indexBuffer.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	models["cube"] = Mesh(vao, vbo, ebo, indexBuffer.size());
}

GL_Shader Renderer::loadShader(const std::string& path, GLenum shaderStage) {
	std::ifstream ifs{path};

	if (!ifs.is_open()) {
		//Throwing an exception seems like slight overkill because the program will still be able to
		//continue operating. It'll just have a broken shader program.
		std::cerr << "Failed to open shader at path: " << path << std::endl;
		return 0;
	}

	ifs.seekg(0, ifs.end);
	std::streampos len = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	
	if (len == 0) {
		std::cerr << "Alleged shader file " << path << " is empty!" << std::endl;
		return 0;
	}

	std::vector<char> buffer(len);

	ifs.seekg(0);
	ifs.read(buffer.data(), len);

	ifs.close();

	char* pBufferData = buffer.data();

	GL_Shader result;
	result = glCreateShader(shaderStage);
	glShaderSource(result, 1, &pBufferData, NULL);
	glCompileShader(result);

	int compileStatus;
	glGetShaderiv(result, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		std::vector<char> infoLog(512);
		glGetShaderInfoLog(result, 512, NULL, infoLog.data());
		std::cerr << "Shader " << path << " compile failure:\n" << infoLog.data() << std::endl;
	}

	return result;
}

bool Renderer::loadShaderProgram(const std::string& vertPath, const std::string& geometryPath,
	const std::string& fragPath, const std::string& resultName) {
	if (shaderPrograms.contains(resultName)) {
		std::cout << "Warning: shader program called \"" << resultName
			<< "\" is already loaded and will be deleted and replaced." << std::endl;
		removeShaderProgramByName(resultName);
	}

	GL_Shader vert = loadShader(vertPath, GL_VERTEX_SHADER);
	GL_Shader frag = loadShader(fragPath, GL_FRAGMENT_SHADER);

	if (vert == 0 || frag == 0) {
		std::cerr << "Mandatory shader stage failed creation; Shader Program \"" 
			<< resultName << "\" not created." << std::endl;
		return false;
	}


	GL_Shader geo;
	if (!geometryPath.empty()) {
		geo = loadShader(geometryPath, GL_GEOMETRY_SHADER);
	}

	GL_ShaderProgram shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vert);
	glAttachShader(shaderProgram, frag);
	if (!geometryPath.empty()) {
		glAttachShader(shaderProgram, geo);
	}
	glLinkProgram(shaderProgram);

	int linkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		std::vector<char> infoLog(512);
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog.data());
		std::cerr << "Shader program link failure; Shader Program \""
			<< resultName << "\" not created. Log:\n" << infoLog.data() << std::endl;
		return false;
	}

	shaderPrograms[resultName] = ShaderProgram(shaderProgram);
	shaderPrograms[resultName].queryUniformLocations();

	glDeleteShader(vert);
	glDeleteShader(frag);

	return true;
}

void Renderer::setBackgroundColor(const glm::vec4& color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

bool Renderer::loadTexture(const std::string& path, const std::string& resultName) {
	if (textures.contains(resultName)) {
		std::cout << "Warning: texture called \"" << resultName
			<< "\" is already loaded and will be deleted and replaced." << std::endl;
		removeTextureByName(resultName);
	}

	int xSize, ySize, texChannels;
	stbi_uc* imageBuffer = stbi_load(path.c_str(), &xSize, &ySize, &texChannels, STBI_rgb_alpha);

	if (!imageBuffer) {
		std::cerr << "Failed to open texture file; Texture \"" 
			<< resultName << "\" was not created." << std::endl;
		return 0;
	}

	GL_Texture result;
	glGenTextures(1, &result);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, xSize, ySize, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(imageBuffer);

	textures[resultName] = result;

	return true;
}

void Renderer::drawByNames(const std::string& modelName, const std::string& textureName, const std::string& shaderName,
	const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) {
	
	bool modelPresent = models.contains(modelName);
	bool texturePresent = textures.contains(textureName);
	bool shaderPresent = shaderPrograms.contains(shaderName);
	if (!modelPresent) {
		std::cerr << "No model \"" << modelName << "\" loaded; cannot draw." << std::endl;
	}
	if (!texturePresent) {
		std::cerr << "No texture \"" << textureName << "\" loaded; cannot draw." << std::endl;
	}
	if (!shaderPresent) {
		std::cerr << "No shader program \"" << shaderName << "\" loaded; cannot draw." << std::endl;
	}
	if (!modelPresent || !texturePresent || !shaderPresent) {
		return;
	}

	ShaderProgram& sp = shaderPrograms[shaderName];

	glUseProgram(sp.getGLReference());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[textureName]);
	glUniform1i(sp.referenceUniforms().diffuseTex, 0);

	glm::mat4 model = glm::mat4(1.0f);
	
	model = glm::translate(model, pos);

	model = glm::rotate(model, rot.z, { 0.0f, 0.0f, 1.0f });
	model = glm::rotate(model, rot.y, { 0.0f, 1.0f, 0.0f });
	model = glm::rotate(model, rot.x, { 1.0f, 0.0f, 0.0f });

	model = glm::scale(model, scale);

	glm::mat4 cam = glm::translate(glm::mat4(1.0f), cameraPos);
	cam = glm::rotate(cam, cameraRot.z, {0.0f, 0.0f, 1.0f});
	cam = glm::rotate(cam, cameraRot.x, { 1.0f, 0.0f, 0.0f });

	glm::mat4 view = glm::inverse(cam);

		//glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
		//glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
		//

	glm::mat4 projection = glm::perspective(glm::radians(60.0f),
		windowXsize / (float)windowYsize, 0.1f, 100.0f);

	glm::mat4 mvp = projection * view * model;

	glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

	std::vector<GLuint> textureSamplerTargets{NUM_LIGHTS};

	for (GLuint i = 0; i < NUM_LIGHTS; i++) {
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, shadowMaps[i].getMap());
		glUniformMatrix4fv(sp.referenceUniforms().lightSpaceMatrixFirstElement + i, 1, GL_FALSE,
			glm::value_ptr(shadowMaps[i].getLightSpaceMatrix()));
		glUniform1i(sp.referenceUniforms().shadowMapFirstElement + i, 1 + i);

		glActiveTexture(GL_TEXTURE1 + NUM_LIGHTS + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubemaps[i].getCubemap());
		glUniform1i(sp.referenceUniforms().shadowCubemapFirstElement + i, 1 + NUM_LIGHTS + i);
	}

	glUniformMatrix4fv(sp.referenceUniforms().mvp, 1, false, glm::value_ptr(mvp));
	glUniformMatrix4fv(sp.referenceUniforms().normMat, 1, false, glm::value_ptr(normalMatrix));
	glUniformMatrix4fv(sp.referenceUniforms().modelMat, 1, false, glm::value_ptr(model));

	glBindVertexArray(models[modelName].getVAO());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, models[modelName].getEBO());

	glDrawElements(GL_TRIANGLES, models[modelName].getIndexCount(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::clearFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setCameraRotation(const glm::vec3& rot) {
	cameraRot = rot;
}

void Renderer::setCameraPosition(const glm::vec3& pos) {
	cameraPos = pos;
}


glm::vec3 Renderer::getCameraRotation() {
	return cameraRot;
}

glm::vec3 Renderer::getCameraPosition() {
	return cameraPos;
}

bool Renderer::loadModel(const std::string& path, const std::string& resultName) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate |
		aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_FlipUVs); //may need to flip UVs

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Model loading failure: " << import.GetErrorString() << std::endl;
		return false;
	}

	if (models.contains(resultName)) {
		std::cout << "Warning: model called \"" << resultName 
			<< "\" is already loaded and will be deleted and replaced." << std::endl;
		removeModelByName(resultName);
	}

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	//Assimp supports models made up of multiple submeshes, each with their own vertex and index buffer
	//that each start at 0.
	//I, however, don't support that quite yet, so all the meshes have to be merged into one.
	//For that to happen, the indices of non-first meshes have to be changed to account for the vertices
	//that were placed into vertex buffer before the first vertex of *their* mesh.
	GLuint indexOffset = 0;

	for (size_t i = 0; i < scene->mNumMeshes; i++) {
		auto mesh = scene->mMeshes[i];
		for (size_t j = 0; j < mesh->mNumVertices; j++) {
			auto srcVert = mesh->mVertices[j];
			auto srcNorm = mesh->mNormals[j];
			Vertex v;
			v.pos = glm::vec3(srcVert.x, srcVert.y, srcVert.z);
			v.norm = glm::vec3(srcNorm.x, srcNorm.y, srcNorm.z);

			if (mesh->mTextureCoords[0] != nullptr) {
				auto srcTexCoord = mesh->mTextureCoords[0][j];
				v.texCoord = glm::vec2(srcTexCoord.x, srcTexCoord.y);
			}
			else {
				v.texCoord = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(v);
		}

		for (size_t k = 0; k < mesh->mNumFaces; k++) {
			//There should always be exactly three indices per face but in case of floating edges or something this is a loop.
			for (unsigned char l = 0; l < mesh->mFaces[k].mNumIndices; l++) {
				indices.push_back(mesh->mFaces[k].mIndices[l] + indexOffset);
			}
		}

		indexOffset = vertices.size();
	}

	GL_VertexArrayObject vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GL_VertexBufferObject vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * 2 * sizeof(float)));
	glEnableVertexAttribArray(2);

	GL_ElementBufferObject ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
		indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	models[resultName] = Mesh(vao, vbo, ebo, indices.size());
}

bool Renderer::removeTextureByName(const std::string& name) {
	if (!textures.contains(name)) {
		std::cerr << "No texture named \"" << name << "\" to remove." << std::endl;
		return false;
	}

	glDeleteTextures(1, &textures[name]);

	return true;
}

bool Renderer::removeShaderProgramByName(const std::string& name) {
	if (!shaderPrograms.contains(name)) {
		std::cerr << "No shader program named \"" << name << "\" to remove." << std::endl;
		return false;
	}

	glDeleteProgram(shaderPrograms[name].getGLReference());

	shaderPrograms.erase(name);

	return true;
}

bool Renderer::removeModelByName(const std::string& name) {
	if (!models.contains(name)) {
		std::cerr << "No model named \"" << name << "\" to remove." << std::endl;
		return false;
	}

	GL_VertexArrayObject victimVAO = models[name].getVAO();
	GL_VertexBufferObject victimVBO = models[name].getVBO();
	GL_ElementBufferObject victimEBO = models[name].getEBO();

	glDeleteVertexArrays(1, &victimVAO);
	glDeleteBuffers(1, &victimVBO);
	glDeleteBuffers(1, &victimEBO);

	return true;
}


bool Renderer::setLightState(const std::string& usableShaderName, size_t lightIndex, GLuint type,
	const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& color, GLfloat intensity,
	GLfloat angle, GLfloat distanceLimit, GLfloat attenuationMax) {
	if (lightIndex >= lights.size()) {
		std::cerr << "Light to set is out of range; no effect from function." << std::endl;
		return false;
	} if (!shaderPrograms.contains(usableShaderName)) {
		std::cerr << "Shader program \"" << usableShaderName <<
			"\" referenced when setting lights does not exist; no effect from function." << std::endl;
		return false;
	}

	lights[lightIndex].type = type;
	lights[lightIndex].position = pos;
	lights[lightIndex].direction = dir;
	lights[lightIndex].color = color;
	lights[lightIndex].intensity = intensity;
	lights[lightIndex].angle = angle;
	lights[lightIndex].distanceLimit = distanceLimit;
	lights[lightIndex].attenuationMax = attenuationMax;

	glm::mat4 proj;
	glm::mat4 view;

	if (lights[lightIndex].type == 1) {
		glm::vec3 lightSourcePos = -1.0f * lights[lightIndex].direction;
		proj = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, -15.0f, 15.0f); //calculation bounding box, constant for now
		view = glm::lookAt(lightSourcePos, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	}
	else if(lights[lightIndex].type == 3) {
		proj = glm::perspective(lights[lightIndex].angle, 1.0f, 0.1f,
			lights[lightIndex].distanceLimit > 0 ? lights[lightIndex].distanceLimit : 100);
		view = glm::lookAt(lights[lightIndex].position,
			lights[lightIndex].position + lights[lightIndex].direction, glm::vec3(0, 0, 1));
	}
	else if (lights[lightIndex].type == 2) {
		proj = glm::perspective(glm::radians(90.0f),
			static_cast<float>(shadowMapXsize) / shadowMapYsize, 0.1f,
			lights[lightIndex].distanceLimit > 0 ? lights[lightIndex].distanceLimit : 100);

		//based on https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows
		shadowCubemaps[lightIndex].updateMatrices(
			proj * glm::lookAt(lights[lightIndex].position, 
				lights[lightIndex].position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			proj * glm::lookAt(lights[lightIndex].position,
				lights[lightIndex].position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			proj * glm::lookAt(lights[lightIndex].position,
				lights[lightIndex].position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			proj * glm::lookAt(lights[lightIndex].position,
				lights[lightIndex].position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
			proj * glm::lookAt(lights[lightIndex].position,
				lights[lightIndex].position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			proj * glm::lookAt(lights[lightIndex].position,
				lights[lightIndex].position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
		);
	}

	glm::mat4 lsm = proj * view;

	shadowMaps[lightIndex].updateLightSpaceMatrix(lsm);

	const LightLayout& element = shaderPrograms[usableShaderName].referenceUniforms().lights[lightIndex];

	glUseProgram(shaderPrograms[usableShaderName].getGLReference());

	glUniform1i(element.type, lights[lightIndex].type);
	glUniform3f(element.position, lights[lightIndex].position.x,
		lights[lightIndex].position.y, lights[lightIndex].position.z);
	glUniform3f(element.direction, lights[lightIndex].direction.x,
		lights[lightIndex].direction.y, lights[lightIndex].direction.z);
	glUniform3f(element.color, lights[lightIndex].color.r,
		lights[lightIndex].color.g, lights[lightIndex].color.b);
	glUniform1f(element.intensity, lights[lightIndex].intensity);
	glUniform1f(element.angle, lights[lightIndex].angle);
	glUniform1f(element.distanceLimit, lights[lightIndex].distanceLimit);
	glUniform1f(element.attenuationMax, lights[lightIndex].attenuationMax);

	glUseProgram(0);

	return true;
}

void Renderer::setAmbientLight(const std::string& usableShaderName, const glm::vec3& ambient) {
	if (!shaderPrograms.contains(usableShaderName)) {
		std::cerr << "Shader program \"" << usableShaderName <<
			"\" referenced when ambient light does not exist; no effect from function." << std::endl;
		return;
	}
	glUseProgram(shaderPrograms[usableShaderName].getGLReference());
	glUniform3f(shaderPrograms[usableShaderName].referenceUniforms().ambientLight, ambient.r, ambient.g, ambient.b);
}

void Renderer::addRenderObject(RenderObject ro)
{
	renderQueue.push_back(ro);
}

void Renderer::renderFromQueue(bool clearBuffer)
{
	shadowRenderPass(clearBuffer);
	mainRenderPass(clearBuffer);
	renderQueue.clear();
}

ShadowMap Renderer::createShadowMap()
{
	GL_FrameBufferObject fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GL_Texture shadowMap;
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapXsize, shadowMapYsize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

	glClear(GL_DEPTH_BUFFER_BIT);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Failed shadow map creation at framebuffer.");
		return ShadowMap();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return ShadowMap(fbo, shadowMap);
}

ShadowCubeMap Renderer::createShadowCubeMap()
{
	GL_FrameBufferObject fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GL_Cubemap shadowCubemap;
	glGenTextures(1, &shadowCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubemap);
	for (GLuint i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			shadowMapXsize, shadowMapYsize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCubemap, 0);

	glClear(GL_DEPTH_BUFFER_BIT);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Failed shadow cubemap creation at framebuffer.");
		return ShadowCubeMap();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return ShadowCubeMap(fbo, shadowCubemap);
}

void Renderer::setupShadowRender() {
	glDepthMask(GL_TRUE);

	//this is widely considered a good idea but in my case it creates horrible 
	// peterpanning *without* quite fixing the acne.
	//glCullFace(GL_FRONT);

	glViewport(0, 0, shadowMapXsize, shadowMapYsize);
}

void Renderer::setupVisibleRender() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, windowXsize, windowYsize);

	glCullFace(GL_BACK);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::castShadow(GLuint lightIndex, const std::string& modelName, const glm::vec3& pos,
	const glm::vec3& rot, const glm::vec3& scale)
{
	glBindVertexArray(models[modelName].getVAO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, models[modelName].getEBO());

	glm::mat4 mvp;

	if (lights[lightIndex].type == 1 || lights[lightIndex].type == 3) {
		glm::mat4 proj;
		glm::mat4 view;

		if (lights[lightIndex].type == 1) {
			glm::vec3 lightSourcePos = -1.0f * lights[lightIndex].direction;
			//calculation bounding box, constant for now
			proj = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, -15.0f, 15.0f); 
			view = glm::lookAt(lightSourcePos, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
		}
		else {
			proj = glm::perspective(lights[lightIndex].angle, 1.0f, 0.1f, 
				lights[lightIndex].distanceLimit > 0 ? lights[lightIndex].distanceLimit : 100);
			view = glm::lookAt(lights[lightIndex].position, lights[lightIndex].position 
				+ lights[lightIndex].direction, glm::vec3(0, 0, 1));
		}

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, pos);

		model = glm::rotate(model, rot.z, { 0.0f, 0.0f, 1.0f });
		model = glm::rotate(model, rot.y, { 0.0f, 1.0f, 0.0f });
		model = glm::rotate(model, rot.x, { 1.0f, 0.0f, 0.0f });

		model = glm::scale(model, scale);

		mvp = proj * view * model;
	}
	else if (lights[lightIndex].type == 2) { //Point lights with cubemap shadows			
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, pos);

		model = glm::rotate(model, rot.z, { 0.0f, 0.0f, 1.0f });
		model = glm::rotate(model, rot.y, { 0.0f, 1.0f, 0.0f });
		model = glm::rotate(model, rot.x, { 1.0f, 0.0f, 0.0f });

		model = glm::scale(model, scale);

		mvp = model;
	}

	glUniformMatrix4fv(shadowShader.referenceUniforms().mvp, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawElements(GL_TRIANGLES, models[modelName].getIndexCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::prepareCubeShadowRenderForLight(GLuint lightIndex) {
	glUseProgram(cubeShadowShader.getGLReference());

	glBindFramebuffer(GL_FRAMEBUFFER, shadowCubemaps[lightIndex].getFBO());

	auto matrices = shadowCubemaps[lightIndex].getFaceShadowMatrices();

	glUniformMatrix4fv(cubeShadowShader.referenceUniforms().cubeShadowFacesFirstElement,
		1, GL_FALSE, glm::value_ptr(matrices[0]));
	glUniformMatrix4fv(cubeShadowShader.referenceUniforms().cubeShadowFacesFirstElement + 1,
		1, GL_FALSE, glm::value_ptr(matrices[1]));
	glUniformMatrix4fv(cubeShadowShader.referenceUniforms().cubeShadowFacesFirstElement + 2,
		1, GL_FALSE, glm::value_ptr(matrices[2]));
	glUniformMatrix4fv(cubeShadowShader.referenceUniforms().cubeShadowFacesFirstElement + 3,
		1, GL_FALSE, glm::value_ptr(matrices[3]));
	glUniformMatrix4fv(cubeShadowShader.referenceUniforms().cubeShadowFacesFirstElement + 4,
		1, GL_FALSE, glm::value_ptr(matrices[4]));
	glUniformMatrix4fv(cubeShadowShader.referenceUniforms().cubeShadowFacesFirstElement + 5,
		1, GL_FALSE, glm::value_ptr(matrices[5]));

	glUniform3fv(cubeShadowShader.referenceUniforms().cubeShadowLightPos, 1,
		glm::value_ptr(lights[lightIndex].position));

	glUniform1f(cubeShadowShader.referenceUniforms().cubeShadowDistanceLimit,
		lights[lightIndex].distanceLimit > 0 ? lights[lightIndex].distanceLimit : 100);
}

void Renderer::prepareFlatShadowRenderForLight(GLuint lightIndex)
{
	glUseProgram(shadowShader.getGLReference());

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMaps[lightIndex].getFBO());
}

void Renderer::shadowRenderPass(bool clearBuffer)
{
	setupShadowRender();
	for (GLuint i = 0; i < NUM_LIGHTS; ++i) {
		if (lights[i].type == 1 || lights[i].type == 3) {
			prepareFlatShadowRenderForLight(i);
		}
		else if (lights[i].type == 2) {
			prepareCubeShadowRenderForLight(i);
		}
		else {
			continue; //if the light isn't of a type we can handle, skip this loop iteration
		}

		if (clearBuffer) {
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		for (const RenderObject& ro : renderQueue) {
			if (ro.shadowing) {
				castShadow(i, ro.modelName, ro.position, ro.rotation, ro.scale);
			}
		}
	}
	setupVisibleRender();
}

void Renderer::mainRenderPass(bool clearBuffer)
{
	setupVisibleRender();
	if (clearBuffer) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	for (auto ro : renderQueue) {
		if (ro.visible) {
			drawByNames(ro.modelName, ro.textureName, ro.shaderName, ro.position, ro.rotation, ro.scale);
		}
	}
}
