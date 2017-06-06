// ExplodingTeapot_2.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	glm::vec3* verts = 0x0;
	glm::vec3* normals = 0x0;
	 glm::ivec3* faces = 0x0;

	std::string path = "Assets/cube.obj";

	MeshLoader::loadOBJ(path, &verts, &normals, &faces);



	return 0;
}

