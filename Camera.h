#include "stdafx.h"




class Camera
{
private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
public:
	Camera::Camera(glm::vec3 position, glm::vec3 lookAtPoint);
	glm::mat4 initProjection(float fovy, float aspect, float znear, float zfar);
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();


};