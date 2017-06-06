#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>



class MeshLoader
{

public:
	static void loadOBJ(std::string path, glm::vec3** positions, glm::vec3** normals,  glm::ivec3** indices);



};
