// 3d Wave Equation Simulation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Window.h"
#include "GLShader.h"
#include "Camera.h"
#include "MeshGrid.h"
#include <time.h>
#include <math.h>




double normalizedX,  normalizedY;
GLfloat deltaTime = .0f;
GLfloat lastFrame = .0f;
GLfloat lastX = 400, lastY = 300, yaw, pitch;
Camera* mainCamera = new Camera(glm::vec3(.5f, 1.5f, 2.5f));
bool keys[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void do_movement();
int random(int min, int max);
Window* window = new Window(800, 600, "Mesh");


int main()
{
	
	window->setWireframeMode(false);
	window->disableCursor();
	window->setCursorPosition(399, 301);
	window->setCallback(key_callback);
	window->setCallback(mouse_callback);

	int num = 50;
	float worldLen = 1.f;


	MeshGrid* worldMesh = new MeshGrid(100, glm::vec3(1.f), "worldMeshShader.vert", "worldMeshShader.frag");
	worldMesh->Translate(glm::vec3(-50.f, 0.f, -50.f));




	glm::vec3 lightPos = glm::vec3(0.f, 1.1f, 0.f);
	MeshGrid* mesh = new MeshGrid(num, glm::vec3(0.f, 1.f, 0.f), "shader.vert", "shader.frag");
	mesh->Translate(glm::vec3(0.f, 1.f, 0.f))->Scale(glm::vec3(worldLen / num));
	mesh->GetShader()->useProgram();
	mesh->RecalculateNormals();

	
	GLint lightPosLoc = glGetUniformLocation(mesh->GetShader()->getProgram(), "lightPos");
	glUniform3fv(lightPosLoc, 1, &lightPos[0]);



	std::vector< std::vector< std::vector<float>> > u;
	u.resize(2);
	u[0].resize(num);
	for (int i = 0; i < num; i++)
		u[0][i].resize(num);

	u[1].resize(num);
	for (int i = 0; i < num; i++)
		u[1][i].resize(num);

	
	for (int i = 1; i < mesh->GetGridSize()-1; i++) {
		for (int j = 1; j < mesh->GetGridSize()-1; j++) {
		
			int x = i, y = j;
			float ampl = 19.6f;
			float x0 = num / 2;
			float y0 = num / 2;
			float theta = 3.8f;

			u[0][i][j] = ampl * exp(-((x-x0)*(x-x0)/2/theta/theta+(y-y0)*(y-y0)/2/theta/theta));
			u[1][i][j] = (ampl - .3) * exp(-((x - x0)*(x - x0) / 2 / theta / theta + (y - y0)*(y - y0) / 2 / theta / theta));

			y0 = num - y0;
			x0 = num - x0;

			//u[0][i][j] += -ampl * exp(-((x - x0)*(x - x0) / 2 / theta / theta + (y - y0)*(y - y0) / 2 / theta / theta));
			//u[1][i][j] += (-ampl - .3) * exp(-((x - x0)*(x - x0) / 2 / theta / theta + (y - y0)*(y - y0) / 2 / theta / theta));
		}
	}


	int currentTime = 1;
	float damping = 0, waveSpeed = .5;
	float dh = .002f;
	float dt = .0001f;
	float timmer = 10000.f;
	float timmer2 = 8.f;
	float minu = 1000;
	while (window->running())
	{
		window->poolEvents();

		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

	
		do_movement();

		
		if (timmer <= deltaTime) {
			
			u[currentTime][random(1, num)][random(1, num)] = -.4;
			timmer = .15f;

		}else timmer -= deltaTime;


		
		if (timmer2 <= deltaTime) {

			for (int i = 1; i < mesh->GetGridSize() - 1; i++) {
				for (int j = 1; j < mesh->GetGridSize() - 1; j++)
				{
					glm::vec3 position;
					int x = i, y = j;
					position.x = x;
					position.z = y;

					u[1 - currentTime][x][y] = 2 * u[currentTime][x][y] - u[1 - currentTime][x][y] + (dt * (u[currentTime][x + 1][y] - 2 * u[currentTime][x][y] + u[currentTime][x - 1][y]) / dh + dt * (u[currentTime][x][y + 1] - 2 * u[currentTime][x][y] + u[currentTime][x][y - 1]) / dh - dt * 2 * damping *(u[currentTime][x][y] - u[1 - currentTime][x][y])) * waveSpeed;
					position.y = u[1 - currentTime][x][y];
					mesh->GetVertex(i, j).position = position;
					mesh->GetVertex(i, j).color = mesh->getColor(position.y, -2,2);
				}
			}
			timmer2 = 0.001f;
		}else timmer2 -= deltaTime;


		currentTime = 1 - currentTime;
		mesh->RecalculateNormals();
		mesh->Draw(window, mainCamera);

		
		window->setWireframeMode(true);
		worldMesh->Draw(window, mainCamera);
		//window->setWireframeMode(false);
		
		window->swapBuffers();
	}

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (action == GLFW_PRESS && key != -1)
		keys[key] = true;
	else if (action == GLFW_RELEASE && key != -1) // Program was crashing when trying to set keys[-1] = true or false
		keys[key] = false;

}void do_movement()
{

	if (keys[GLFW_KEY_W])
	{
		mainCamera->processKeyboardMovement(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S])
	{
		mainCamera->processKeyboardMovement(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		mainCamera->processKeyboardMovement(RIGHT, deltaTime);
	}
	if (keys[GLFW_KEY_A])
	{
		mainCamera->processKeyboardMovement(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_LEFT_CONTROL]) {
		window->enableCursor();
	}
	else {
		window->disableCursor();
	}

}

void mouse_callback(GLFWwindow * _window, double xpos, double ypos)
{

	normalizedX = -1.0 + 2.0 * xpos / window->dimensions().x;
	normalizedY = 1.0 - 2.0 * ypos / window->dimensions().y;
	GLfloat xoffset = (GLfloat)xpos - lastX;
	GLfloat yoffset = lastY - (GLfloat)ypos;
	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	if (!window->isCursorEnabled()) {
		mainCamera->processMouseMovement(xoffset, yoffset);
	}
}

int random(int min, int max)
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL)); //seeding for the first time only!
		first = false;
	}
	return min + rand() % (max - min);
}
