#include "stdafx.h"

/*


class TransformFeedbackManager
{
private:
	GLuint VAO_ID;
	GLuint inBuff, outBuff;
	GLuint ShaderProgramID;
	void SwapBuffers();
public:
	TransformFeedbackManager(char* shaderSource);
	void ExecuteTransformFeedback();


};
*/




const int numCount = 5;

TransformFeedbackManager::TransformFeedbackManager(char** shaderSource)
{
	GLuint ShaderProgramID = glCreateProgram();

	GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1,(const GLchar**)shaderSource,  NULL);
	glCompileShader(vsID);
	compiledStatus(vsID);
	glAttachShader(ShaderProgramID, vsID);

	const GLchar* outputs[] = {"numOut"};
	glTransformFeedbackVaryings(ShaderProgramID, 1, outputs, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(ShaderProgramID);
	glUseProgram(ShaderProgramID);

	

	
	GLfloat zeros[] = {0.0, 0.0, 0.0, 0.0, 0.0};



	glGenBuffers(1, &inBuff);
	glBindBuffer(GL_ARRAY_BUFFER, inBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(zeros), zeros, GL_STREAM_DRAW);


	numInAttrib = glGetAttribLocation(ShaderProgramID,"numIn");
	glEnableVertexAttribArray(numInAttrib);
	glVertexAttribPointer(numInAttrib, 1,GL_FLOAT, GL_FALSE,0,0);



	glGenBuffers(1, &outBuff);
	glBindBuffer(GL_ARRAY_BUFFER, outBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(zeros), NULL , GL_STATIC_READ);






}


void TransformFeedbackManager::ExecuteTransformFeedback()
{
	glUseProgram(ShaderProgramID);
	
	glEnable(GL_RASTERIZER_DISCARD);
	glBindBuffer(GL_ARRAY_BUFFER, inBuff);

	glEnableVertexAttribArray(numInAttrib);
	glVertexAttribPointer(numInAttrib, 1,GL_FLOAT, GL_FALSE,0,0);


	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, outBuff);

	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0,numCount);
	glEndTransformFeedback();

	glDisable(GL_RASTERIZER_DISCARD);

	std::swap(inBuff, outBuff);
}

GLuint TransformFeedbackManager::getOutputID()
{
	return inBuff;//After the executeTransformFeedback() gets called, the outBuff and inBuff get swapped, so the id for the buffer with the most recent output will always be inBuff;
}

bool compiledStatus(GLuint shaderID)
{
	GLint compiled = -2;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if(compiled == GL_TRUE)
	{

		if(int e=glGetError())
			printf("compiledStatus() error# %d :%s\n",e,glewGetErrorString(e));
		
		return true;
	}
	else
	{
		printf("compile status = %d\n",compiled);
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		printf("log length = %d\n", logLength);
		GLchar* msgBuffer = (GLchar*)malloc(sizeof(GLchar)*logLength);
		glGetShaderInfoLog(shaderID, logLength, NULL,msgBuffer);
		printf("ERROR LOG:\n%s\n\n",msgBuffer);
		delete (msgBuffer);
		return false;

	}

}