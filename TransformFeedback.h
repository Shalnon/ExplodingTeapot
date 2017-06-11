
#include "stdafx.h";





class TransformFeedbackManager
{
private:
	GLuint VAO_ID;
	GLuint inBuff;
	GLuint outBuff;
	GLuint ShaderProgramID;
	void SwapBuffers();

	GLuint numInAttrib;
	GLuint getOutputID();

public:
	TransformFeedbackManager(char** shaderSource);
	void ExecuteTransformFeedback();




};

bool compiledStatus(GLuint shaderID);
