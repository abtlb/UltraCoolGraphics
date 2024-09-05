#include <iostream>
#include <glad/glad.h>//loads functions to function pointers(must be before glfw)
#include <GLFW/glfw3.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"
#include "Texture.h"
#include <map>

int main();

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void processInput(GLFWwindow*);

struct Color
{
public:
	Color (float _r, float _g, float _b, float _a): r(_r), g(_g), b(_b), a(_a) {}
	float r;
	float g; 
	float b;
	float a;
};

int width = 1600, height = 900;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), 45.0f, (float)width / height);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	//initialize glfw,configure glfw options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a window
	GLFWwindow* window = glfwCreateWindow(width, height, "Epic stuff", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to initialize OpenGL window";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to Initialize GLAD";
		glfwTerminate();
		return -1;
	}
	
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);//callback function on window resize

	/*//rectangle
	float vertices[] =
	{
		//positions        color             tex coords
		-0.5f, -0.5, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,// bottom left
		-0.5f, 0.5, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top left
		0.5f, -0.5, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,// bottom right
		0.5f, 0.5, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f, 1.0f// top right
	};
	unsigned int indices[] =
	{
		0,2,3,
		1,3,0
	};
	 
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);//encapsulates VBOs, vertex attribs configurations
	unsigned int VBO;
	glGenBuffers(1, &VBO);//Generate 1 buffer with an ID stored in VBO
	//only one buffer can be bound to a buffer type, 
	//any calls to GL_ARRAY_BUFFER will be used on the currently bound buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

#pragma region Cube setup
	float cubeVertices[] =
	{
		// positions(3) // normals(3) // texture coords(2)
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	unsigned int cubeVBO;
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);//coordinates
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//tex coords
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
#pragma endregion

#pragma region Quad setup
	float quadVertices[] = 
	{
		// Positions      // Texture Coords
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // Top-right
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // Bottom-right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // Bottom-left
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f   // Top-left
	};

	unsigned int quadIndices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	unsigned int quadVAO;
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	unsigned int quadVBO;
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	unsigned int quadEBO;
	glGenBuffers(1, &quadEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);//coordinates
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));//tex coords
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

#pragma endregion



	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, 8 * sizeof(float), (void*)0);//coordinates
	glEnableVertexAttribArray(0);

#pragma region Load textures
	unsigned int texture1 = Texture::TextureFromFile("wall.jpg");
	unsigned int texture2 = Texture::TextureFromFile("awesomeface.png");
	unsigned int grassTexture = Texture::TransparentTextureFromFile("grass.png");
	unsigned int windowTexture = Texture::TransparentTextureFromFile("blending_transparent_window.png");
	unsigned int diffuseMap1 = Texture::TextureFromFile("container2.png");
	unsigned int specularMap1 = Texture::TextureFromFile("container2_specular.png");
#pragma endregion

#pragma region Shader definition
	Shader lightSourceShader("./lightSource.vert", "./lightSource.frag");
	Shader lightingShader("./lighting.vert", "./lighting.frag");
	Shader outlineShader("./lighting.vert", "./outline.frag");
	Shader grassShader("./grass.vert", "./grass.frag");
#pragma endregion

#pragma region Setup Rotating cubes
	float cubeRotSpeeds[10] = {2};
	for (int i = 0; i < 10; i++)
	{
		cubeRotSpeeds[i] = ((float)rand() / RAND_MAX) * 4 - 2;
	}
	glm::vec3 cubeRotAxis[10];
	for (int i = 0; i < 10; i++)
	{
		cubeRotAxis[i] = glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX));
	}
	glm::vec3 cubePositions[] = {
		//glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -6.5f),
		glm::vec3(-1.3f, 1.0f, -3.5f)
	};

#pragma endregion

#pragma region Setup windows
	std::vector<glm::vec3> windows;
	windows.push_back(glm::vec3(-1.5f * 2, -4.0f, -0.48f * 2));
	windows.push_back(glm::vec3(1.5f * 2, -4.0f, 0.51f * 2));
	windows.push_back(glm::vec3(0.0f * 2, -4.0f, 0.7f * 2));
	windows.push_back(glm::vec3(-0.3f * 2, -4.0f, -2.3f * 2));
	windows.push_back(glm::vec3(0.5f * 2, -4.0f, -0.6f * 2));
	std::map<float, glm::vec3> sortedWindows;
	for (int i = 0; i < windows.size(); i++)
	{
		float distance = glm::length(windows[i] - camera.cameraPos);
		sortedWindows[distance] = windows[i];
	}
#pragma endregion



	//setup mouse input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//capture the cursor and make it non-visible
	glfwSetCursorPosCallback(window, mouse_callback);

	Model backpackModel("C:/Users/balla/source/repos/UltraCoolGraphics/UltraCoolGraphics/Models/backpack/backpack.obj");

	glm::vec3 lightPos = glm::vec3(1.2f, 0.5f, 4.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//input
		processInput(window);

		//rendering commands
		glStencilMask(0x00);//for outline

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap1);

#pragma region Render Rotating Cubes
		//10 rotating cubes
		lightingShader.useProgram();
		int cubeNum = sizeof(cubePositions) / sizeof(glm::vec3);
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, cubeRotSpeeds[i] * (float)glfwGetTime() * glm::radians(45.0f), cubeRotAxis[i]);
			lightingShader.setMat4("model", model);

			glm::mat4 view = camera.getViewMat();
			glm::mat4 proj = camera.getProjMat();

			lightingShader.setMat4("view", view);
			lightingShader.setMat4("proj", proj);
			lightingShader.setFloat("material.texture_diffuse1", 0);//sampler
			lightingShader.setFloat("material.texture_specular1", 1);//sampler
			lightingShader.setFloat("material.shininess", 64.0f);
			//camera end
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
#pragma endregion

		glm::mat4 model, view, proj;

		//light source
		lightSourceShader.useProgram();
		glm::vec3 pointLightPositions[] =
		{
			glm::vec3(0.7f, 0.2f, 2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f, 2.0f, -12.0f),
			glm::vec3(0.0f, 0.0f, -3.0f)
		};
		for(int i = 0; i < 4; i++)
		{
			//inside factor: speed, outside factor: radius
			//lightPos = glm::vec3(sin(glfwGetTime() * 0.4f) * 2, .y, cos(glfwGetTime() * 0.4f) * 2) + glm::vec3(0, 0.0f, 3.0f);//rotate
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			view = camera.getViewMat();
			proj = camera.getProjMat();
			lightSourceShader.setMat4("model", model);
			lightSourceShader.setMat4("view", view);
			lightSourceShader.setMat4("proj", proj);
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightingShader.useProgram();
		lightingShader.setVec3("cameraPos", camera.cameraPos);

#pragma region Lighting stuff
		lightingShader.setVec3("dirLight.dir", glm::vec3(-0.2f, -1.0f, -0.3f));
		lightingShader.setVec3("dirLight.ambient", glm::vec3(0.1f));
		lightingShader.setVec3("dirLight.diffuse", glm::vec3(0.0, 0.0, 0.5));
		lightingShader.setVec3("dirLight.specular", glm::vec3(1.0f));

		for (int i = 0; i < 4; i++)
		{
			lightingShader.setVec3("pointLights[" + std::to_string(i) + "].pos", pointLightPositions[i]);
			lightingShader.setVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.0f));
			lightingShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f);
			lightingShader.setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f));

			lightingShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
			lightingShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.14f);
			lightingShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.07f);

		}

		lightingShader.setVec3("flashlight.pos", camera.cameraPos);
		lightingShader.setVec3("flashlight.dir", camera.cameraZ);
		lightingShader.setVec3("flashlight.ambient", glm::vec3(0.0f));
		lightingShader.setVec3("flashlight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f) * 0.0f);
		lightingShader.setVec3("flashlight.specular", glm::vec3(1.0f));
		lightingShader.setFloat("flashlight.constant", 1.0f);
		lightingShader.setFloat("flashlight.linear", 0.045f);
		lightingShader.setFloat("flashlight.quadratic", 0.0075f);
		lightingShader.setFloat("flashlight.innerCutOff", glm::cos(glm::radians(12.0f)));
		lightingShader.setFloat("flashlight.outerCutOff", glm::cos(glm::radians(15.0f)));
#pragma endregion

#pragma region Ground
		glm::vec3 groundPos = glm::vec3(0, -5, 0);
		glm::vec3 groundDimensions = glm::vec3(10, 1, 10);
		model = glm::mat4(1.0f);
		model = glm::translate(model, groundPos);
		model = glm::scale(model, groundDimensions);
		view = camera.getViewMat();
		proj = camera.getProjMat();
		lightingShader.setMat4("model", model);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("proj", proj);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap1);
		lightingShader.setFloat("material.texture_diffuse1", 0);//sampler
		lightingShader.setFloat("material.texture_specular1", 1);//sampler
		lightingShader.setFloat("material.shininess", 64.0f);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

#pragma endregion

#pragma region Grass
		/*std::vector<glm::vec3> vegetation;
		vegetation.push_back(glm::vec3(-1.5f * 2, -4.0f, -0.48f * 2));
		vegetation.push_back(glm::vec3(1.5f * 2, -4.0f, 0.51f * 2));
		vegetation.push_back(glm::vec3(0.0f * 2, -4.0f, 0.7f * 2));
		vegetation.push_back(glm::vec3(-0.3f * 2, -4.0f, -2.3f * 2));
		vegetation.push_back(glm::vec3(0.5f * 2, -4.0f, -0.6f * 2));

		grassShader.useProgram();
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		grassShader.setFloat("texture1", 0.6f);
		view = camera.getViewMat();
		proj = camera.getProjMat();
		grassShader.setMat4("view", view);
		grassShader.setMat4("proj", proj);
		for (unsigned int i = 0; i < vegetation.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, vegetation[i]);
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1));
			grassShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}*/

#pragma endregion

#pragma region Windows
		

		grassShader.useProgram();
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, windowTexture);
		grassShader.setFloat("texture1", 0.6f);
		view = camera.getViewMat();
		proj = camera.getProjMat();
		grassShader.setMat4("view", view);
		grassShader.setMat4("proj", proj);
		/*for (unsigned int i = 0; i < windows.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, windows[i]);
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1));
			grassShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}*/
		for (std::map<float, glm::vec3>::reverse_iterator iter = sortedWindows.rbegin(); iter != sortedWindows.rend(); iter++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, iter->second);
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1));
			grassShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
#pragma endregion

#pragma region Render backpack

		lightingShader.useProgram();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0,0,-5));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		//model = glm::rotate(model, glm::radians(float(sin(glfwGetTime()) * 180)), glm::vec3(0, 1, 0));
		view = camera.getViewMat();
		proj = camera.getProjMat();
		lightingShader.setMat4("model", model);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("proj", proj);

		glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0XFF);
		glStencilMask(0xFF);
		backpackModel.Draw(lightingShader);

		glDisable(GL_DEPTH_TEST);
		outlineShader.useProgram();
		model = glm::scale(model, glm::vec3(1.02, 1.02, 1.02));
		outlineShader.setMat4("model", model);
		outlineShader.setMat4("view", view);
		outlineShader.setMat4("proj", proj);
		glStencilMask(0x00);
		//glDepthFunc(GL_ALWAYS);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		backpackModel.Draw(outlineShader);
		
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);

#pragma endregion

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int _width, int _height)
{
	glViewport(0, 0, _width, _height);
	width = _width;
	height = _height;
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	float sens = 0.1f;
	static float lastX = xPos, lastY = yPos;
	float xOffset = xPos - lastX, yOffset = lastY - yPos;//y screen coordinates range from top to bottom
	lastX = xPos;
	lastY = yPos;

	xOffset *= sens;
	yOffset *= sens;
	camera.yaw += xOffset;
	camera.pitch += yOffset;

	//prevent flip
	if (camera.pitch > 89.0f)
		camera.pitch = 89.0f;
	if (camera.pitch < -89.0f)
		camera.pitch = -89.0f;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		cameraSpeed *= 2;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)//forward
	{
		//fps camera(should be in camera class)
		//camera.cameraPos += cameraSpeed * glm::vec3(camera.cameraZ.x, 0, camera.cameraZ.z);
		camera.cameraPos += cameraSpeed * glm::vec3(camera.cameraZ);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)//back
	{
		//fps camera(should be in camera class)
		//camera.cameraPos -= cameraSpeed * glm::vec3(camera.cameraZ.x, 0, camera.cameraZ.z);
		camera.cameraPos -= cameraSpeed * glm::vec3(camera.cameraZ);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)//right
	{
		camera.cameraPos -= cameraSpeed * camera.cameraX;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)//left
	{
		camera.cameraPos += cameraSpeed * camera.cameraX;
	}
}