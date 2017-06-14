#include "stdafx.h"


class Shader_set
{
private:
	GLchar* VertshaderSource;
	GLchar* FragshaderSource;
	GLuint vertexShaderID, fragmentShaderID,shaderProgramID,vSize,fSize;
	GLuint createVertexShader(GLchar** vsource,char* Vname);
	GLuint createFragmentShader(GLchar** fsource,char* fName);
	GLuint createShaderProgram (char* Vname, char* Fname);
	bool compiledStatus(GLint shaderID,char* Vname,char* Fname);
public:
	
	static void getShader(char* shaderName,GLchar **fsource,GLuint *sSize);
	Shader_set(char* vName,char* fName);


	GLuint getVshader_id();
	GLuint getFshader_id();
	GLuint getProgram_id();
};


