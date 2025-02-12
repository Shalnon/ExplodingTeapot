
#include "stdafx.h"

typedef struct FeedbackBufferInfo
{
	int floats_per_texel;
	int texels_per_primitive;
	int bytes_per_primitive;
	int buffer_size;

}FeedbackBufferInfo;

typedef struct AttribSet
{
	GLint Offset_attrib;
	GLint Velocity_attrib;
	GLint InitialLocation_attrib;

}AttribSet;

class TransformFeedbackManager
{
private:
	GLuint VAO_ID;
	GLuint inBuff;
	GLuint outBuff;
	GLint updateShaderID;
	GLint ExplosionShaderID;
	GLint resetShaderID;
	void SwapBuffers();
	int faceCount;

	FeedbackBufferInfo fbi;

	AttribSet updateAttribs;
	AttribSet explosionAttribs;
	AttribSet resetAttribs;

	GLuint Offset_attrib;
	GLuint Velocity_attrib;
	GLuint InitialLocation_attrib;
	int stride;
	FeedbackBufferInfo setBufferData(glm::vec3* positions, glm::ivec3* faces,int faceCount, float** data);
	void setupUpdateShader(char* shaderPath);
	void setupExplosionShader(char*shaderPath);
	void setupResetShader(char* shaderPath);
	

public:
	TransformFeedbackManager(char* updateShaderPath,char* explosionShaderPath,char* resetShaderPath,  glm::vec3* positions, glm::ivec3* faces, int faceCount);
	GLuint getOutputID();

	void executeUpdate(MeshInstance* meshInstance);
	void executeExplosion(float x, float y, float z,MeshInstance* meshInstance);
	void executeReset();


};

bool compiledStatus(GLuint shaderID);
void ShaderLinkandValidateStatus(GLuint shaderProgramID);