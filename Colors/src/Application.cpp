#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.h>
#include <Camera.h>
#include <glm/glm.hpp>	
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




Material genRandomMaterial();

float RandomRangef(float min = 0, float max = 1);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

unsigned int WIDTH = 800;
unsigned int HEIGHT = 600;
Camera camera(glm::vec3(0, 0, 3.0), glm::vec3(0.0, 1.0, 0.0), -90.0, 0.0);

Light white;
Material plastic;
Material m_white;

bool firstMouse = true;
float lastX = 0;
float lastY = 0;
float deltaTime = 0;
float lastFrame = 0;
float input_A = 0.2f;
bool disabled = true;
float lastKeyPress = 0.0f;
float waitTime = 3.0f;

int main() {
	//Set up white light
	white.position = glm::vec3(1.0f, 1.0f, -1.0f);
	white.ambient = glm::vec3(1.0f);
	white.diffuse = glm::vec3(1.0f);
	white.specular = glm::vec3(1.0f);
	white.color = glm::vec3(1.0f);

	//Set up cyan plastic material
	plastic.ambient = glm::vec3(0.0f,0.1f,0.06f);
	plastic.diffuse = glm::vec3(0.0f,0.50980392f,0.50980392f);
	plastic.specular = glm::vec3(0.50196078f,0.50196078f,0.50196078f);
	plastic.shininess = 0.25f;

	//Set up white shineyt material
	m_white.ambient = glm::vec3(0.97f, 0.95f, 0.94f);
	m_white.diffuse = glm::vec3(1.0f);
	m_white.specular = glm::vec3(0.39f, 0.39f, 0.4f);
	m_white.shininess = 34.0f;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	// create window context
	GLFWwindow*  window = glfwCreateWindow(WIDTH, HEIGHT, "Colors", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;

	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	glfwSetCursorPosCallback(window,mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Make sure GLAD is initialized
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initailize GLAD" << std::endl;
		return -1;
	}
	//Enable Depth Testing
	glEnable(GL_DEPTH_TEST);

	//Build  and compile shader code
	Shader shaderProgram("./shaders/vertex.vert", "./shaders/fragment.frag");
	Shader lightShader("./shaders/vertex.vert", "./shaders/light.frag");

	//Cube data with normals
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	//Cube Models
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  -1.0f,  0.0f),
		glm::vec3(2.0f,  -1.0f,  0.0f),
		glm::vec3(0.0f,  -1.0f,  -2.0f),
		glm::vec3(2.0f,  -1.0f,  -2.0f),
	};
	int total_cubes = sizeof(cubePositions) / sizeof(glm::vec3);
	Material mats[] = {
		m_white,
	};
	int total_mats = sizeof(mats) / sizeof(Material);
	std::cout << total_mats << std::endl;
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Lights
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//projection matrix
	glm::mat4 proj;
	proj = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	
	//Set the projection matrix and light source
	shaderProgram.use();
	shaderProgram.setMat4("projection", proj);

	//Set Light Uniform
	shaderProgram.setLight("light",white);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//render loop
	float waitTime = 5.0f;
	while (!glfwWindowShouldClose(window)) {
		
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		if (lastKeyPress > 0)
			lastKeyPress -= 0.15f;
		

		processInput(window);
		
		//all rendering happens here
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//create transformations and camera
		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 proj;
		proj = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);


		//Setup Lighting color
		white.color.r = sin(currentFrame * 2.0f);
		white.color.g = sin(currentFrame * 0.7f);
		white.color.b = sin(currentFrame * 1.3f);
		//white.position.x = cos(currentFrame) * total_cubes;
		//white.position.y = sin(currentFrame);
		//white.position.z = sin(currentFrame) * total_cubes;
		
		//Calculate diffusion and ambiance
		white.diffuse = white.color * glm::vec3(0.5f);
		white.ambient = white.diffuse * glm::vec3(0.2f);
		shaderProgram.setVec3("light.ambient", white.ambient);
		shaderProgram.setVec3("light.diffuse", white.diffuse);

		//Draw Lights
		lightShader.use();
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", proj);
		lightShader.setVec3("lightColor", white.color);
		glBindVertexArray(lightVAO);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, white.position);
		model = glm::scale(model, glm::vec3(0.5f));
		lightShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//Use shader Program to render non light emissive objects
		shaderProgram.use();
		shaderProgram.setVec3("viewPos", camera.Position);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", proj);
		shaderProgram.setVec3("light.position", white.position);
		//Draw Cubes
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < total_cubes; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 0.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shaderProgram.setMat4("model", model);
			shaderProgram.setMaterial("material", mats[i % total_mats]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	shaderProgram.discard();
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//window resize callback
	WIDTH =  width;
	HEIGHT = height;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		if (lastKeyPress <= 0.0f) {
			disabled = !disabled;
			if (disabled) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			lastKeyPress = waitTime;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		input_A += 0.01f;
		if (input_A >= 1.0f) {
			input_A = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		input_A -= 0.01f;
		if (input_A <= 0.0f) {
			input_A = 0.0f;
		}
	}
	float cameraSpeed = static_cast<float>(2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

float RandomRangef(float min, float max) {
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

// Create a random material 
Material genRandomMaterial(){
	Material mat;
	mat.ambient = glm::vec3(RandomRangef(), RandomRangef(), RandomRangef());
	mat.diffuse = glm::vec3(RandomRangef(), RandomRangef(), RandomRangef());
	mat.specular = glm::vec3(RandomRangef(), RandomRangef(), RandomRangef());
	mat.shininess = RandomRangef();
	return mat;
}