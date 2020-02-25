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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;


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

	Shader TriangleShader("../Boids/src/shaders/Triangle.vs", "../Boids/src/shaders/Triangle.fs");


	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_LINE or GL_FILL

	float vertices[] = {
		// positions          // texture coords
		 0.5f,  0.0f, 0.0f,   1.0f, 1.0f, // right
		-0.5f, -0.3f, 0.0f,   1.0f, 0.0f, // bottom left
		-0.5f,  0.3f, 0.0f,   0.0f, 0.0f, // top left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 2, // first triangle
		//1, 2, 3  // second triangle
	};

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2* sizeof(float)));
	glEnableVertexAttribArray(1);
	// 2. use our shader program when we want to render an object
	TriangleShader.use();

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	glm::vec3 location = glm::vec3(0.0f, 0.0f, 0.0f);

	static const int numOfBoids = 100;

	//Boid boids[numOfBoids];

	//for (int i = 0; i < numOfBoids; i++) {
	//	boids[i] = Boid(genRandomFloat(),genRandomFloat(), genRandomFloat() * 2 * 3.1415, 0.01f);
	//}

	//Boid boid = Boid(genRandomFloat(), genRandomFloat(), 0.01f, 0.01f);

	Flock flock(70);
		
	/* Render Loop */
	while (!glfwWindowShouldClose(window))
	{
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		//printf("random: %f\n", genRandomFloat());
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processInput(window);


		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use Shader
		TriangleShader.use();

		//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		//	boids[0].angle += .04f;
		//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		//	boids[0].angle -= .04f;

		//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		//	boids[0].speed -= .001f;
		//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		//	boids[0].speed += .001f;

		flock.moveFlock();

		for (int i = 0; i < flock.numOfBoids; i++) {

			//printf("Boid rotation %f\n", boids[0].getAngle());

			// transformation matrix
			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::translate(trans, glm::vec3(flock.boids[i].getX(), flock.boids[i].getY(), 0.0f));
			trans = glm::rotate(trans, flock.boids[i].getAngle(), glm::vec3(0.0f, 0.0f, 1.0f));
			trans = glm::scale(trans, glm::vec3(0.1f, 0.1f, 0.1f));

			TriangleShader.setMat4("transform", trans);
			TriangleShader.setVec4("colour", flock.boids[i].colour);


			// Render triangle
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		// check and call events and swap the buffers
		glfwPollEvents();			/* Swap front and back buffers */
		glfwSwapBuffers(window);	/* Poll for and process events */
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


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
