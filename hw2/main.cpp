#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int createShader(const char* filename, const char* type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
void LoadTexture(unsigned int& texture, const char* tFileName);
glm::mat4 getPerspective();
glm::mat4 getView();

Object* catModel = new Object("obj/cat.obj");
Object* boxModel = new Object("obj/CardboardBox1.obj");

int windowWidth = 800, windowHeight = 600;

float flat = 1.0f;
float dark = 1.0f;
bool on = false;
bool f = false;
bool trend = false;
bool c = false;

void Draw(glm::mat4 mat, Object* model, unsigned int VAO, unsigned int shaderProgram){
	unsigned int mLoc, vLoc, pLoc;
	mLoc = glGetUniformLocation(shaderProgram, "M");
	vLoc = glGetUniformLocation(shaderProgram, "V");
	pLoc = glGetUniformLocation(shaderProgram, "P");
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mat));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(getView()));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(getPerspective()));
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, model->positions.size());
	glBindVertexArray(0);
}

unsigned int setVAO(Object* model) {
	unsigned int VAO, VBO[3];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * model->positions.size(), &(model->positions[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * model->normals.size(), &(model->normals[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->texcoords.size()), &(model->texcoords[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	return VAO;
}

int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFE_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW2", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Shaders
	unsigned int vertexShader, fragmentShader, shaderProgram;
	vertexShader = createShader("vertexShader.vert", "vert");
	fragmentShader = createShader("fragmentShader.frag", "frag");
	shaderProgram = createProgram(vertexShader, fragmentShader);
	glUseProgram(shaderProgram);

	// Texture
	unsigned int catTexture, boxTexture;
	LoadTexture(catTexture, "obj/Cat_diffuse.jpg");
	LoadTexture(boxTexture, "obj/CardboardBox1_Albedo.tga");

	// TO DO:
	// Create VAO, VBO
	unsigned int cat_VAO, box_VAO;
	cat_VAO = setVAO(catModel);
	box_VAO = setVAO(boxModel);

	// Display loop
	// TO DO:
	// Draw the models with shaders
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//這三行好像沒差
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);

	glm::mat4 box_mat = glm::mat4(1.0f);
	glm::mat4 cat_mat = glm::mat4(1.0f);
	glUniform1i(glGetUniformLocation(shaderProgram, "catTexture"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "boxTexture"), 1);
	int k = 0;
	double delta = 0.0;
	double time = glfwGetTime();
	float k1[6] = { 0.8, 0.4, 0.0, 0.0, 0.0, 0.4 };
	float k2[6] = { 0.0, 0.4, 0.7, 0.4, 0.0, 0.0 };
	float k3[6] = { 0.0, 0.0, 0.0, 0.4, 0.7, 0.4 };
	int kindex = 0;
	double period = 0.0;
	while (!glfwWindowShouldClose(window))
	{
		// 這兩行重要!!
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//
		box_mat = glm::mat4(1.0f);
		cat_mat = glm::mat4(1.0f);
		box_mat = glm::scale(box_mat, glm::vec3(0.0625, 0.05, 0.05));
		cat_mat = glm::rotate(cat_mat, glm::radians(90.0f), glm::vec3(0, 1, 0));
		if (f) {
			printf("%f\n", flat);
			if (trend) {
				if (flat >= 2.0f) trend = false;
				else flat = flat + 0.02f;
			}
			else{
				if (flat <= 0.5f) trend = true;
				else flat = flat - 0.02f;
			}
		}
		glUniform1f(glGetUniformLocation(shaderProgram, "f1"), flat);
		double tmp = glfwGetTime();
		delta = tmp - time;
		period = period + delta;
		k = k + delta * 90.0;
		time = tmp;
		cat_mat = glm::rotate(cat_mat, glm::radians(1.0f * k), glm::vec3(0, 1, 0));
		box_mat = glm::rotate(box_mat, glm::radians(1.0f * k), glm::vec3(0, 1, 0));
		k = k % 360;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, catTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, boxTexture);

		glUniform1f(glGetUniformLocation(shaderProgram, "D"), dark);
		glUniform1i(glGetUniformLocation(shaderProgram, "on"), on);

		glUniform1i(glGetUniformLocation(shaderProgram, "c"), c);

		glUniform1f(glGetUniformLocation(shaderProgram, "k1"), k1[kindex]);
		glUniform1f(glGetUniformLocation(shaderProgram, "k2"), k2[kindex]);
		glUniform1f(glGetUniformLocation(shaderProgram, "k3"), k3[kindex]);
		if (period > 1) {
			kindex = (kindex + 1) % 6;
			period -= 1;
		}
		

		glUniform1i(glGetUniformLocation(shaderProgram, "up"), true);
		glUniform1f(glGetUniformLocation(shaderProgram, "choose"), 0.0f);
		Draw(cat_mat, catModel, cat_VAO, shaderProgram);

		glUniform1i(glGetUniformLocation(shaderProgram, "up"), false);
		glUniform1f(glGetUniformLocation(shaderProgram, "choose"), 1.0f);
		Draw(box_mat, boxModel, box_VAO, shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (key == GLFW_KEY_1) {
		f = true;
	}
	else if (key == GLFW_KEY_2) {
		f = false;
	}
	else if (key == GLFW_KEY_3) {
		dark = dark + 0.2f;
		if (dark > 1.0f) dark = 1.0f;
	}
	else if (key == GLFW_KEY_4) {
		dark = dark - 0.2f;
		if (dark < 0.0f) dark = 0.0f;
	}
	else if (key == GLFW_KEY_5) on = true;
	else if (key == GLFW_KEY_6) on = false;
	else if (key == GLFW_KEY_7) c = true;
	else if (key == GLFW_KEY_8) c = false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

unsigned int createShader(const char* filename, const char* type)
{
	unsigned int shader;
	if (strcmp(type, "vert") == 0)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (strcmp(type, "frag") == 0)
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else
	{
		cout << "Unknown Shader Type.\n";
		return 0;
	}

	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);  //same as rewind(fp);

	char* source = (char*)malloc(sizeof(char) * (fsize + 1));
	fread(source, fsize, 1, fp);
	fclose(fp);

	source[fsize] = 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::" << type << "::COMPLIATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	//Link our program
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		char* infoLog = (char*)malloc(sizeof(char) * (maxLength));
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		puts(infoLog);
		free(infoLog);

		return -1;
	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

glm::mat4 getPerspective()
{
	return glm::perspective(
		glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 
		0.1f, 100.0f);
}

glm::mat4 getView()
{
	return glm::lookAt(
		glm::vec3(0, 5, 5),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
}