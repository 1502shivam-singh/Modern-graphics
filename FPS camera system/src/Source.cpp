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

bool firstMouse = true;
float lastX = 320.0f, lastY = 240.0f, yaw = -90.0f, pitch = 0.0f;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

void frame_mouse_detect(GLFWwindow* window, double xpos, double ypos) {		//acts as callback in function responsible for size change
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) 
		pitch = 89.0f;

	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

float alongX = 0.0f, alongY = 0.0f, alongZ = 7.0f ;

void processInput(GLFWwindow *window)	//checks for input every frame
{
	const float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		alongY-=0.4f*cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		alongY += 0.4f*cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		alongX -= 0.4f*cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		alongX += 0.4f*cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		alongZ -= 0.4f*cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		alongZ += 0.4f*cameraSpeed;
	}
}

float fov = 45.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= yoffset;
	if (fov >= 45.0f) {
		fov = 45.0f;
	}
	if (fov <= 1.0f) {
		fov = 1.0f;
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
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, frame_buffer_resizing);
	glfwSetCursorPosCallback(window, frame_mouse_detect);
	glfwSetScrollCallback(window, scroll_callback);

	gladInitializing();


	Shader* shader = new Shader("shaders/VertexShader.txt","shaders/FragmentShader.txt");	//Shader (Vertex, Fragment) creation, compilation, error checking and linking shaders into shaderprogram.

	float vertexData[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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



	float rotateAngle = 0.0f ;
	float radius = 10.0f;
	float camCoordX=0.0f, camCoordZ=0.0f;

	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

	while (!glfwWindowShouldClose(window)) {
		//input area
		processInput(window);

		//rendering area
		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 0.3f, 0.5f );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Flushes the color buffer with yellowish color 
		
		//glUseProgram(shaderProgram);
		shader->use();
		glm::mat4 transform = glm::mat4(1.0f);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), 640.0f / 480.0f, 0.1f, 100.0f);
		glm::vec3 cameraPos = glm::vec3(alongX, alongY, alongZ);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos+cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));

		//glm::mat4 view = glm::lookAt(cameraPos,  cameraPos+cameraFront, cameraUp);
			//glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		//transform = glm::rotate(transform, rotateAngle, glm::vec3(1.0f, 0.0f, 0.0f));
		//transform = glm::scale(transform, glm::vec3(abs(sin(glfwGetTime())), abs(sin(glfwGetTime())), abs(sin(glfwGetTime()))));
		//glm::translate(transform, glm::vec3(0.5f, -1.0f, 0));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawArray(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	std::cin.get();
}