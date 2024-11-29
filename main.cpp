#include <iostream>
#include <fmt/core.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

// Window global settings	
GLFWwindow* window;
auto closeWindow = false;
const int WIN_WIDTH = 1920;
const int WIN_HEIGHT = 1080;

// Global variables
unsigned int program;
unsigned int VBO;
int colorLocation;
// Vertices for arrow

float vertices[] =
{
	// Tail
	/*-0.5f, -0.5f, 0.0f, // Endpoint 1
	0.5f, -0.5f, 0.0f, // Endpoint 2

	// Head
	// Top
	0.5f, -0.5f, 0.0f,   
	0.2f, -0.1f, 0.0f,

	// Bottom
	0.5f, -0.5f, 0.0f,
	0.2f, -0.8f, 0.0f */

	-0.5f, 0.5f, 0.0f,
	0.0f, 0.0f, 0.0f,

	0.3f, 0.3f, 0.0f,
	0.45f, 0.6f, 0.0f,

	0.55f, 0.3f, 0.0f,
	0.6f, -0.1f, 0.0f,

	// LINES
	// 1
	-0.5f, 0.5f, 0.0f,
	0.0f, 0.0f, 0.0f, 

	// 2
	0.0f, 0.0f, 0.0f,
	0.3f, 0.3f, 0.0f,

	// 3
	0.3f, 0.3f, 0.0f,
	0.45f, 0.6f, 0.0f,

	// 4
	0.45f, 0.6f, 0.0f,
	0.55f, 0.3f, 0.0f,

	// 5
	0.55f, 0.3f, 0.0f,
	0.6f, -0.1f, 0.0f


};

// Shaders
// Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0);"
"}\0";

// Fragment
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 colorTest;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(colorTest, 1.0f);\n"
"}\0";

// Error callback
static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

// Build and compile Shader program
static void shaderProgram()
{
	// Vertex shader - create, link, and compile
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for Vertex Shader errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SUCCESS::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for Fragment Shader errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SUCCESS::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// Link shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Check for linking shaders
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	glUseProgram(program);
	colorLocation = glGetUniformLocation(program, "colorTest");

	glUseProgram(0);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void createWindow()
{
	if (GLFW_TRUE != glfwInit())
	{
		cout << "Failed to init glfw" << '\n';
	}
	else
	{
		cout << "GLFW nit success" << '\n';
	}

	glfwSetErrorCallback(error_callback);

	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Step 1", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwShowWindow(window);

	auto gint = glewInit();
	if (gint != GLEW_OK)
	{
		cout << "Failed init glew" << '\n';
		return;
	}
	else 
	{
		auto glVersionBytes = (char*)glGetString(GL_VERSION);
		auto glVendorBytes = (char*)glGetString(GL_VERSION);
		string glVersion((char*)glVersionBytes);
		string glVendor((char*)glVendorBytes);

		cout << "glew init success" << '\n';
		cout << glVersion << '\n';
		cout << glVendor << '\n';
		cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
	}

}

void setupOpenGL()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

int main()
{
	createWindow();
	setupOpenGL();
	shaderProgram();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);

		glUniform3f(colorLocation, 0.0f, 0.5f, 0.65f);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
		glPointSize(20.0);
		glDrawArrays(GL_POINTS, 0, 2);
		glDrawArrays(GL_POINTS, 2, 2);
		glDrawArrays(GL_POINTS, 3, 2);
		glDrawArrays(GL_LINES, 6, 2);
		glDrawArrays(GL_LINES, 8, 2);
		glDrawArrays(GL_LINES, 10, 2);
		glDrawArrays(GL_LINES, 12, 2);

		glDisableVertexAttribArray(0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}
