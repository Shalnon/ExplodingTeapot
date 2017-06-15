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
	
	glm::vec3 forward = glm::vec3(0.0,0.0,1.0);
	glm::vec3 rotated =lookAtPoint-position;
	float rotAngle = glm::angle(forward,rotated);
	glm::vec3 rotAxis = glm::cross(forward, rotated);

	rotation = glm::rotate(rotAngle, rotAxis);
	translation = glm::translate(position);
	viewMatrix = glm::lookAt(position,lookAtPoint,glm::vec3(0.0,1.0,0.0));
	
}

glm::mat4 Camera::initProjection(float fovy, float aspect, float znear, float zfar)
{
	projectionMatrix = glm::perspective(fovy,aspect,znear,zfar);
	this->fovy = fovy;
	this->aspectRatio = aspect;
	this->znear = znear;
	this->zfar = zfar;
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

void Camera::getZfarPlane(glm::vec3* zFarPlane_out)
{
	double zfar_height = (tan(fovy/2)*zfar)*2;
	double zfar_width = zfar_height * aspectRatio;
	double zfar_right = zfar_width/2.0;
	double zfar_top = zfar_height/2.0;

	glm::vec4 farPlane[4];
	farPlane[0] = glm::vec4(-zfar_right, zfar_top, zfar, 1.0);//top left corner
	farPlane[1] = glm::vec4(-zfar_right, -zfar_top, zfar, 1.0);//bottom left corner
	farPlane[2] = glm::vec4(zfar_right, -zfar_top, zfar, 1.0);//bottom right corner
	farPlane[3] = glm::vec4(zfar_right, zfar_top, zfar, 1.0);//top right corner
	
	
	for(int i=0; i<4; i++)
	{
		farPlane[i] = translation * farPlane[i] ;
		farPlane[i] = farPlane[i] / farPlane[i].w;
		zFarPlane_out[i] = glm::vec3(farPlane[i]);
	}



}

glm::mat4 Camera::getTranslationMatrix()
{
	return translation;
}

void ray_plane_intersection(glm::vec4 &origin, glm::vec4 &direction, glm::vec4 &v0, glm::vec4 &v1, glm::vec4 &v2, glm::vec4 &intersection)
{

	glm::vec4 norm_direction = glm::normalize(direction-origin);
	//printf("normalized direction = %.3f, %.3f, %.3f\n", direction.x, direction.y, direction.z);
	
	glm::vec3 v3normal = glm::triangleNormal(glm::vec3(v0),glm::vec3(v1),glm::vec3(v2));
	glm::vec4 v4normal = glm::vec4(v3normal.x, v3normal.y, v3normal.z, 1.0);
	float dist = 0.0;
	bool interscts  = glm::intersectRayPlane(origin,norm_direction,v0, v4normal, dist);
	//printf("dist = %f\n", dist);
	glm::vec4 v_dist = glm::vec4(dist,dist,dist,1.0);
	intersection = origin + (norm_direction * v_dist);
	

}

void Camera::getHitPlane(glm::vec4* basePlane, glm::vec4* hitPlane)
{
	glm::vec4 cameraLoc = translation * glm::vec4(0.0,0.0,0.0,1.0);
	glm::vec3 zFarPlane[4];
	getZfarPlane(zFarPlane);

	for(int i=0; i<4; i++)
	{
		glm::vec4 direction = glm::vec4(zFarPlane[i],1.0);
		ray_plane_intersection(cameraLoc,direction,basePlane[2],basePlane[1],basePlane[0],hitPlane[i]);
	}
}