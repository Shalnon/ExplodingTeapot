#include "stdafx.h"




class Camera
{
private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	float fovy;
	float aspectRatio;
	float znear;
	float zfar;
	glm::mat4 translation;
	glm::mat4 rotation;
	glm::vec3 lookAtPoint;

public:
	Camera::Camera(glm::vec3 position, glm::vec3 lookAtPoint);
	glm::mat4 initProjection(float fovy, float aspect, float znear, float zfar);
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	
	void getZfarPlane(glm::vec3* zFarPlane_out);
	glm::mat4 getTranslationMatrix();
	void getHitPlane(glm::vec4* basePlane, glm::vec4* hitPlane);

};

void ray_plane_intersection(glm::vec4 &origin, glm::vec4 &direction, glm::vec4 &v0, glm::vec4 &v1, glm::vec4 &v2, glm::vec4 &intersection);
