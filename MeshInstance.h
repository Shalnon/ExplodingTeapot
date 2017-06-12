#include "stdafx.h"


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

};