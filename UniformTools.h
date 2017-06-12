#include "stdafx.h"


class UniformTools{
private:
	UniformTools();
public:
	static void set_1ui_Uniform(char *uName,GLuint value,GLuint shaderProgramID);
	static void set_1i_Uniform(char *uName,GLint value,GLuint shaderProgramID);
	static void set_1f_Uniform(char *uName,GLfloat value,GLuint shaderProgramID);
	static void set_4x4f_matrix(char *uName,glm::mat4 matrix,GLuint shaderProgramID);
	static void set_3f_Uniform(char* uName, glm::vec3 valu, GLuint shaderProgramID);
	static void set_3x4f_matrix(char *uName,float* mat[3][4],GLuint spid);

};