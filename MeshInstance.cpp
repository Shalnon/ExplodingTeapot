#include "stdafx.h"

/*
class MeshInstance
{
private:
	glm::vec4 position;
	glm::vec4 rotation;
	glm::vec4 scale;

	glm::mat4 modelMatrix;
public:
	MeshInstance(glm::vec4 position, glm::vec4 rotation, glm::vec4 scale);
	glm::mat4 getModelMatrix();

};*/


MeshInstance::MeshInstance(glm::vec4 position, glm::vec4 rotation, glm::vec4 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;


	glm::mat4 mtranslation = glm::translate(glm::vec3(position));
	glm::mat4 mrotation = glm::translate(glm::vec3(position)) * glm::rotate(rotation.x, glm::vec3(1.0,0.0,0.0)) * glm::rotate(rotation.y,glm::vec3(0.0,1.0,0.0)) * glm::rotate(rotation.z,glm::vec3(0.0,0.0,1.0)) * mtranslation;
	glm::mat4 mscale = glm::scale(glm::vec3(scale));

	modelMatrix = mscale * mrotation * mtranslation;


}

glm::mat4 MeshInstance::getModelMatrix()
{
	return modelMatrix;
}