#include <glad/glad.h>//loads functions to function pointers
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main();

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
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

int main()
{
	int width, height, channelNum;
	unsigned char* texData = stbi_load("wall.jpg", &width, &height, &channelNum, 0);

	//initialize glfw,configure glfw options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a window
	GLFWwindow* window = glfwCreateWindow(500, 500, "Epic stuff", NULL, NULL);
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
	
	glViewport(0, 0, 500, 500);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);//callback function on window resize

	////rectangle
	//float vertices[] =
	//{
	//	-0.5f, -0.5, 0.0f,// bottom left
	//	-0.5f, 0.5, 0.0f,// top left
	//	0.5f, -0.5, 0.0f,// bottom right
	//	0.5f, 0.5, 0.0f// top right
	//};
	//unsigned int indices[] =
	//{
	//	0,2,3,
	//	1,3,0
	//};
	// 
	//unsigned int VAO;
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);//encapsulates VBOs, vertex attribs configurations
	//unsigned int VBO;
	//glGenBuffers(1, &VBO);//Generate 1 buffer with an ID stored in VBO
	////only one buffer can be bound to a buffer type, 
	////any calls to GL_ARRAY_BUFFER will be used on the currently bound buffer
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//triangle 1
	float vertices1[] =
	{
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	unsigned int VAO1;
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);
	unsigned int VBO1;
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	Shader shader("./v.vert", "./f.frag");
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//input
		processInput(window);

		//rendering commands
		shader.useProgram();

		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);//uses the currently active shader
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
}