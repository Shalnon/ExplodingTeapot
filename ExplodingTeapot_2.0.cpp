// ExplodingTeapot_2.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

char* TransformFeedbackShaderSource = 
	"#version 150 core\n\
	in float numIn;\
	out float numOut;\
	void main()\
	{\
		numOut = numIn + 1.0f;\
	}\
	";

void init(int* argc, char* argv[])
{
	int width = 800;
	int height = 600;


	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_ALPHA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(width,height);
	glutCreateWindow("Sean_Halnon - Exploding Teapot");

	glClearColor(0.2,0.2,0.2,1.0);

	glEnable(GL_DEPTH_TEST);

	glewInit();

}


int _tmain(int argc, _TCHAR* argv[])
{
	glm::vec3* verts = 0x0;
	glm::vec3* normals = 0x0;
	 glm::ivec3* faces = 0x0;

	std::string path = "Assets/cube.obj";

	//MeshLoader::loadOBJ(path, &verts, &normals, &faces);

	init(&argc, (char**)&argv);

	TransformFeedbackManager* tfm = new TransformFeedbackManager(&TransformFeedbackShaderSource);

	for(int i=0; i<3; i++)
	{
		tfm->ExecuteTransformFeedback();
		
	}

	glFlush();
	GLfloat feedback[5];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
	printf("feedback results: %f, %f, %f, %f, %f\n", feedback[0],feedback[1],feedback[2],feedback[3],feedback[4]);

	glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(feedback), feedback);
	printf("input: %f, %f, %f, %f, %f\n", feedback[0],feedback[1],feedback[2],feedback[3],feedback[4]);

	return 0;
}

