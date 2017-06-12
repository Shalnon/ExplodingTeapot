#include "stdafx.h"


typedef struct GeometryBufferInfo
{
	GLuint VBO_ID;
	GLuint VAO_ID;
	GLuint renderShaderProgram;
	
	int vertex_count;
	int face_count;
}GeometryBufferInfo;


class GeometryBuffer
{
private:
	GLuint VBO_ID;
	GLuint VAO_ID;
	GLuint renderShaderProgram;
	
	int vertex_count;
	int face_count;

public:
	GeometryBuffer(GLuint renderShader, glm::vec3* vertices, glm::vec3* normals, glm::ivec3* faces, int face_count);


	GeometryBufferInfo getInfo();
};