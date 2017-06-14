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








TransformFeedbackManager::TransformFeedbackManager(char* shaderPath, glm::vec3* positions, glm::ivec3* faces, int faceCount)
{

	this->faceCount = faceCount;
	GLuint ShaderProgramID = glCreateProgram();

	GLchar* shaderSource = NULL;
	unsigned int shaderSize = -1;

	Shader_set::getShader(shaderPath, &shaderSource, &shaderSize);

	GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1,(const GLchar**)&shaderSource,  NULL);
	glCompileShader(vsID);
	compiledStatus(vsID);
	glAttachShader(ShaderProgramID, vsID);

	const GLchar* outputs[] = {"offset_out", "velocity_out","InitialLocation"};
	glTransformFeedbackVaryings(ShaderProgramID, 3, outputs, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(ShaderProgramID);
	glUseProgram(ShaderProgramID);

	

	
	//GLfloat zeros[] = {0.0, 0.0, 0.0, 0.0, 0.0};
	int dataSize = -1;
	GLfloat* data = NULL;
	fbi = setBufferData(positions,faces,faceCount,&data);



	glGenBuffers(1, &inBuff);
	glBindBuffer(GL_ARRAY_BUFFER, inBuff);
	glBufferData(GL_ARRAY_BUFFER, fbi.buffer_size, data, GL_STREAM_DRAW);


	Offset_attrib = glGetAttribLocation(ShaderProgramID,"Offset");
	Velocity_attrib = glGetAttribLocation(ShaderProgramID,"Velocity");
	InitialLocation_attrib = glGetAttribLocation(ShaderProgramID,"InitialLocation");


	glEnableVertexAttribArray(Offset_attrib);
	glEnableVertexAttribArray(Velocity_attrib);
	glEnableVertexAttribArray(InitialLocation_attrib);

	 stride = fbi.bytes_per_primitive; 
	glVertexAttribPointer(Offset_attrib,fbi.floats_per_texel,GL_FLOAT,GL_FALSE,stride,0);
	glVertexAttribPointer(Velocity_attrib,fbi.floats_per_texel,GL_FLOAT,GL_FALSE,stride,(void*)(sizeof(float) * fbi.floats_per_texel));
	glVertexAttribPointer(InitialLocation_attrib,fbi.floats_per_texel,GL_FLOAT,GL_FALSE,stride,(void*)(sizeof(float) * fbi.floats_per_texel * 2));




	glGenBuffers(1, &outBuff);
	glBindBuffer(GL_ARRAY_BUFFER, outBuff);
	glBufferData(GL_ARRAY_BUFFER, fbi.buffer_size, NULL , GL_STATIC_READ);


//	free(shaderSource);
	free(data);





}


void TransformFeedbackManager::ExecuteTransformFeedback()
{
	glUseProgram(ShaderProgramID);
	
	glEnable(GL_RASTERIZER_DISCARD);
	glBindBuffer(GL_ARRAY_BUFFER, inBuff);

	glEnableVertexAttribArray(Offset_attrib);
	glEnableVertexAttribArray(Velocity_attrib);
	glEnableVertexAttribArray(InitialLocation_attrib);


	glVertexAttribPointer(Offset_attrib,fbi.floats_per_texel,GL_FLOAT,GL_FALSE,stride,0);
	glVertexAttribPointer(Velocity_attrib,fbi.floats_per_texel,GL_FLOAT,GL_FALSE,stride,(void*)(sizeof(float) * fbi.floats_per_texel));
	glVertexAttribPointer(InitialLocation_attrib,fbi.floats_per_texel,GL_FLOAT,GL_FALSE,stride,(void*)(sizeof(float) * fbi.floats_per_texel * 2));





	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, outBuff);

	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0,faceCount);
	glEndTransformFeedback();

	glDisable(GL_RASTERIZER_DISCARD);

	std::swap(inBuff, outBuff);
}

GLuint TransformFeedbackManager::getOutputID()
{
	return inBuff;//After the executeTransformFeedback() gets called, the outBuff and inBuff get swapped, so the id for the buffer with the most recent output will always be inBuff;
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
		glm::vec3 triangle_center;
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
		(*data)[current_coord++] = 1.0;

		//initial triangle velocity
		(*data)[current_coord++] = 0.01;
		(*data)[current_coord++] = 0.0;
		(*data)[current_coord++] = 0.0;
		(*data)[current_coord++] = 1.0;

		//triangle center
		(*data)[current_coord++] = triangle_center.x;
		(*data)[current_coord++] = triangle_center.y;
		(*data)[current_coord++] = triangle_center.z;
		(*data)[current_coord++] = 1.0;

	
	}

	return fbi;
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