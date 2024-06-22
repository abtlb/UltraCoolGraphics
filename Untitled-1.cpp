#include <iostream>
#include <glad/glad.h>//loads functions to function pointers
#include <GLFW/glfw3.h>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

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

int width = 500, height = 500;
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
//glm::vec3 cameraZ = glm::vec3(0.0f, 0.0f, -1.0f);//negative z axis(camera front)
//glm::vec3 cameraY = glm::vec3(0.0f, 1.0f, 0.0f);
//glm::vec3 cameraX = glm::cross(cameraY, cameraZ);
//float yaw = -90.0f, pitch = 0.0f;

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

	//cube
	float cubeVertices[] =
	{
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};
	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	unsigned int cubeVBO;
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width1, height1, channelsNum1;
	unsigned char* texData1 = stbi_load("wall.jpg", &width1, &height1, &channelsNum1, 0);


	if (texData1)
	{
		//internalFormat argument: how opengl is going to store the texture data
		//format argument: format of the pixel data provided
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, texData1);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(texData1);
	}
	else
	{
		std::cerr << "Failed to load the texture";
	}

	//operations done on buffer are stored in the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(true);
	int width2, height2, channelsNum2;
	unsigned char* texData2 = stbi_load("awesomeface.png", &width2, &height2, &channelsNum2, 0);
	if (texData2)
	{	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData2);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(texData2);
	}
	else
	{
		std::cerr << "Failed to load the texture";
	}

	Shader shader("./v.vert", "./f.frag");
	//shader.useProgram();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	Shader lightingShader("./lighting.vert", "./lighting.frag");
	lightingShader.useProgram();
   	lightingShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
   	lightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	Shader lightSourceShader("./lightSource.vert", "./lightSource.frag");
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

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

	//setup mouse input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//capture the cursor and make it non-visible
	glfwSetCursorPosCallback(window, mouse_callback);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//input
		processInput(window);

		//rendering commands

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
		};

		glEnable(GL_DEPTH_TEST);

		//10 rotating cubes
		//int cubeNum = sizeof(cubePositions) / sizeof(glm::vec3);
		//for (int i = 0; i < 10; i++)
		//{
		//	glm::mat4 model = glm::mat4(1.0f);
		//	model = glm::translate(model, cubePositions[i]);
		//	model = glm::rotate(model, cubeRotSpeeds[i] * (float)glfwGetTime() * glm::radians(45.0f), cubeRotAxis[i]);
		//	shader.setMat4("model", model);

		//	glm::mat4 view = camera.getViewMat();
		//	glm::mat4 proj = camera.getProjMat();

		//	shader.setMat4("view", view);
		//	shader.setMat4("proj", proj);
		//	//camera end
		//	glBindVertexArray(cubeVAO);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		glm::mat4 model, view, proj;

		/*//light source
		lightSourceShader.useProgram();
		model = glm::mat4(1.0f);
		model = glm::mat4(1.0f);
	   	model = glm::translate(model, lightPos);
   		model = glm::scale(model, glm::vec3(0.2f));
		view = camera.getViewMat();
		proj = camera.getProjMat();
		lightSourceShader.setMat4("model", model);
		lightSourceShader.setMat4("view", view);
		lightSourceShader.setMat4("proj", proj);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/

		//our cube
		lightingShader.useProgram();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 3.0f, 0));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view = camera.getViewMat();
		proj = camera.getProjMat();
		lightingShader.setMat4("model", model);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("proj", proj);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	float sens = 0.1f;
	static float lastX = width / 2.0f, lastY = height / 2.0f;
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
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)//forward
	{
		//fps camera(should be in camera class)
		camera.cameraPos += cameraSpeed * glm::vec3(camera.cameraZ.x, 0, camera.cameraZ.z);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)//back
	{
		//fps camera(should be in camera class)
		camera.cameraPos -= cameraSpeed * glm::vec3(camera.cameraZ.x, 0, camera.cameraZ.z);
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