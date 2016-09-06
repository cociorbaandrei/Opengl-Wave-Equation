#ifndef CAMERA_H
#define  CAMERA_H

#pragma once
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.f,0.f,0.f), GLfloat sensitivity = .25f, GLfloat movementSpeed =3.f);
	
	void processMouseMovement(GLfloat xoffset, GLfloat yoffset);
	void lookAt(glm::vec3 target);
	void processKeyboardMovement(Camera_Movement direction, GLfloat deltaTime);
	
	glm::mat4 getViewMatrix();
	glm::vec3 getFrontVector();
	glm::vec3 getRightVector();
	glm::vec3 getPosition();
	glm::mat4 getProjection();
	glm::vec3 createRay(double normalizedX, double normalizedY);
	~Camera();

private:
	void updateVectors();
	void clampValue(GLfloat & val, GLfloat min, GLfloat max);
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 front = glm::vec3(.0f, 0.f, -1.0f);
	glm::mat4 m_view;
	GLfloat _pitch;
	GLfloat _yaw;
	GLfloat _sensitivity;
	GLfloat _movementSpeed;



};


#endif
