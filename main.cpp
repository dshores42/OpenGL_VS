#include <stdio.h>

#define GL_SILENCE_DEPRECATION

// Without this gl.h gets included instead of gl3.h
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// For includes related to OpenGL, make sure their are included after glfw3.h
#include <OpenGL/gl3.h>

void errorCallback(int error, const char* description)
{
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void frameBufferResizeCallback(GLFWwindow* window, int width, int height){
   glViewport(0, 0, width, height);
}

int main(void)
{
	GLFWwindow* window;

	// Set callback for errors
	glfwSetErrorCallback(errorCallback);

	// Initialize the library
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Without these two hints, nothing above OpenGL version 2.1 is supported
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(1024, 1024, "Running OpenGL on Mac", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Set callback for window
	glfwSetKeyCallback(window, keyCallback);

	// Set callback fro framebuffer
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Used to avoid screen tearing
	glfwSwapInterval(1);

	//OpenGL initializations start from here
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	/* Vertex array object*/
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex data and buffer
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Vertex shader
	const char* vertexShaderSource = "#version 410 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		fputs(infoLog, stderr);
	}

	// Fragment shader
	const char* fragmentShaderSource = "#version 410 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		fputs(infoLog, stderr);
	}

	// Shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
	  glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	  fputs(infoLog, stderr);
	}
	glUseProgram(shaderProgram);

	// Binding the buffers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//OpenGL initializations end here

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Resize the viewport
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// OpenGL Rendering related code
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}






////
////  main.cpp
////  OpenGL
////
////  Created by David Shores on 3/2/23.
////
//
//#define GLFW_INCLUDE_COREARB
////#define GL_SILENCE_DEPRECATION
//
//#include <iostream>
//#include <stdlib.h>
////#include <stdio.h>
//
////#include <OpenGL/gl.h>
////#include <OpenGL/glu.h>
//
////#include "linmath.h"
//
//#define GLFW_INCLUDE_NONE
//
//#include <GLFW/glfw3.h>
////#include <glad/glad.h>
//
//void error_callback(int error, const char* description)
//{
//	std::cout << "Error: " << description << std::endl;
//}
//
////static const struct
////{
////	float x, y;
////	float r, g, b;
////} vertices[3] =
////{
////	{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
////	{  0.6f, -0.4f, 0.f, 1.f, 0.f },
////	{   0.f,  0.6f, 0.f, 0.f, 1.f }
////};
////
////static const char* vertex_shader_text =
////"#version 110\n"
////"uniform mat4 MVP;\n"
////"attribute vec3 vCol;\n"
////"attribute vec2 vPos;\n"
////"varying vec3 color;\n"
////"void main()\n"
////"{\n"
////"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
////"    color = vCol;\n"
////"}\n";
////
////static const char* fragment_shader_text =
////"#version 110\n"
////"varying vec3 color;\n"
////"void main()\n"
////"{\n"
////"    gl_FragColor = vec4(color, 1.0);\n"
////"}\n";
//
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GLFW_TRUE);
//}
//
//int main(void)
//{
//	GLFWwindow* window;
////	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
////	GLint mvp_location, vpos_location, vcol_location;
//
//	glfwSetErrorCallback(error_callback);
//
//	if (!glfwInit())
//		exit(EXIT_FAILURE);
//
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1 );
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
//
//	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
//	if (!window)
//	{
//		glfwTerminate();
//		exit(EXIT_FAILURE);
//	}
//
//	glfwSetKeyCallback(window, key_callback);
//
//	glfwMakeContextCurrent(window);
////	gladLoadGL(glfwGetProcAddress);
//	glfwSwapInterval(1);
//
//	// NOTE: OpenGL error checks have been omitted for brevity
//#if 0
//	glGenBuffers(1, &vertex_buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
//	glCompileShader(vertex_shader);
//
//	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
//	glCompileShader(fragment_shader);
//
//	program = glCreateProgram();
//	glAttachShader(program, vertex_shader);
//	glAttachShader(program, fragment_shader);
//	glLinkProgram(program);
//
//	mvp_location = glGetUniformLocation(program, "MVP");
//	vpos_location = glGetAttribLocation(program, "vPos");
//	vcol_location = glGetAttribLocation(program, "vCol");
//
//	glEnableVertexAttribArray(vpos_location);
//	glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
//						  sizeof(vertices[0]), (void*) 0);
//	glEnableVertexAttribArray(vcol_location);
//	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
//						  sizeof(vertices[0]), (void*) (sizeof(float) * 2));
//
//	while (!glfwWindowShouldClose(window))
//	{
//		float ratio;
//		int width, height;
//		mat4x4 m, p, mvp;
//
//		glfwGetFramebufferSize(window, &width, &height);
//		ratio = width / (float) height;
//
//		glViewport(0, 0, width, height);
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		mat4x4_identity(m);
//		mat4x4_rotate_Z(m, m, (float) glfwGetTime());
//		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//		mat4x4_mul(mvp, p, m);
//
//		glUseProgram(program);
//		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
//		glDrawArrays(GL_TRIANGLES, 0, 3);
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//#endif
//
//	glfwDestroyWindow(window);
//
//	glfwTerminate();
//	exit(EXIT_SUCCESS);
//}
//
