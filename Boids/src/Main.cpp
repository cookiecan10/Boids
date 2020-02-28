#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include "shader_s.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Boid.h"
#include <vector>
#include "stdlib.h"
#include <time.h>
#include "Flock.h"
#include <string>

#define ASSERT(x) if (!(x)) __debugbreak(); //Sets a breakpoint at that line of code
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//Clears all of the errors left in OpenGL
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

//Prints an error message to the console
bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_cursorPos_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

double xPos, yPos;

const static int INITIAL_FLOCK_SIZE = 1;

Flock flock(INITIAL_FLOCK_SIZE);
glm::vec4 *colours = new glm::vec4[INITIAL_FLOCK_SIZE];

float genRandomFloat();

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//OpenGL version definition
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window;
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Boids", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//Check if GLAD loaded
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Tell OpenGL size of window
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//Tell OpenGL which function to set a callback for when chaning window sizes
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Tell OpenGL which function to set a callback for when clicking with mouse
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	Shader TriangleShader("./src/shaders/Triangle.vs", "./src/shaders/Triangle.fs");
	Shader TriangleShaderInstanced("./src/shaders/TriangleInstanced.vs", "./src/shaders/TriangleInstanced.fs");
	
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_LINE or GL_FILL

	// For vertex buffer
	float vertices[] = {
		// positions          // texture coords
		 0.5f,  0.0f, 0.0f,   1.0f, 1.0f, // right
		-0.5f, -0.3f, 0.0f,   1.0f, 0.0f, // bottom left
		-0.5f,  0.3f, 0.0f,   0.0f, 0.0f, // top left
	};

	// For index buffer
	unsigned int indices[] = {
		0, 1, 2, // first triangle
		//1, 2, 3  // second triangle
	};

	// For instance matrix buffer
	glm::mat4 *translations = new glm::mat4[INITIAL_FLOCK_SIZE];

	// For instance colour buffer


	// Vertex Array Object
	unsigned int VAO;		// Buffer that stores whatever is bound
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex Buffer Object
	unsigned int VBO;		// Buffer with vertex data
	glGenBuffers(1, &VBO);
	// copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Element Buffer Object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//copy our indices array in a buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 1. then set the vertex attributes pointers
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2* sizeof(float)));

	unsigned int instanceMatricesVBO;
	GLCall(glGenBuffers(1, &instanceMatricesVBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, instanceMatricesVBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, INITIAL_FLOCK_SIZE * sizeof(glm::mat4), &translations[0], GL_DYNAMIC_DRAW));

	GLsizei vec4Size = sizeof(glm::vec4);
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0));
	GLCall(glEnableVertexAttribArray(3));
	GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size)));
	GLCall(glEnableVertexAttribArray(4));
	GLCall(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size)));
	GLCall(glEnableVertexAttribArray(5));
	GLCall(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size)));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	unsigned int instanceColourVBO;
	GLCall(glGenBuffers(1, &instanceColourVBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, instanceColourVBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, INITIAL_FLOCK_SIZE * sizeof(glm::mat4), &colours[0], GL_DYNAMIC_DRAW));

	GLCall(glEnableVertexAttribArray(6));
	GLCall(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0));

	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	// 2. use our shader program when we want to render an object
	TriangleShader.use();

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	/* 1 Time Logic */

	colours = flock.getColours();

	glBindVertexArray(VAO);

	/* Render Loop */
	while (!glfwWindowShouldClose(window))
	{
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			printf("%i boids\n", flock.getFlockSize());

			nbFrames = 0;
			lastTime += 1.0;
		}

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// Input
		processInput(window);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			flock.addBoid(0.0f, 0.0f, 0.01f, glm::vec4(genRandomFloat(), genRandomFloat(), genRandomFloat(), 1.0f));
			colours = flock.getColours();
			printf("Flocksize %i\n", flock.getFlockSize());
		}


		// Logic
		flock.moveFlock(); // Move all of the flock objects

		translations = flock.getTranslations(); // Calculate translation matrices for every boid object


		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		TriangleShaderInstanced.use();
		
		//TriangleShaderInstanced.setVec4("colour", glm::vec4(1.0f, 0.5f, 0.1f, 1.0f));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, instanceMatricesVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, flock.getFlockSize() * sizeof(glm::mat4), &translations[0], GL_DYNAMIC_DRAW));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, instanceColourVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, flock.getFlockSize() * sizeof(glm::vec4), &colours[0], GL_DYNAMIC_DRAW));


		// Render triangle
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3, flock.getFlockSize());
		////glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0, flockSize);


		// check and call events and swap the buffers
		glfwPollEvents();			/* Swap front and back buffers */
		glfwSwapBuffers(window);	/* Poll for and process events */
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// My de-allocations
	delete(translations);
	delete(colours);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// Process keyboard input
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

// Define callback for changing window sizes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Define callback for mouse button
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//Tell OpenGL to automaticly set the cursor position in these variables
		glfwGetCursorPos(window, &xPos, &yPos);
		//printf("Left mouse button pressed at %f %f\n", xPos/SCR_WIDTH, yPos/SCR_HEIGHT);
		flock.addBoid((xPos / SCR_WIDTH)*2 -1, ((yPos / SCR_HEIGHT)*2 -1) * -1, 0.01f, glm::vec4(genRandomFloat(), genRandomFloat(), genRandomFloat(), 1.0f));
		colours = flock.getColours();
	}
}
