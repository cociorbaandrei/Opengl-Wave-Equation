#include "stdafx.h"
#include "Window.h"



Window::Window(int width, int height, char * name) : _width(width), _height(height), _name(name)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 16);// Antialiasing
	this->_window = glfwCreateWindow(_width, _height, _name, NULL, NULL);

	if (!this->_window) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	this->makeCurrentContext();

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to instantiate GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	glViewport(0, 0, _width, _height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	this->setWireframeMode(false);

}


Window::~Window()
{
	glfwTerminate();
}

GLFWwindow * Window::getWindow()
{
	return this->_window;
}

void Window::makeCurrentContext()
{
	glfwMakeContextCurrent(this->_window);
}

void Window::poolEvents()
{
	glfwPollEvents();
}

void Window::setCallback(GLFWkeyfun fun)
{
	glfwSetKeyCallback(_window, fun);
}

void Window::setCallback(GLFWcursorposfun fun)
{
	glfwSetCursorPosCallback(_window, fun);
}

void Window::setCursorPosition(int x, int y)
{
	glfwSetCursorPos(_window, 399, 301);
}

void Window::printVersionInfo()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion =
		glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("GL Vendor : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version (string) : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version : %s\n", glslVersion);
}

void Window::setWireframeMode(bool active)
{
	this->_wireframeMode = active;
	glPolygonMode(GL_FRONT_AND_BACK, _wireframeMode ? GL_LINE : GL_FILL);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(this->_window);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::disableCursor()
{
	_isCursorEnabled = false;
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::enableCursor()
{
	_isCursorEnabled = true;
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool Window::isInWireframeMode()
{
	return this->_wireframeMode;
}

bool Window::running()
{
	return !glfwWindowShouldClose(this->getWindow());
}

bool Window::isCursorEnabled()
{
	return _isCursorEnabled;
}

float Window::getAspectRatio()
{
	return (float)_width / (float)_height;
}

glm::vec2 Window::dimensions()
{
	return glm::vec2(_width, _height);
}
