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
Camera* mainCamera;

std::vector<MeshInstance*> instances;

int screenHeight = 600;
int screenWidth = 800;


void render()
{
	GeometryBufferInfo gBuff_info = gBuff->getInfo();
	GLuint shaderID = render_shader->getProgram_id();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderID);
	glBindVertexArray(gBuff_info.VAO_ID);
	glBindBuffer(GL_ARRAY_BUFFER,gBuff_info.VBO_ID);


	UniformTools::set_4x4f_matrix("Projection", glm::transpose(mainCamera->getProjectionMatrix()),shaderID);
	UniformTools::set_4x4f_matrix("View", glm::transpose(mainCamera->getViewMatrix()),shaderID);
	for(int i=0; i<instances.size(); i++)
	{
		UniformTools::set_4x4f_matrix("Model", glm::transpose(instances[i]->getModelMatrix()),shaderID);
		glDrawArrays(GL_TRIANGLES,0,gBuff_info.face_count*3);
	}

	glutSwapBuffers();
	glBindVertexArray(0);

}

void idle()
{
	glutPostRedisplay();
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


	glEnable(GL_DEPTH_TEST);

	glewInit();

}


void _tmain(int argc, _TCHAR* argv[])
{
	glm::vec3* verts = 0x0;
	glm::vec3* normals = 0x0;
	glm::ivec3* faces = 0x0;
	int faceCount = 0;

	std::string model_path = "Assets/cube.obj";
	char* vertex_shader_path = ".\\shaders\\vertex.txt";
	char* frag_shader_path = ".\\shaders\\frag.txt";
		

	init(&argc, (char**)&argv);
	
	MeshLoader::loadOBJ(model_path, &verts, &normals, &faces, &faceCount);
	
	render_shader = new Shader_set(vertex_shader_path, frag_shader_path);

	gBuff = new GeometryBuffer(render_shader->getProgram_id(),verts,normals,faces,faceCount);

	MeshInstance* defaultInstance = new MeshInstance(glm::vec4(0.0,0.0,0.0,1.0),glm::vec4(0.0,0.0,0.0,1.0),glm::vec4(1.0,1.0,1.0,1.0));
	instances.push_back(defaultInstance);

	mainCamera = new Camera(glm::vec3(-1.5,1.5,-2.0),glm::vec3(0.0,0.0,0.0));
	mainCamera->initProjection(40, (float)screenWidth/(float)screenHeight,1.0,100.0);

	glm::mat4 mvp = glm::transpose(mainCamera->getViewMatrix()) * glm::transpose(mainCamera->getProjectionMatrix());
	glm::vec4 lp = glm::vec4(0.0,0.0, -3.1, 1.0) * mvp;
	glm::vec4 v = glm::vec4(3.0,-3.0,-3.0,1.0) * mvp;
	printf("light position = %.2f  %.2f  %.2f  %.2f\n", lp.x, lp.y, lp.z, lp.w); 
	printf("vert position = %.2f  %.2f  %.2f  %.2f\n", v.x, v.y, v.z, v.w);


	/*

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
	*/
	printf("Exiting");
	glutMainLoop();
}

