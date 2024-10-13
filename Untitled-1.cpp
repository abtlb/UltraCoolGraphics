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

int width = 2560, height = 1400;

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
	float cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, // top-left

		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom-left

		// Left face
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top-right

		// Right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom-left

		 // Bottom face
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f, // top-right
		  0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f, // top-left
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom-left
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom-left
		 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, // bottom-right
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f, // top-right

		 // Top face
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // top-left
		  0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom-right
		  0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, // top-right
		  0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom-right
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // top-left
		 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f  // bottom-left
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


#pragma region Skybox setup
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};


	unsigned int skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);

	unsigned int skyboxVBO;
	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	const char* cubemapPaths[] =
	{
		"./skybox1/right.jpg",
		"./skybox1/left.jpg",
		"./skybox1/top.jpg",
		"./skybox1/bottom.jpg",
		"./skybox1/front.jpg",
		"./skybox1/back.jpg"
	};
	unsigned int cubemap1 = Texture::CubemapFromFiles(cubemapPaths);
#pragma endregion


#pragma region Quad setup
	float quadVertices[] = {
		// positions // texCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	unsigned int quadVAO;
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	unsigned int quadVBO;
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);//coordinates
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));//tex coords
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
	Shader lightSourceShader("./Shaders/lightSource.vert", "./Shaders/lightSource.frag");
	Shader lightingShader("./Shaders/lighting.vert", "./Shaders/lighting.frag");
	Shader outlineShader("./Shaders/lighting.vert", "./Shaders/outline.frag");
	Shader grassShader("./Shaders/grass.vert", "./Shaders/grass.frag");
	Shader screenShader("./Shaders/SingleTexture/screen.vert", "./Shaders/SingleTexture/screen.frag");
	Shader screenRearShader("./Shaders/SingleTexture/screenrear.vert", "./Shaders/SingleTexture/screen.frag");
	Shader greyScaleShader("./Shaders/SingleTexture/screen.vert", "./Shaders/SingleTexture/greyscale.frag");
	Shader sharpenShader("./Shaders/SingleTexture/screen.vert", "./Shaders/SingleTexture/sharpen.frag");
	Shader skyboxShader("./Shaders/cubemap.vert", "./Shaders/cubemap.frag");
	Shader reflectionShader("./Shaders/envreflection.vert", "./Shaders/envreflection.frag");
	Shader refractionShader("./Shaders/envreflection.vert", "./Shaders/envrefraction.frag");
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

#pragma region Front Framebuffer setup
	unsigned int fbo1;
	glGenFramebuffers(1, &fbo1);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo1);

	auto colorAttachment = Texture::ColorAttachment(width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" <<
		std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//render to the default framebuffer
#pragma endregion



	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);//i make it enabled by default
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		//input
		processInput(window);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo1);
		glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//rendering commands
		glStencilMask(0x00);//for outline

		glm::mat4 model, view, proj;


#pragma region Render Rotating Cubes
		//10 rotating cubes

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap1);

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
			glBindFramebuffer(GL_FRAMEBUFFER, fbo1);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightingShader.useProgram();
		lightingShader.setVec3("cameraPos", camera.cameraPos);

#pragma region Lighting stuff
		lightingShader.setVec3("dirLight.dir", glm::vec3(-0.2f, 1.0f, -1.0f));
		lightingShader.setVec3("dirLight.ambient", glm::vec3(0.2f));
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
		/*
		glDisable(GL_CULL_FACE);
		std::vector<glm::vec3> vegetation;
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
		}
		glEnable(GL_CULL_FACE);
		*/

#pragma endregion

#pragma region Windows
		
		glDisable(GL_CULL_FACE);

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
		//---PROBLEM HERE---
		/*for (std::map<float, glm::vec3>::reverse_iterator iter = sortedWindows.rbegin(); iter != sortedWindows.rend(); iter++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, iter->second);
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1));
			grassShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}*/

		glEnable(GL_CULL_FACE);
#pragma endregion

#pragma region Render backpack

		glDisable(GL_BLEND);
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
		glEnable(GL_BLEND);
		//glDepthFunc(GL_LESS);

#pragma endregion

#pragma region Render refracted backpack

		refractionShader.useProgram();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3, 3, -3));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		//model = glm::rotate(model, glm::radians(float(sin(glfwGetTime()) * 180)), glm::vec3(0, 1, 0));
		view = camera.getViewMat();
		proj = camera.getProjMat();
		refractionShader.setMat4("model", model);
		refractionShader.setMat4("view", view);
		refractionShader.setMat4("proj", proj);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap1);
		refractionShader.setFloat("cubemap", 0);
		refractionShader.setVec3("cameraPos", camera.cameraPos);

		backpackModel.Draw(refractionShader);

#pragma endregion


#pragma region Render reflection cube
		reflectionShader.useProgram();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5, 5 ,-5));
		reflectionShader.setMat4("model", model);

		view = camera.getViewMat();
		proj = camera.getProjMat();

		reflectionShader.setMat4("view", view);
		reflectionShader.setMat4("proj", proj);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap1);
		reflectionShader.setFloat("cubemap", 0);
		reflectionShader.setVec3("cameraPos", camera.cameraPos);
		
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
#pragma endregion

#pragma region RenderCubemap
		//glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		skyboxShader.useProgram();
		view = glm::mat4(glm::mat3(camera.getViewMat()));//removes translation but keeps rotation
		proj = camera.getProjMat();

		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", proj);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap1);
		skyboxShader.setFloat("cubemap", 0);
		glBindVertexArray(skyboxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
		//glDepthMask(GL_TRUE);

#pragma endregion

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.useProgram();
		screenShader.setFloat("texture1", 0);
		glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorAttachment);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_DEPTH_TEST);

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

	camera.updateCameraAxes();
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