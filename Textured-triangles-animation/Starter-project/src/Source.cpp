#include "Shader.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>

void gladInitializing() {		//initializes glad and checks whether glad is initialized or not
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		std::cin.get();
	}
}

void frame_buffer_resizing(GLFWwindow* window, int width, int height) {		//acts as callback in function responsible for size change
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Shader* shader, float& angle)	//checks for input every frame
{
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		//shader->setFloat("offsety", -0.5f);
		angle++;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		//shader->setFloat("offsety", 0.5f);
		angle--;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		shader->setFloat("offsetx", -0.1f);
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		shader->setFloat("offsetx", 0.1f);
	}
}

int main() {
	if (glfwInit()) {	//Initialising glfw
		std::cout << "initialised";
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //providing a window hint (meta data stored with window to tell glfw(in this case) how window wants to be treated)
	
	auto* window = glfwCreateWindow(640, 480, "Hello OpenGL", nullptr, nullptr);	//window creation and checking code
	if (window) {
		std::cout << "window created successfully";
	}

	glfwMakeContextCurrent(window);
	gladInitializing();


	Shader* shader = new Shader("shaders/VertexShader.txt","shaders/FragmentShader.txt");	//Shader (Vertex, Fragment) creation, compilation, error checking and linking shaders into shaderprogram.

	float vertexData[] = {
		//pos				//color				//texture	
		0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,	0.5f, 0.0f,	//common vertex 
	   -0.5f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,	0.0f, 0.0f,	//left tri, left vert
	   -0.25f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,	0.25f, 1.0f,
		0.5f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.25f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,	0.75f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2, 
		0, 3, 4
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int tex1;
	glGenTextures(1, &tex1);

	glBindTexture(GL_TEXTURE_2D, tex1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* imageData = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imageData);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glViewport(0, 0, 640, 480);

	glfwSetFramebufferSizeCallback(window, frame_buffer_resizing);
	float rotateAngle = 0.0f ;

	while (!glfwWindowShouldClose(window)) {
		//input area
		processInput(window, shader, rotateAngle);

		//rendering area
		glClearColor(1.0f, 1.0f, 0.3f, 0.5f );
		glClear(GL_COLOR_BUFFER_BIT);	//Flushes the color buffer with yellowish color 
		
		//glUseProgram(shaderProgram);
		shader->use();
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::rotate(transform, rotateAngle, glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::scale(transform, glm::vec3(abs(sin(glfwGetTime())), abs(sin(glfwGetTime())), abs(sin(glfwGetTime()))));
			//glm::translate(transform, glm::vec3(0.5f, -1.0f, 0));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	//glDeleteProgram(shaderProgram);

	glfwTerminate();
	std::cin.get();
}
