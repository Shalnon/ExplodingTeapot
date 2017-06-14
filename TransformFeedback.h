
#include "stdafx.h";

typedef struct FeedbackBufferInfo
{
	int floats_per_texel;
	int texels_per_primitive;
	int bytes_per_primitive;
	int buffer_size;

}FeedbackBufferInfo;



class TransformFeedbackManager
{
private:
	GLuint VAO_ID;
	GLuint inBuff;
	GLuint outBuff;
	GLuint ShaderProgramID;
	void SwapBuffers();
	int faceCount;

	FeedbackBufferInfo fbi;

	GLuint Offset_attrib;
	GLuint Velocity_attrib;
	GLuint InitialLocation_attrib;
	GLuint getOutputID();
	int stride;
	FeedbackBufferInfo setBufferData(glm::vec3* positions, glm::ivec3* faces,int faceCount, float** data);

public:
	TransformFeedbackManager(char* shaderPath, glm::vec3* positions, glm::ivec3* faces, int faceCount);
	void ExecuteTransformFeedback();




};

bool compiledStatus(GLuint shaderID);
