#include "stdafx.h"


/*
class Camera
{
private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
public:
	Camera::Camera(glm::vec3 position, glm::vec3 lookAtPoint);
	glm::mat4 initProjection(float fovy, float aspect, float znear, float zfar);
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();


};
*/


Camera::Camera(glm::vec3 position, glm::vec3 lookAtPoint)
{
	viewMatrix = glm::lookAt(position,lookAtPoint,glm::vec3(0.0,1.0,0.0)); 
}

glm::mat4 Camera::initProjection(float fovy, float aspect, float znear, float zfar)
{
	projectionMatrix = glm::perspective(fovy,aspect,znear,zfar);
	return projectionMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}