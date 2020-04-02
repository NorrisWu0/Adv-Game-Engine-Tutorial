#include <glad/glad.h>
#include <GlFW/glfw3.h>
#include <iostream>
#include <string>

using namespace std;

// Vertex Shader Source Code
const char* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);\n"
"}\n\0";

// Fragment Shader
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


int main() {
	glfwInit();
	// Tell glfw to load this version of OpenGL (3.3 in this instance)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Major version - 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Minor version - .3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core

	// Build a 1920x1080 window
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Adv Game Engine Tutorial", NULL, NULL);

	// Check if window is created
	if (window == NULL)
	{
		cout << "Window failed to build!!!" << endl;
		glfwTerminate();
		system("pause");
		return 0;
	}

	// Assign m_Window as current window for GLFW
	glfwMakeContextCurrent(window);

	// Check if GLAD failed to setup.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// Program failed.
		cout << "GLFW failed to initialize." << endl;
		glfwTerminate();
		system("pause");
		return 0;
	}
	
	#pragma region Vertex Shader - Draw Vertices
	int vertexShaderID = glCreateShader(GL_VERTEX_SHADER); // return a ID generated
	glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShaderID);

	int success; // Build checker
	char infoLog[512]; // Error log

	// Check compile status
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);

	if (!success) 
	{
		// Fetch error log
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		cout << "Vertex Compile Error: " << infoLog << endl;
		system("pause");
	}
	#pragma endregion

	#pragma region Fragment Shader
	int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderID);

	// Check compile status
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);

	if (!success) 
	{
		// Fetch error log
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		cout << "Fragment Compile Error: " << infoLog << endl;
		system("pause");
	}
	#pragma endregion

	#pragma region Shader Program
	int shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);

	glLinkProgram(shaderProgramID);
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);

	if (!success)
	{
		// Fetch error log
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		cout << "Shader Linking Error: " << infoLog << endl;
		system("pause");
	}
	#pragma endregion
	
	// Cleanup the shader
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	#pragma region Vertex Buffer Objects
	#pragma region Vertex Datas
	float triangleVs[] = 
	{
		-0.5, -0.5, 0,
		0, 0.5, 0,
		0.5, -0.5, 0
	};
 
	#pragma endregion

	// Vertex Buffer Objects - Generate buffer to store vertex data from RAM to VRAM
	unsigned int vboID1;
	glGenBuffers(1, &vboID1);

	// Vertex Array Objects - Generate buffer to pass VBO to vertex shader
	unsigned int vaoID1;
	glGenVertexArrays(1, &vaoID1);
	glBindVertexArray(vaoID1);

	// Mark this VBO current and active
	glBindBuffer(GL_ARRAY_BUFFER, vboID1);
	// Moving datas into VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVs), triangleVs, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	#pragma region Rect Objects
	float rectVs[] =
	{
		-0.5, 0.5, 0,
		0.5, 0.5, 0,
		-0.5, -0.5, 0,
		0.5, -0.5, 0
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		1, 2, 3,
	};

	unsigned int vaoID2;
	glGenVertexArrays(1, &vaoID2);

	unsigned int vboID2;
	glGenBuffers(1, &vboID2);
	
	unsigned int eboID1;
	glGenBuffers(1, &eboID1);
	
	glBindVertexArray(vaoID2);
	glBindBuffer(GL_ARRAY_BUFFER, vboID2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectVs), rectVs, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

#pragma endregion


	#pragma endregion

	#pragma region Game Loop - OpenGL equivilant of Update() in Unity.
	while (!glfwWindowShouldClose(window)) 
	{
		// Process Inputs

		#pragma region Preparing screen
		// Set clear color
		glClearColor(1, 1, 1, 1);
		// Clear Screen
		glClear(GL_COLOR_BUFFER_BIT);
		#pragma endregion

		#pragma region Drawing Vertices
		glUseProgram(shaderProgramID);
		glBindVertexArray(vaoID1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		#pragma endregion

		#pragma region Drawing Rectangle
		glUseProgram(shaderProgramID);
		glBindVertexArray(vaoID2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		#pragma endregion

		// Swap Buffers
		glfwSwapBuffers(window);

	}
	#pragma endregion

	glfwTerminate();
	system("pause");
}

