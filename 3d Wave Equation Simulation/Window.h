#ifndef WINDOW_H
#define WINDOW_H

class Window
{
public:

	Window(int width, int height, char * name);
	~Window();

	GLFWwindow * getWindow();
	void makeCurrentContext();
	void poolEvents();
	void setCallback(GLFWkeyfun fun);
	void setCallback(GLFWcursorposfun fun);
	void setCursorPosition(int x, int y);
	void printVersionInfo();
	void setWireframeMode(bool active);
	void swapBuffers();
	void disableCursor();
	void enableCursor();
	bool isInWireframeMode();
	bool running();
	bool isCursorEnabled();
	float getAspectRatio();
	

	glm::vec2 dimensions();

private:
	GLFWwindow * _window;
	int _height;
	int _width;
	char * _name;
	bool _wireframeMode = false;
	bool _isCursorEnabled = false;

};

#endif // !WINDOW_H
