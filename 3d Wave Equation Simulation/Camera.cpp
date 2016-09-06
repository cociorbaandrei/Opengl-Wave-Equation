#include "stdafx.h"
#include "Camera.h"




Camera::Camera(glm::vec3 position, GLfloat sensitivity, GLfloat movementSpeed) : _yaw(-90.f), _pitch(0)
{
	this->position = position;
	this->_sensitivity = sensitivity;
	this->_movementSpeed = movementSpeed;
	this->updateVectors();
}

void Camera::updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
	front.y = sin(glm::radians(this->_pitch));
	front.z = sin(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, glm::vec3(.0f, 1.f, .0f)));
	this->up    = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset)
{
	xoffset *= this->_sensitivity;
	yoffset *= this->_sensitivity;
	
	this->_yaw   += xoffset;
	this->_pitch += yoffset;

	this->clampValue(this->_pitch, -89.f, 89.f);
	this->updateVectors();
}

void Camera::lookAt(glm::vec3 target)
{
	front = target;
}

void Camera::clampValue(GLfloat & val, GLfloat min, GLfloat max)
{
	val = std::max(min, std::min(val, max));
}

void Camera::processKeyboardMovement(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = this->_movementSpeed * deltaTime;
	if (direction == FORWARD)
		this->position += this->front * velocity;
	if (direction == BACKWARD)
		this->position -= this->front * velocity;
	if (direction == LEFT)
		this->position -= this->right * velocity;
	if (direction == RIGHT)
		this->position += this->right * velocity;
}

glm::mat4 Camera::getViewMatrix()
{
	m_view = glm::lookAt(this->position, this->position + this->front, this->up);
	return m_view;
}


glm::vec3 Camera::getFrontVector()
{
	return front;
}

glm::vec3 Camera::getRightVector()
{
	return right;
}

glm::vec3 Camera::getPosition()
{
	return this->position;
}

glm::mat4 Camera::getProjection()
{
	return glm::perspective(glm::radians(45.0f), 8.f / 6.f, 0.01f, 100.0f);
}

glm::vec3 Camera::createRay(double mouseX, double mouseY, double z,  glm::mat4 model)
{
	// these positions must be in range [-1, 1] (!!!), not [0, width] and [0, height]

	glm::mat4 proj = this->getProjection();
	glm::mat4 view = this->getViewMatrix();

	
	float winX = mouseX;
	float winY = 600 - mouseY;
	float winZ = 2.0f * z - 1.0f;

	glm::mat4 invVP = glm::inverse(proj * view * model);

	winX = (winX) / 800 * 2.0 - 1.0;
	winY = (winY) / 600 * 2.0 - 1.0;

	glm::vec4 screenPos = glm::vec4(winX, winY, winZ, 1.0f);
	
	glm::vec4 worldPos =  invVP * screenPos;
	worldPos.w = 1.0 / worldPos.w;

	worldPos.x *= worldPos.w;
	worldPos.y *= worldPos.w;
	worldPos.z *= worldPos.w;

	glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

	return dir;
}

Camera::~Camera()
{
}
