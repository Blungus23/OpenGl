#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.h>
#include <TextureBuffer.h>
#include <Texture2D.h>
#include <glm/glm.hpp>	
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Random.h>


typedef struct Color {
	float r;
	float g;
	float b;
}Color;

Random r;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
Color genRandomColor();

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;



unsigned int framebuffer;

glm::vec2 mouse;
bool mouseDown = false;
bool swap = false;
float brushSize = 0.01f;
int pixSize = 1;
int frameCount = 0;


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window context
	GLFWwindow*  window = glfwCreateWindow(WIDTH, HEIGHT, "Water Ripple Effect", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;

	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	//Make sure GLAD is initialized
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initailize GLAD" << std::endl;
		return -1;
	}

	//Create & bind smiley texture
	Texture2D smileyTexture("Resources/awesomeface.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_RGBA);
	glActiveTexture(GL_TEXTURE2);
	smileyTexture.bind();

	//Create framebuffer and add two textures to it
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	TextureBuffer sourceTexture(WIDTH, HEIGHT, GL_COLOR_ATTACHMENT0);		// past generation
	TextureBuffer destinationTexture(WIDTH, HEIGHT, GL_COLOR_ATTACHMENT1);	// next generation
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Build  and compile shader code
	Shader canvasShader("./shaders/reaction_vertex.vert", "./shaders/rain_fragment.frag");
	Shader screenShader("./shaders/screen_vertex.vert", "./shaders/screen_fragment.frag");

	float m = 2.0f;
	glm::mat4 scaler = glm::mat4(1.0f);
	scaler = glm::scale(scaler, glm::vec3(m, m, m));


	// canvas vertecies
	float canvasVertices[] = {
		0.5f,  0.5f, 0.0f,  1.0f,1.0f,	// top right
		0.5f, -0.5f, 0.0f,  1.0f,0.0f,	// bottom right
	   -0.5f, -0.5f, 0.0f,	0.0f,0.0f,	// bottom left
	   -0.5f,  0.5f, 0.0f,	0.0f,1.0f	// top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};


	// Screen Verts
	float quadVertices[] = {
		// positions   // texCoords
	   -1.0f,  1.0f,  0.0f, 1.0f,
	   -1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

	   -1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};



	//Canvas 
	unsigned int canvasVAO, canvasVBO, canvasEBO;
	glGenVertexArrays(1, &canvasVAO);
	glGenBuffers(1, &canvasVBO);
	glGenBuffers(1, &canvasEBO);
	glBindVertexArray(canvasVAO);

	glBindBuffer(GL_ARRAY_BUFFER, canvasVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(canvasVertices), canvasVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, canvasEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Texture Image
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	// Screen or (quad)
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	// Positions
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Texture Coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//render loop
	unsigned int colorAttachment = GL_COLOR_ATTACHMENT1;

	canvasShader.use();
	canvasShader.setInt("_MainTexture", 0);
	canvasShader.setInt("pastGeneration", 1);

	while (!glfwWindowShouldClose(window)) {
		/* INPUT STEP */
		processInput(window);


		/* RENDER STEP */
		// first pass
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glDrawBuffer(colorAttachment);
		glEnable(GL_DEPTH_TEST);
		
		glActiveTexture(GL_TEXTURE0 + 0);
		sourceTexture.bind();
		glActiveTexture(GL_TEXTURE0 + 1);
		destinationTexture.bind();

		canvasShader.use();
		canvasShader.setMat4("scaleMatrix", scaler);
		canvasShader.setVec2("mousePos", mouse);
		canvasShader.setBool("swap", swap);
		canvasShader.setInt("pixleSize", pixSize);
		canvasShader.setFloat("brushSize", brushSize);
		canvasShader.setBool("mouseDown", mouseDown);
		canvasShader.setInt("screenWidth", WIDTH);
		canvasShader.setInt("screenHeight", HEIGHT);
		


		// Render whats on that FBO texture 
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		screenShader.use();
		screenShader.setInt("size", pixSize);
		screenShader.setBool("swap", swap);
		destinationTexture.bind(); // draw next generation
		glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Swap textures
		TextureBuffer t = sourceTexture;
		sourceTexture = destinationTexture;
		destinationTexture = t;


		// clear buffers and swap color attachments 
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		if (colorAttachment == GL_COLOR_ATTACHMENT1) {
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			colorAttachment = GL_COLOR_ATTACHMENT0;
		}
		else {
			glDrawBuffer(GL_COLOR_ATTACHMENT1);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			colorAttachment = GL_COLOR_ATTACHMENT1;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (!mouseDown) {
			mouse = glm::vec2(-10, -10);
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
		frameCount++;
		if (frameCount > 60) {
			frameCount = 0;
		}
		swap = (frameCount % 2 == 0);
	}
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteVertexArrays(1, &canvasVAO);
	glDeleteBuffers(1, &canvasVBO);
	glDeleteFramebuffers(1, &framebuffer);
	screenShader.discard();
	canvasShader.discard();
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//window resize callback
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	float xoff = 0.5f;
	float yoff = 0.5f;
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		mouse = glm::vec2((0.5 - ((xpos) / WIDTH))*-1, 0.5 - ((ypos) / HEIGHT));
		mouseDown = true;
	}
	else {
		mouseDown = false;
	}
}

Color genRandomColor() {
	Color c;
	c.r = r.RandomRangef();
	c.g = r.RandomRangef();
	c.b = r.RandomRangef();
	return c;
}

