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






//ShaderProgramID

TransformFeedbackManager::TransformFeedbackManager(char* updateShaderPath,char* explosionShaderPath, glm::vec3* positions, glm::ivec3* faces, int faceCount)
{
	 
	this->faceCount = faceCount;
	setupUpdateShader(updateShaderPath);
	setupExplosionShader(explosionShaderPath);
	//GLfloat zeros[] = {0.0, 0.0, 0.0, 0.0, 0.0};
	int dataSize = -1;
	GLfloat* data = NULL;
	fbi = setBufferData(positions,faces,faceCount,&data);
	stride = fbi.bytes_per_primitive;


	glGenBuffers(1, &inBuff);
	glBindBuffer(GL_ARRAY_BUFFER, inBuff);
	glBufferData(GL_ARRAY_BUFFER, fbi.buffer_size, data, GL_STREAM_DRAW);

	glGenBuffers(1, &outBuff);
	glBindBuffer(GL_ARRAY_BUFFER, outBuff);
	glBufferData(GL_ARRAY_BUFFER, fbi.buffer_size, NULL , GL_STATIC_READ);


//	free(shaderSource);
	free(data);

	



}


void TransformFeedbackManager::ExecuteTransformFeedback(MeshInstance* meshInstance)
{
	glBindVertexArray(0);
	glUseProgram(updateShaderID);
	
	glEnable(GL_RASTERIZER_DISCARD);
	glBindBuffer(GL_ARRAY_BUFFER, inBuff);

	glEnableVertexAttribArray(updateAttribs.Offset_attrib);
	glVertexAttribPointer(updateAttribs.Offset_attrib,fbi.floats_per_texel,GL_FLOAT,GL_FALSE,stride,0);

	glEnableVertexAttribArray(updateAttribs.Velocity_attrib);
	glVertexAttribPointer(updateAttribs.Velocity_attrib,fbi.floats_per_texel,GL_FLOAT,GL_FALSE,stride,(void*)(sizeof(float) * fbi.floats_per_texel));

	glEnableVertexAttribArray(updateAttribs.InitialLocation_attrib);
	glVertexAttribPointer(updateAttribs.InitialLocation_attrib,fbi.floats_per_texel,GL_FLOAT,GL_FALSE,stride,(void*)(sizeof(float) * fbi.floats_per_texel * 2));


	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, outBuff);

	glBeginTransformFeedback(GL_POINTS);
	UniformTools::set_4x4f_matrix("model_mMatrix", meshInstance->getModelMatrix(),ExplosionShaderID);
	glDrawArrays(GL_POINTS, 0,faceCount);
	glEndTransformFeedback();
	//glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
	glDisable(GL_RASTERIZER_DISCARD);

	std::swap(inBuff, outBuff);
	
}

GLuint TransformFeedbackManager::getOutputID()
{
	return inBuff;//After the executeTransformFeedback() gets called, the outBuff and inBuff get swapped, so the id for the buffer with the most recent output will always be inBuff;
}

void TransformFeedbackManager::setupUpdateShader(char* shaderPath)
{
	glBindVertexArray(0);
	if(int e=glGetError())
		printf("beginning of setupUpdateShader, current error is error# %d :%s\n",e,glewGetErrorString(e));

	updateShaderID = glCreateProgram();

	GLchar* shaderSource = NULL;
	unsigned int shaderSize = -1;

	Shader_set::getShader(shaderPath, &shaderSource, &shaderSize);

	GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1,(const GLchar**)&shaderSource,  NULL);



	glCompileShader(vsID);
	compiledStatus(vsID);
	glAttachShader(updateShaderID, vsID);

	const GLchar* outputs[] = {"offset_out", "velocity_out","InitialLocation_out"};
	glTransformFeedbackVaryings(updateShaderID, 3, outputs, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(updateShaderID);

	ShaderLinkandValidateStatus(updateShaderID);
	glUseProgram(updateShaderID);

	updateAttribs.InitialLocation_attrib = glGetAttribLocation(updateShaderID,"InitialLocation");
	updateAttribs.Velocity_attrib = glGetAttribLocation(updateShaderID,"Velocity");
	updateAttribs.Offset_attrib = glGetAttribLocation(updateShaderID,"Offset");
	glUseProgram(0);
}

void TransformFeedbackManager::setupExplosionShader(char* shaderPath)
{
	printf("===============setupExplosionShader() Begin==============\n");
	glBindVertexArray(0);
	if(int e=glGetError())
		printf("beginning of setupUpdateShader, current error is error# %d :%s\n",e,glewGetErrorString(e));

	ExplosionShaderID = glCreateProgram();

	GLchar* shaderSource = NULL;
	unsigned int shaderSize = -1;

	Shader_set::getShader(shaderPath, &shaderSource, &shaderSize);

	GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1,(const GLchar**)&shaderSource,  NULL);



	glCompileShader(vsID);
	compiledStatus(vsID);
	glAttachShader(ExplosionShaderID, vsID);

	const GLchar* outputs[] = {"offset_out", "velocity_out","InitialLocation_out"};
	glTransformFeedbackVaryings(ExplosionShaderID, 3, outputs, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(ExplosionShaderID);

	ShaderLinkandValidateStatus(ExplosionShaderID);
	glUseProgram(ExplosionShaderID);

	explosionAttribs.InitialLocation_attrib = glGetAttribLocation(ExplosionShaderID,"InitialLocation");
	explosionAttribs.Velocity_attrib = glGetAttribLocation(ExplosionShaderID,"Velocity");
	explosionAttribs.Offset_attrib = glGetAttribLocation(ExplosionShaderID,"Offset");
	glUseProgram(0);
	printf("===============setupExplosionShader() End==============\n");
}


FeedbackBufferInfo TransformFeedbackManager::setBufferData(glm::vec3* positions, glm::ivec3* faces,int faceCount, float** data)
{
	FeedbackBufferInfo fbi;
	int floats_per_texel = 4;//only need 3 floats, but GL_RGB32F image format is only supported by GL 4.0+ and i should probably keep this compatible with lower versions
	int texels_per_primitive = 3;//A triangle offset, and triangle velocity, location of triangle center
	int bytes_per_primitive = floats_per_texel * texels_per_primitive * sizeof(float);
	*data = (float*)malloc(faceCount*bytes_per_primitive);
	int dataSize = faceCount * bytes_per_primitive;
	fbi.floats_per_texel = floats_per_texel;
	fbi.texels_per_primitive = texels_per_primitive;
	fbi.bytes_per_primitive = bytes_per_primitive;
	fbi.buffer_size = dataSize;

	int current_coord = 0;

	
	for(int i=0; i<faceCount; i++)
	{
		glm::ivec3 position_indices = faces[i*2];
		glm::vec3 triangle_center = glm::vec3(0.0,0.0,0.0);
		for(int n=0; n<3; n++)
		{
			triangle_center += positions[position_indices[n]];
		}
		triangle_center.x = triangle_center.x/3.0;
		triangle_center.y = triangle_center.y/3.0;
		triangle_center.z = triangle_center.z/3.0;
		

		//initial triangle offset
		(*data)[current_coord++] = 0.0;
		(*data)[current_coord++] = 0.0;
		(*data)[current_coord++] = 0.0;
		(*data)[current_coord++] = 0.0;

		//initial triangle velocity
		(*data)[current_coord++] = 0.0;
		(*data)[current_coord++] = 0.0;
		(*data)[current_coord++] = 0.0;
		(*data)[current_coord++] = 0.0;

		//triangle center
		(*data)[current_coord++] = triangle_center.x;
		(*data)[current_coord++] = triangle_center.y;
		(*data)[current_coord++] = triangle_center.z;
		(*data)[current_coord++] = -1.0;
	
	}

	return fbi;
}


void TransformFeedbackManager::executeExplosion(float x, float y, float z,MeshInstance* meshInstance)
{

	glBindVertexArray(0);
	glUseProgram(ExplosionShaderID);
	
	glEnable(GL_RASTERIZER_DISCARD);
	glBindBuffer(GL_ARRAY_BUFFER, inBuff);

	glEnableVertexAttribArray(explosionAttribs.Offset_attrib);
	glVertexAttribPointer(explosionAttribs.Offset_attrib,fbi.floats_per_texel,GL_FLOAT,GL_FALSE,stride,0);

	glEnableVertexAttribArray(explosionAttribs.Velocity_attrib);
	glVertexAttribPointer(explosionAttribs.Velocity_attrib,fbi.floats_per_texel,GL_FLOAT,GL_FALSE,stride,(void*)(sizeof(float) * fbi.floats_per_texel));

	glEnableVertexAttribArray(explosionAttribs.InitialLocation_attrib);
	glVertexAttribPointer(explosionAttribs.InitialLocation_attrib,fbi.floats_per_texel,GL_FLOAT,GL_FALSE,stride,(void*)(sizeof(float) * fbi.floats_per_texel * 2));


	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, outBuff);

	glBeginTransformFeedback(GL_POINTS);

	UniformTools::set_4x4f_matrix("model_Matrix", meshInstance->getModelMatrix(),ExplosionShaderID);
	glDrawArrays(GL_POINTS, 0,faceCount);
	glEndTransformFeedback();
	//glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
	glDisable(GL_RASTERIZER_DISCARD);

	std::swap(inBuff, outBuff);


}

bool compiledStatus(GLuint vshaderID)
{
	GLint compiled = -2;
	glGetShaderiv(vshaderID, GL_COMPILE_STATUS, &compiled);
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
		glGetShaderiv(vshaderID, GL_INFO_LOG_LENGTH, &logLength);
		printf("log length = %d\n", logLength);
		GLchar* msgBuffer = (GLchar*)malloc(sizeof(GLchar)*logLength);
		glGetShaderInfoLog(vshaderID, logLength, NULL,msgBuffer);
		printf("ERROR LOG:\n%s\n\n",msgBuffer);
		delete (msgBuffer);
		return false;

	}

}

void ShaderLinkandValidateStatus(GLuint shaderProgramID)
{

	GLint status=-1,vstatus=-1,logLength=-1;   
	glGetProgramiv(shaderProgramID,GL_LINK_STATUS,&status);          
	glGetProgramiv(shaderProgramID,GL_VALIDATE_STATUS,&vstatus);     
	glGetProgramiv(shaderProgramID,GL_INFO_LOG_LENGTH,&logLength);   
	printf("GL_LINK_STATUS=%d,GL_VALIDATE_STATUS=%d,GL_INFO_LOG_LENGTH=%d\n ",status,vstatus,logLength);

	GLchar* errorlog= (GLchar*) malloc(sizeof(GLchar)*256);
	GLint logsize=0;
	glGetProgramInfoLog(shaderProgramID,256,&logsize,errorlog);
	if(logsize)printf("Shader Program Error Log:\n%s\n",errorlog);

	free(errorlog);
}