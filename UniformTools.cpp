#include "stdafx.h"



void UniformTools::set_4x4f_matrix(char *uName,glm::mat4 matrix,GLuint spid)
{
	GLint uniformID = glGetUniformLocation(spid,uName);
	glUniformMatrix4fv(uniformID,1,0,glm::value_ptr(matrix));
	
}
void UniformTools::set_3x4f_matrix(char *uName,float* mat[3][4],GLuint spid)
{
	float matrix[12];
	for(int i=0; i<3; i++)
	{
		for(int n=0; n<4; n++)
		{
			matrix[(i*4)+n] = (*mat)[i][n];
		}
	}
	GLint uniformID = glGetUniformLocation(spid,uName);
	glUniformMatrix3x4fv(uniformID,1,0,matrix);
	
}

void UniformTools::set_1ui_Uniform(char *uName,GLuint value,GLuint spid)
{
	GLint uniformID = glGetUniformLocation(spid,uName);
	glUniform1ui(uniformID,value);
}

void UniformTools::set_3f_Uniform(char *uName,glm::vec3 value,GLuint spid)
{
	GLint uniformID = glGetUniformLocation(spid,uName);
	GLfloat data[] = {value.x,value.y,value.z};
	glUniform3f(uniformID,data[0],data[1],data[2]);
}

void UniformTools::set_1i_Uniform(char *uName,GLint value,GLuint spid)
{
	GLint uniformID = glGetUniformLocation(spid,uName);
	glUniform1i(uniformID,value);

}
void UniformTools::set_1f_Uniform(char *uName,GLfloat value,GLuint spid)
{
	GLint uniformID = glGetUniformLocation(spid,uName);
	glUniform1f(uniformID,value);

}