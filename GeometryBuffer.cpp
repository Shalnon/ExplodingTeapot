#include "stdafx.h"

/*

typedef struct GeometryBufferInfo
{
	GLuint VBO_ID;
	GLuint VAO_ID;
	GLuint TBO_TEX_ID;
	GLuint renderShaderProgram;
	
	int vertex_count;
	int face_count;
}GeometryBufferInfo;


class GeometryBuffer
{
private:
	GLuint VBO_ID;
	GLuint VAO_ID;
	GLuint TBO_TEX_ID;//The id for the texture object that needs to be bound to GL_TEXTURE_BUFFER
	GLuint renderShaderProgram;
	
	int vertex_count;
	int face_count;

public:
	GeometryBuffer(GLuint renderShader, glm::vec3* vertices, glm::vec3* normals, glm::ivec3* faces, int face_count);


	GeometryBufferInfo getInfo();
};*/

GeometryBuffer::GeometryBuffer(GLuint renderShader, glm::vec3* positions, glm::vec3* normals, glm::ivec3* faces, int face_count)
{
	renderShaderProgram = renderShader;
	this->face_count = face_count;
	this->vertex_count = face_count*3;

	glGenVertexArrays(1, &VAO_ID);
	glGenBuffers(1,&VBO_ID);

	int vert_floats = 6;
	int vert_ints = 1;

	int verts_in = 0;
	int current_face = 0;
	int current_coord = 0;

	glBindVertexArray(VAO_ID);
	glBindBuffer(GL_ARRAY_BUFFER,VBO_ID);

	glBufferData(GL_ARRAY_BUFFER,(face_count * 3 * vert_floats * sizeof(GLfloat)) + (face_count * 3 * vert_ints * sizeof(GLint)), NULL, GL_STATIC_DRAW);

	void* vdata = glMapBuffer(GL_ARRAY_BUFFER,GL_READ_WRITE);
	
	GLfloat* vertex_data = (GLfloat*)vdata;
	GLint* ivertex_data = (GLint*)vdata;

	for(int i=0; i<face_count; i++)
	{
		glm::ivec3 position_indices = faces[i*2];
		glm::ivec3 normal_indices = faces[(i*2)+1];

		for(int n=0; n<3; n++)
		{
			glm::vec3 position = positions[position_indices[n]];
			glm::vec3 normal = normals[normal_indices[n]];

			vertex_data[current_coord++] = position.x;
			vertex_data[current_coord++] = position.y;
			vertex_data[current_coord++] = position.z;

			vertex_data[current_coord++] = normal.x;
			vertex_data[current_coord++] = normal.y;
			vertex_data[current_coord++] = normal.z;

			ivertex_data[current_coord++] = current_face;

		}
		current_face++;
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	GLint pos_attrib = glGetAttribLocation(renderShader,"Vposition");
	GLint normal_attrib = glGetAttribLocation(renderShader,"Vnormal");
	GLint face_index_attrib =  glGetAttribLocation(renderShader,"Vface_index");

	int stride = (vert_floats*sizeof(GLfloat)) + (vert_ints * sizeof(GLint));
	glVertexAttribPointer(pos_attrib,3,GL_FLOAT,GL_FALSE,stride,0);
	glVertexAttribPointer(normal_attrib,3,GL_FLOAT,GL_FALSE,stride,(void*)(sizeof(GLfloat)*3));		
	glVertexAttribIPointer(face_index_attrib,1,GL_INT,stride,(void*)(sizeof(GLfloat)*6));	


	glEnableVertexAttribArray(pos_attrib);
	glEnableVertexAttribArray(normal_attrib);
	glEnableVertexAttribArray(face_index_attrib);


	glGenTextures(1,&TBO_TEX_ID);



}

GeometryBufferInfo GeometryBuffer::getInfo()
{

	GeometryBufferInfo gbi;
	gbi.face_count = this->face_count;
	gbi.renderShaderProgram = this->renderShaderProgram;
	gbi.VAO_ID = this->VAO_ID;
	gbi.VBO_ID = this->VBO_ID;
	gbi.vertex_count = this->vertex_count;
	gbi.TBO_TEX_ID = this->TBO_TEX_ID;

	return gbi;

}