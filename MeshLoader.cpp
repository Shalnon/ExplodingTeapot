#include "stdafx.h"

using namespace std;


void countPrimitiveData(ifstream* ifs, int* vcount, int* ncount, int* fcount)
{

	ifs->seekg(0, ifs->beg);
			
	char buff[8];

	while(!ifs->eof())
	{

		ifs->get(buff,4);
		ifs->ignore(256,'\n');

		printf("%s\n",buff);
		if(buff[0] == 'v') 
		{
			if(buff[1] == 'n')
			{
				(*ncount)++;
			}
			else
			{
				(*vcount)++;
			}
		}
		else if(buff[0] == 'f')
		{
			(*fcount)++;
		}

	}



}

void MeshLoader::loadOBJ(std::string path, glm::vec3** positions, glm::vec3** normals, glm::ivec3** indices,int* faceCount)
{
	int vert_count = 0;
	int normal_count = 0;
	int face_count = 0;


	ifstream ifs;
	ifs.open(path);
	if(!ifs.is_open())
	{ 
		cerr<< path << "could not open";
		return; 
	}

	countPrimitiveData(&ifs, &vert_count, &normal_count, &face_count);
	printf("vert_count = %d\nnormal_count = %d\nface_count = %d\n",vert_count,normal_count,face_count);
	*faceCount = face_count;

	*positions = (glm::vec3*)malloc(sizeof(glm::vec3)*vert_count);
	*normals = (glm::vec3*)malloc(sizeof(glm::vec3)*normal_count);
	*indices = (glm::ivec3*)malloc(sizeof(glm::vec3)*face_count*2);//its twice face_count because theres is an ivec for the vertex index and ivec for normal index

	int current_vert = 0;
	int current_normal = 0;
	int current_face = 0;
	

	ifs.clear();
	ifs.seekg(0,ifs.beg);
	glm::ivec3 ones = glm::ivec3 (1,1,1);
	while(!ifs.eof())
	{
		glm::vec3 temp_vec;
		glm::ivec3 temp_ivec, temp_ivec2;

		char buff[256];
		ifs.getline(buff,256);
	

		if(buff[1] == 'n')
		{
			sscanf(buff, "vn %f %f %f", &(temp_vec.x),&(temp_vec.y),&(temp_vec.z));
			(*normals)[current_normal++] = temp_vec;
		}
		else if(buff[0] == 'v')
		{
			sscanf(buff,"v %f %f %f",&(temp_vec.x),&(temp_vec.y),&(temp_vec.z));
			(*positions)[current_vert++] = temp_vec;
		}
		else if(buff[0] == 'f')
		{
			sscanf(buff, "f %d//%d %d//%d %d//%d",&(temp_ivec[0]),&(temp_ivec2[0]),&(temp_ivec[1]),&(temp_ivec2[1]),&(temp_ivec[2]),&(temp_ivec2[2]));
			(*indices)[current_face*2] = temp_ivec - ones;//obj indices star at 1 instead of 0 so im subtracting 1 to convert to 0 based indices
			(*indices)[(current_face*2)+1] = temp_ivec2 - ones;
			current_face++;
		}


	}
};