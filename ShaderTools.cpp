
#include "stdafx.h"

using namespace std;


GLuint Shader_set::getVshader_id()
{
	return vertexShaderID;
}
GLuint Shader_set::getFshader_id()
{
	return fragmentShaderID;
}
GLuint Shader_set::getProgram_id()
{
	return shaderProgramID;
}


void Shader_set::getShader(char* shaderName,GLchar **fsource,GLuint *sSize)
{
	
	int shaderSize; 
	ifstream fp (shaderName,  std::ifstream::in);
	fp.clear();
	fp.seekg(0, fp.end);
	shaderSize = fp.tellg();

	(*fsource)=(GLchar*) malloc((sizeof(GLchar)*shaderSize)+1);
	fp.seekg(0, fp.beg);
	fp.read((*fsource), shaderSize);


	//makes sure that there are no characters after the last } in a shader which woudl prevent compilation
	char* lastBracket= strrchr((*fsource),'}');
	if(((*fsource)+shaderSize)>lastBracket+1&& lastBracket)
	{
		*sSize=((*fsource)+shaderSize)-lastBracket;
		memset(lastBracket+1,0,((*fsource)+shaderSize)-lastBracket);
		*(lastBracket+1)='\0';
	}
	else{
	(*fsource)[shaderSize]='\0';
	}

}

bool Shader_set::compiledStatus(GLint shaderID,char* Vname,char* Fname)
{
	GLint compiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (compiled==true) 
	{

		if(int e=glGetError())
			printf("compiledStatus(%s,%s) error# %d :%s\n",Vname,Fname,e,glewGetErrorString(e));
		
		return true;
	}
	else {

		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* msgBuffer = (GLchar*)malloc(sizeof(GLchar)*logLength);
		glGetShaderInfoLog(shaderID, logLength, NULL,msgBuffer);
		printf("ERROR LOG:\n%s\n\n",msgBuffer);
		delete (msgBuffer);
		return false;
	}
}

GLuint Shader_set::createVertexShader(GLchar** vsource,char* Vname)
{
	GLuint vShaderID = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vShaderID,1,(const GLchar**)vsource,NULL);
	glCompileShader(vShaderID);


	bool compiledCorrectly = compiledStatus(vShaderID,Vname," ");
	GLint compiled = 0;
	glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &compiled); 
	printf("Vertex Shader Compile status=%d\n",compiled);

	
	if (compiledCorrectly==1) {
	 	return vShaderID;
	 }
	std::cerr << "createVertexShader" << std::string(Vname) << "did not compile\n";
	 return -1;
}
GLuint Shader_set::createFragmentShader(GLchar** fsource,char* fName) 
{	
	GLuint fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLint compiled = 0;

	glShaderSource(fShaderID, 1, (const GLchar**)fsource,NULL);
	glCompileShader(fShaderID);
	
	glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &compiled); 
	printf("Frag Shader Compile status=%d\n",compiled);
	bool compiledCorrectly = compiledStatus(fShaderID,fName," ");
	 if (compiledCorrectly==1) {
	 	return fShaderID;
	 }

	std::cerr<<"Fragment Shader:"<< fName << "did not compile\n";
	 return -1;
}

GLuint Shader_set::createShaderProgram (char* Vname, char* Fname) {
	GLuint shaderID = glCreateProgram();                          
	GLint status=-1,vstatus=-1,logLength=-1;   

	printf("shaderID=%d\n",shaderID); 
	glAttachShader(shaderID, vertexShaderID);    
	glAttachShader(shaderID, fragmentShaderID);
	

	glLinkProgram(shaderID);                        

	               
	glGetProgramiv(shaderID,GL_LINK_STATUS,&status);          
	glGetProgramiv(shaderID,GL_VALIDATE_STATUS,&vstatus);     
	glGetProgramiv(shaderID,GL_INFO_LOG_LENGTH,&logLength);   
	printf("GL_LINK_STATUS=%d,GL_VALIDATE_STATUS=%d,GL_INFO_LOG_LENGTH=%d ",status,vstatus,logLength);

	GLchar* errorlog= (GLchar*) malloc(sizeof(GLchar)*256);
	GLint logsize=0;
	glGetProgramInfoLog(shaderID,256,&logsize,errorlog);
	if(logsize)printf("Shader Program Error Log:\n%s\n",errorlog);
	
	free(errorlog);
	return shaderID;
}

Shader_set::Shader_set(char* vName, char* fName)
{
	printf("\n=====ShaderInit Begin====\n");

	getShader(vName, &VertshaderSource,&vSize);                            
	vertexShaderID=createVertexShader(&VertshaderSource,vName);               

	getShader(fName,&FragshaderSource,&fSize);                              
	fragmentShaderID=createFragmentShader(&FragshaderSource,fName);          

	shaderProgramID=createShaderProgram((char*)(vName),(char*)fName);         
	printf("\n=====ShaderInit End====\n");
}

