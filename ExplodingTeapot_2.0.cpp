// ExplodingTeapot_2.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

char* TransformFeedbackShaderSource = 
	"#version 150 core\n\
	in float numIn;\n\
	out float numOut;\n\
	void main()\n\
	{\n\
		numOut = numIn + 1.0f;\n\
	}\n\
	";

Shader_set* render_shader;
GeometryBuffer* gBuff;
TransformFeedbackManager* tfm;
Camera* mainCamera;

std::vector<MeshInstance*> instances;

int screenHeight = 600;
int screenWidth = 800;

int count = 0;
void render()
{

	tfm->ExecuteTransformFeedback(instances[0]);


	GeometryBufferInfo gBuff_info = gBuff->getInfo();
	GLuint shaderID = render_shader->getProgram_id();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderID);

	glBindVertexArray(gBuff_info.VAO_ID);
	glBindBuffer(GL_ARRAY_BUFFER,gBuff_info.VBO_ID);


	UniformTools::set_4x4f_matrix("Projection", glm::transpose(mainCamera->getProjectionMatrix()),shaderID);
	UniformTools::set_4x4f_matrix("View", glm::transpose(mainCamera->getViewMatrix()),shaderID);

	GLuint tti = gBuff->getInfo().TBO_TEX_ID;
	
	glBindTexture(GL_TEXTURE_BUFFER, gBuff->getInfo().TBO_TEX_ID);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, tfm->getOutputID());


	for(int i=0; i<instances.size(); i++)
	{
		UniformTools::set_4x4f_matrix("Model", glm::transpose(instances[i]->getModelMatrix()),shaderID);
		glDrawArrays(GL_TRIANGLES,0,gBuff_info.face_count*3);
	}

	glutSwapBuffers();
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	

	glUseProgram(0);

}



void idle()
{

	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y)
{

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		tfm ->executeExplosion(0.0,0.0,0.0,instances[0]);
	}


}

void keyboardFunc(unsigned char key, int x, int y)
{


}


void init(int* argc, char* argv[])
{
	int width = screenWidth;
	int height = screenHeight;


	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_ALPHA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(width,height);
	glutCreateWindow("Sean_Halnon - Exploding Teapot");

	glClearColor(0.2,0.2,0.2,1.0);

	glutIdleFunc(idle);
	glutDisplayFunc(render);

	glutMouseFunc(mouseFunc);
	glutKeyboardFunc(keyboardFunc);

	glEnable(GL_DEPTH_TEST);

	glewInit();

}


void _tmain(int argc, _TCHAR* argv[])
{
	glm::vec3* verts = 0x0;
	glm::vec3* normals = 0x0;
	glm::ivec3* faces = 0x0;
	int faceCount = 0;

	std::string model_path = "Assets/hhp_teapot.obj";
	char* vertex_shader_path = ".\\shaders\\render\\Vertex.txt";
	char* frag_shader_path = ".\\shaders\\render\\frag.txt";
	char* feedback_updateShader = ".\\shaders\\feedback\\TransformFeedback_updateShader.txt";
	char* feedback_explosionShader = ".\\shaders\\feedback\\Explosion.txt";
		

	init(&argc, (char**)&argv);
	
	MeshLoader::loadOBJ(model_path, &verts, &normals, &faces, &faceCount);
	
	render_shader = new Shader_set(vertex_shader_path, frag_shader_path);

	gBuff = new GeometryBuffer(render_shader->getProgram_id(),verts,normals,faces,faceCount);

	MeshInstance* defaultInstance = new MeshInstance(glm::vec4(0.0,0.0,-2.0,1.0),glm::vec4(0.0,0.0,0.0,1.0),glm::vec4(2.0,2.0,2.0,1.0));
	instances.push_back(defaultInstance);

	mainCamera = new Camera(glm::vec3(0.0,0.0,2.0),glm::vec3(0.0,0.0,0.0));
	mainCamera->initProjection(1.22, (float)screenWidth/(float)screenHeight,1.0,1000.0);



	

	tfm = new TransformFeedbackManager(feedback_updateShader,feedback_explosionShader, verts, faces, faceCount);
	/*
	tfm->ExecuteTransformFeedback(instances[0]);
	tfm ->executeExplosion(0.0,0.0,0.0,instances[0]);


	glFlush();
	GLfloat feedback[12];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 24 * sizeof(float), sizeof(feedback), feedback);
	printf("feedback results: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", feedback[0],feedback[1],feedback[2],feedback[3],feedback[4],feedback[5],feedback[6],feedback[7],feedback[8],feedback[9],feedback[10],feedback[11]);

	glGetBufferSubData(GL_ARRAY_BUFFER, 24 * sizeof(float), sizeof(feedback), feedback);
	printf("input: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", feedback[0],feedback[1],feedback[2],feedback[3],feedback[4],feedback[5],feedback[6],feedback[7],feedback[8],feedback[9],feedback[10],feedback[11]);
	*/
	printf("Exiting");
	glutMainLoop();
}

