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
glm::vec4 hitTri[3];
glm::vec4 hitQuad[4];
float hitQuadWidth;
float hitQuadHeight;
float meshDepth = 1.5;

std::vector<MeshInstance*> instances;

int screenHeight = 600;
int screenWidth = 800;


int lastTime = 0;
void render()
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	if(currentTime - lastTime >= 16)
	{
		tfm->executeUpdate(instances[0]);
		lastTime = currentTime;
	}



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
		float percentAlongWidth = (float)x/(float)screenWidth;//percent the click on the screen is along the width of the screen
		float percentAlongHeight =(float)y/(float)screenHeight;//percent the click on the screen is along the height of the screen.

		float explosionX = hitQuad[0].x + (percentAlongWidth * hitQuadWidth);//hitQuad[0] is upper left corner;
		float explosionY = hitQuad[0].y - (percentAlongHeight * hitQuadHeight);
		float explosionZ = meshDepth;
		tfm ->executeExplosion(-explosionX, explosionY, explosionZ-0.7, instances[0]);
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		tfm->executeReset();
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

	std::string model_path = "Assets/4hhp_teapot.obj";
	char* vertex_shader_path = ".\\shaders\\render\\Vertex.txt";
	char* frag_shader_path = ".\\shaders\\render\\frag.txt";
	char* feedback_updateShader = ".\\shaders\\feedback\\TransformFeedback_updateShader.txt";
	char* feedback_explosionShader = ".\\shaders\\feedback\\Explosion.txt";
	char* feedback_resetShader = ".\\shaders\\feedback\\resetShader.txt";
		

	init(&argc, (char**)&argv);
	
	MeshLoader::loadOBJ(model_path, &verts, &normals, &faces, &faceCount);
	
	render_shader = new Shader_set(vertex_shader_path, frag_shader_path);

	gBuff = new GeometryBuffer(render_shader->getProgram_id(),verts,normals,faces,faceCount);

	
	MeshInstance* defaultInstance = new MeshInstance(glm::vec4(0.0,0.0,meshDepth,1.0),glm::vec4(0.0,0.0,0.0,1.0),glm::vec4(2.0,2.0,1.8,1.0));
	instances.push_back(defaultInstance);

	mainCamera = new Camera(glm::vec3(0.0,0.0,-5.0),glm::vec3(0.0,0.0,0.0));
	mainCamera->initProjection(1.22, (float)screenWidth/(float)screenHeight,1.0,1000.0);

	glm::vec3 zfarPlane[4];
	mainCamera->getZfarPlane(zfarPlane);

	//creates base plane that will be used to create plane that spans the width and the height of the view
	hitTri[0] = glm::vec4(0.0,1.0,meshDepth,1.0);
	hitTri[1] = glm::vec4(-1.0,0.0,meshDepth,1.0);
	hitTri[2] = glm::vec4(0.0,0.0,meshDepth,1.0);

	mainCamera->getHitPlane(hitTri,hitQuad);
	
	hitQuadWidth = hitQuad[3].x - hitQuad[0].x;
	hitQuadHeight = hitQuad[0].y - hitQuad[1].y;

	tfm = new TransformFeedbackManager(feedback_updateShader,feedback_explosionShader,feedback_resetShader, verts, faces, faceCount);
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

