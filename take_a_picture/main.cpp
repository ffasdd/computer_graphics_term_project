#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include<stdbool.h>
#include <Windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(disable:4996)
using namespace std;
using namespace glm;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
void InitBuffer();
void InitTexture(const char* filename,unsigned int texture);
char* filetobuf(const char* file);
GLint height, width;
GLuint shaderProgram;
GLuint vertexShader;
GLuint fragmentShader;
GLuint VAO_axis, VBO_axis;
GLvoid keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
typedef struct obj {
	unsigned int texture;
	GLuint VAO, VBO_pos, VBO_normal, VBO_uv;
	std::vector< glm::vec3 > out_vertices;
	std::vector< glm::vec2 > out_uvs;
	std::vector< glm::vec3 > out_normals;
};
obj cam;
obj map;
obj char2;
obj char3;
obj char4;
obj balloon;
obj cube;
float plane[] = {
-0.5f, -0.5f, 0.5f, 0.0, 0.0, 1.0, 0.0, 0.0,
0.5f, -0.5f, 0.5f, 0.0, 0.0, 1.0, 1.0, 0.0,
0.5f, 0.5f, 0.5f, 0.0, 0.0, 1.0, 1.0, 1.0,
0.5f, 0.5f, 0.5f, 0.0, 0.0, 1.0, 1.0, 1.0,
-0.5f, 0.5f, 0.5f, 0.0, 0.0, 1.0, 0.0, 1.0,
-0.5f, -0.5f, 0.5f, 0.0, 0.0, 1.0, 0.0, 0.0

};
bool loadOBJ(const char* path, vector< glm::vec3 >& vertices, vector< glm::vec2 >& uvs, vector< glm::vec3 >& normals);
GLvoid keyboard(unsigned char key, int x, int y);

bool res = loadOBJ("cam.obj", cam.out_vertices, cam.out_uvs, cam.out_normals);
bool res2 = loadOBJ("char1_1.obj", char2.out_vertices, char2.out_uvs, char2.out_normals);
bool res3 = loadOBJ("char3.obj", char3.out_vertices, char3.out_uvs, char3.out_normals);
bool res5 = loadOBJ("char4.obj", char4.out_vertices, char4.out_uvs, char4.out_normals);
bool res6 = loadOBJ("balloon.obj", balloon.out_vertices, balloon.out_uvs, balloon.out_normals);
bool res7= loadOBJ("cube.obj", cube.out_vertices, cube.out_uvs,cube.out_normals);

bool res4 = loadOBJ("map10.obj", map.out_vertices, map.out_uvs, map.out_normals);
GLint object = cam.out_vertices.size();
GLint object3 = char3.out_vertices.size();
GLint object2 = char2.out_vertices.size();
GLint object4 = map.out_vertices.size();
GLint object5 = char4.out_vertices.size();
GLint object6 = balloon.out_vertices.size();
GLint object7 = cube.out_vertices.size();
int iWidth;
int iHeight;
int iChannels;
//함수=====================================
void blanksys();
void opensys();

//bool=====================================
//힌트 구조체
typedef struct Hint
{
	int num = 3;               //힌트 개수
	bool blank = false;
	bool hint_ = false;            //힌트 키 
	void hintsys() {            //타이머 

		cout << "3" << endl;
		Sleep(1000);
		cout << "2" << endl;
		Sleep(1000);
		cout << "1" << endl;


	}

};
Hint hint;
struct Element
{
	float x;
	float y;
	float z;
	float scalex;
	float scaley;
	float scalez;

};
Element blank_h;

float bufferx = 0;
float bufferz = 0;
float radius = 10.0;
float revolve3x = 0;
float revolve3z = 0;
float revolve2x = 0;
float revolve2z = 0;
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{ //--- 윈도우 생성하기
	
	height = 600;
	width = 1200;
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(width, height); // 윈도우의 크기 지정
	glutCreateWindow("cube");
	glewExperimental = GL_TRUE;
	glewInit();
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	make_shaderProgram(); //--- 세이더 프로그램 만들기
	InitBuffer();
	

	glutKeyboardFunc(keyboard);
	glutTimerFunc(100, TimerFunction, 1);
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape);
	glutMainLoop(); // 이벤트 처리 시작 
}




GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderProgram);

	unsigned int modelLocation = glGetUniformLocation(shaderProgram, "modelTransform");

	glm::vec3 cameraPos = glm::vec3(3.0, 1.0, 1.0);
	glm::vec3 cameraDirection = glm::vec3(0.0, 0.0, .0);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));

	unsigned int projectionLocation = glGetUniformLocation(shaderProgram, "projectionTransform");//--- 버텍스 세이더에서 viewTransform 변수위치
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	int objColorLocation = glGetUniformLocation(shaderProgram, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	unsigned int lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 13.0,13.0,12.0);

	int lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0,1.0,1.0);

	unsigned int viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, -5.0, 0.0, 0.0);


	{//UI

		glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 1.0);
		glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
		glViewport(-300, 350, width, height / 2.0);
		int objColorLocation = glGetUniformLocation(shaderProgram, "objectColor");
		glUniform3f(objColorLocation, 1.0, 0.0, 1.0);
		glUseProgram(shaderProgram);
		//ui 아직 안함
	}


	{//메인(플레이)화면
		glm::vec3 cameraPos = glm::vec3(10.0f, 10.0f, -1.f);
		glm::vec3 cameraDirection = glm::vec3(-11.0f, -10.0f, 2.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
		glViewport(-200, 0, width - 100, height);
		int objColorLocation = glGetUniformLocation(shaderProgram, "objectColor");
		glUniform3f(objColorLocation, 0.0, 0.0, 0.0);
		glUseProgram(shaderProgram);
		//시작 화면
		
	

		{

			glm::mat4 Signx = glm::mat4(1.0f);
			glm::mat4 Signy = glm::mat4(1.0f);
			glm::mat4 Signz = glm::mat4(1.0f);
			glm::mat4 SignRx = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 SignRz = glm::mat4(1.0f);
			glm::mat4 Signscale = glm::mat4(1.0f);
			glm::mat4 Sign = glm::mat4(1.0f);
			Signx = glm::translate(Signx, glm::vec3(2.5, 0.0, 0.0));
			Signy = glm::translate(Signy, glm::vec3(0.0, 12., 0.0));
			Signz = glm::translate(Signz, glm::vec3(0.0, 0.0, -2));
			SignRx = glm::rotate(SignRx, glm::radians(0.0f), glm::vec3(1.5, 0.0, 0.0));
			SignRy = glm::rotate(SignRy, glm::radians(200.0f), glm::vec3(0.0, 1.0, 0.0));
			SignRz = glm::rotate(SignRz, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
			Signscale = glm::scale(Signscale, glm::vec3(16., 13., 16.));
			Sign = Signx * Signy * Signz * SignRx * SignRy * SignRz * Signscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Sign));
			InitTexture("map10_I.png", map.texture);
			glBindTexture(GL_TEXTURE_2D, map.texture);
			glBindVertexArray(map.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object4);
		}//맵

		{
			glm::mat4 camx = glm::mat4(1.0f);
			glm::mat4 camy = glm::mat4(1.0f);
			glm::mat4 camz = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 camscale = glm::mat4(1.0f);
			glm::mat4 cam1 = glm::mat4(1.0f);
			SignRy = glm::rotate(SignRy, glm::radians(120.0f), glm::vec3(0.0, 1.0, 0.0));
			camx = glm::translate(camx, glm::vec3(1.0, 0.0, 0.0));
			camy = glm::translate(camy, glm::vec3(0.0, 0.5, 0.0));
			camz = glm::translate(camz, glm::vec3(0.0, 0.0, -1.));
			camscale = glm::scale(camscale, glm::vec3(0.5, 0.5, 0.5));
			cam1 = camx * camy * camz * SignRy * camscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
			InitTexture("char1_1.png", char2.texture);
			glBindTexture(GL_TEXTURE_2D, char2.texture);
			glBindVertexArray(char2.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object2);
		}//핑크 곰
		{
			glm::mat4 camx = glm::mat4(1.0f);
			glm::mat4 camy = glm::mat4(1.0f);
			glm::mat4 camz = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 camscale = glm::mat4(1.0f);
			glm::mat4 cam1 = glm::mat4(1.0f);
			SignRy = glm::rotate(SignRy, glm::radians(120.0f), glm::vec3(0.0, 1.0, 0.0));
			camx = glm::translate(camx, glm::vec3(3.0, 0.0, 0.0));
			camy = glm::translate(camy, glm::vec3(0.0, 0.5, 0.0));
			camz = glm::translate(camz, glm::vec3(0.0, 0.0, -1.));
			camscale = glm::scale(camscale, glm::vec3(0.5, 0.5, 0.5));
			cam1 = camx * camy * camz * SignRy * camscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
			InitTexture("char3.png", char3.texture);
			glBindTexture(GL_TEXTURE_2D, char3.texture);
			glBindVertexArray(char3.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object3);
		}//곰2
		{
			glm::mat4 camx = glm::mat4(1.0f);
			glm::mat4 camy = glm::mat4(1.0f);
			glm::mat4 camz = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 camscale = glm::mat4(1.0f);
			glm::mat4 cam1 = glm::mat4(1.0f);
			SignRy = glm::rotate(SignRy, glm::radians(120.0f), glm::vec3(0.0, 1.0, 0.0));
			camx = glm::translate(camx, glm::vec3(2.0, 0.0, 0.0));
			camy = glm::translate(camy, glm::vec3(0.0, 0.5, 0.0));
			camz = glm::translate(camz, glm::vec3(0.0, 0.0, -2.));
			camscale = glm::scale(camscale, glm::vec3(0.5, 0.5, 0.5));
			cam1 = camx * camy * camz * SignRy * camscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
			InitTexture("char4.png", char4.texture);
			glBindTexture(GL_TEXTURE_2D, char4.texture);
			glBindVertexArray(char4.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object5);
		}//곰3



	}


	{//현재 진행상황 화면
		glViewport(width - 300, height - 500, 300, 200);
		glm::vec3 cameraPos = glm::vec3(3.f, 1.0f, -1.5f);
		glm::vec3 cameraDirection = glm::vec3(.0f, 1.0f, 0.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

		{

			glm::mat4 Signx = glm::mat4(1.0f);
			glm::mat4 Signy = glm::mat4(1.0f);
			glm::mat4 Signz = glm::mat4(1.0f);
			glm::mat4 SignRx = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 SignRz = glm::mat4(1.0f);
			glm::mat4 Signscale = glm::mat4(1.0f);
			glm::mat4 Sign = glm::mat4(1.0f);
			Signx = glm::translate(Signx, glm::vec3(2.5, 0.0, 0.0));
			Signy = glm::translate(Signy, glm::vec3(0.0, 12., 0.0));
			Signz = glm::translate(Signz, glm::vec3(0.0, 0.0, -2));
			SignRx = glm::rotate(SignRx, glm::radians(0.0f), glm::vec3(1.5, 0.0, 0.0));
			SignRy = glm::rotate(SignRy, glm::radians(200.0f), glm::vec3(0.0, 1.0, 0.0));
			SignRz = glm::rotate(SignRz, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
			Signscale = glm::scale(Signscale, glm::vec3(16., 13., 16.));
			Sign = Signx * Signy * Signz * SignRx * SignRy * SignRz * Signscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Sign));
			InitTexture("map10_I.png", map.texture);
			glBindTexture(GL_TEXTURE_2D, map.texture);
			glBindVertexArray(map.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object4);
		}//맵
	
		{
			glm::mat4 camx = glm::mat4(1.0f);
			glm::mat4 camy = glm::mat4(1.0f);
			glm::mat4 camz = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 camscale = glm::mat4(1.0f);
			glm::mat4 cam1 = glm::mat4(1.0f);
			SignRy = glm::rotate(SignRy, glm::radians(120.0f), glm::vec3(0.0, 1.0, 0.0));
			camx = glm::translate(camx, glm::vec3(1.0, 0.0, 0.0));
			camy = glm::translate(camy, glm::vec3(0.0, 0.5, 0.0));
			camz = glm::translate(camz, glm::vec3(0.0, 0.0, -1.));
			camscale = glm::scale(camscale, glm::vec3(0.5, 0.5, 0.5));
			cam1 = camx * camy * camz * SignRy * camscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
			InitTexture("char1_1.png", char2.texture);
			glBindTexture(GL_TEXTURE_2D, char2.texture);
			glBindVertexArray(char2.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object2);
		}//곰1
		{
			glm::mat4 camx = glm::mat4(1.0f);
			glm::mat4 camy = glm::mat4(1.0f);
			glm::mat4 camz = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 camscale = glm::mat4(1.0f);
			glm::mat4 cam1 = glm::mat4(1.0f);
			SignRy = glm::rotate(SignRy, glm::radians(120.0f), glm::vec3(0.0, 1.0, 0.0));
			camx = glm::translate(camx, glm::vec3(3.0, 0.0, 0.0));
			camy = glm::translate(camy, glm::vec3(0.0, 0.5, 0.0));
			camz = glm::translate(camz, glm::vec3(0.0, 0.0, -1.));
			camscale = glm::scale(camscale, glm::vec3(0.5, 0.5, 0.5));
			cam1 = camx * camy * camz * SignRy * camscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
			InitTexture("char3.png", char3.texture);
			glBindTexture(GL_TEXTURE_2D, char3.texture);
			glBindVertexArray(char3.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object3);
		}//곰2
		{
			glm::mat4 camx = glm::mat4(1.0f);
			glm::mat4 camy = glm::mat4(1.0f);
			glm::mat4 camz = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 camscale = glm::mat4(1.0f);
			glm::mat4 cam1 = glm::mat4(1.0f);
			SignRy = glm::rotate(SignRy, glm::radians(120.0f), glm::vec3(0.0, 1.0, 0.0));
			camx = glm::translate(camx, glm::vec3(2.0, 0.0, 0.0));
			camy = glm::translate(camy, glm::vec3(0.0, 0.5, 0.0));
			camz = glm::translate(camz, glm::vec3(0.0, 0.0, -2.));
			camscale = glm::scale(camscale, glm::vec3(0.5, 0.5, 0.5));
			cam1 = camx * camy * camz * SignRy * camscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
			InitTexture("char4.png", char4.texture);
			glBindTexture(GL_TEXTURE_2D, char4.texture);
			glBindVertexArray(char4.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object5);
		}//곰3
		
	}
	{//예시(힌트)화면fdgdfg

		glViewport(width - 300, height - 200, 300,200);
		glm::vec3 cameraPos = glm::vec3(3.0f, 0.0f, -1.5f);
		glm::vec3 cameraDirection = glm::vec3(.0f, 0.0f, 0.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
		{

			glm::mat4 Signx = glm::mat4(1.0f);
			glm::mat4 Signy = glm::mat4(1.0f);
			glm::mat4 Signz = glm::mat4(1.0f);
			glm::mat4 SignRx = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 SignRz = glm::mat4(1.0f);
			glm::mat4 Signscale = glm::mat4(1.0f);
			glm::mat4 Sign = glm::mat4(1.0f);
			Signx = glm::translate(Signx, glm::vec3(3.5, 0.0, 0.0));
			Signy = glm::translate(Signy, glm::vec3(0.0, 2., 0.0));
			Signz = glm::translate(Signz, glm::vec3(0.0, 0.0, -2));
			SignRx = glm::rotate(SignRx, glm::radians(0.0f), glm::vec3(1.5, 0.0, 0.0));
			SignRy = glm::rotate(SignRy, glm::radians(200.0f), glm::vec3(0.0, 1.0, 0.0));
			SignRz = glm::rotate(SignRz, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
			Signscale = glm::scale(Signscale, glm::vec3(4, 3, 4));
			Sign = Signx * Signy * Signz * SignRx * SignRy * SignRz * Signscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Sign));
			InitTexture("map10_I.png", map.texture);
			glBindTexture(GL_TEXTURE_2D, map.texture);
			glBindVertexArray(map.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object4);
		}
	
	
		

	}
	glutSwapBuffers(); // 화면에 출력하기S


}


GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}



GLvoid InitBuffer() {

	//좌표축

	glGenVertexArrays(1, &cam.VAO);
	glGenBuffers(1, &cam.VBO_pos);
	glGenBuffers(1, &cam.VBO_normal);
	glGenBuffers(1, &cam.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(cam.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, cam.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, cam.out_vertices.size() * sizeof(glm::vec3), &cam.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute);


	glBindBuffer(GL_ARRAY_BUFFER, cam.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, cam.out_normals.size() * sizeof(glm::vec3), &cam.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, cam.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, cam.out_uvs.size() * sizeof(glm::vec2), &cam.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);

	///============================================
	glGenVertexArrays(1, &map.VAO);
	glGenBuffers(1, &map.VBO_pos);
	glGenBuffers(1, &map.VBO_normal);
	glGenBuffers(1, &map.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(map.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, map.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, map.out_vertices.size() * sizeof(glm::vec3), &map.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute2 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute2);


	glBindBuffer(GL_ARRAY_BUFFER, map.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, map.out_normals.size() * sizeof(glm::vec3), &map.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute2 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute2);

	glBindBuffer(GL_ARRAY_BUFFER, map.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, map.out_uvs.size() * sizeof(glm::vec2), &map.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute2 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute2);

	//=================================
	glGenVertexArrays(1, &char2.VAO);
	glGenBuffers(1, &char2.VBO_pos);
	glGenBuffers(1, &char2.VBO_normal);
	glGenBuffers(1, &char2.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(char2.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, char2.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, char2.out_vertices.size() * sizeof(glm::vec3), &char2.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute3 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute3);


	glBindBuffer(GL_ARRAY_BUFFER, char2.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, char2.out_normals.size() * sizeof(glm::vec3), &char2.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute3 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute3);

	glBindBuffer(GL_ARRAY_BUFFER, char2.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, char2.out_uvs.size() * sizeof(glm::vec2), &char2.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute3 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute3);

	//
	glGenVertexArrays(1, &char3.VAO);
	glGenBuffers(1, &char3.VBO_pos);
	glGenBuffers(1, &char3.VBO_normal);
	glGenBuffers(1, &char3.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(char3.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, char3.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, char3.out_vertices.size() * sizeof(glm::vec3), &char3.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute4 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute4);


	glBindBuffer(GL_ARRAY_BUFFER, char3.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, char3.out_normals.size() * sizeof(glm::vec3), &char3.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute4 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute4);

	glBindBuffer(GL_ARRAY_BUFFER, char3.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, char3.out_uvs.size() * sizeof(glm::vec2), &char3.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute4 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute4);
	//======
	glGenVertexArrays(1, &char4.VAO);
	glGenBuffers(1, &char4.VBO_pos);
	glGenBuffers(1, &char4.VBO_normal);
	glGenBuffers(1, &char4.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(char4.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, char4.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, char4.out_vertices.size() * sizeof(glm::vec3), &char4.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute5 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute5, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute5);


	glBindBuffer(GL_ARRAY_BUFFER, char4.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, char4.out_normals.size() * sizeof(glm::vec3), &char4.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute5 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute5);

	glBindBuffer(GL_ARRAY_BUFFER, char4.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, char4.out_uvs.size() * sizeof(glm::vec2), &char4.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute5 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute5, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute5);
	//========================
	glGenVertexArrays(1, &balloon.VAO);
	glGenBuffers(1, &balloon.VBO_pos);
	glGenBuffers(1, &balloon.VBO_normal);
	glGenBuffers(1, &balloon.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(balloon.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, balloon.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, balloon.out_vertices.size() * sizeof(glm::vec3), &balloon.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute6 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute6, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute6);


	glBindBuffer(GL_ARRAY_BUFFER, balloon.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, balloon.out_normals.size() * sizeof(glm::vec3), &balloon.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute6 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute6);

	glBindBuffer(GL_ARRAY_BUFFER, balloon.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, balloon.out_uvs.size() * sizeof(glm::vec2), &balloon.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute6 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute6);
	//----------
	glGenVertexArrays(1, &cube.VAO);
	glGenBuffers(1, &cube.VBO_pos);
	glGenBuffers(1, &cube.VBO_normal);
	glGenBuffers(1, &cube.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(cube.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, cube.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, cube.out_vertices.size() * sizeof(glm::vec3), &cube.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute7 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute7);


	glBindBuffer(GL_ARRAY_BUFFER, cube.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, cube.out_normals.size() * sizeof(glm::vec3), &cube.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute7 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute7, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute7);

	glBindBuffer(GL_ARRAY_BUFFER, cube.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, cube.out_uvs.size() * sizeof(glm::vec2), &cube.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute7 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute7, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute7);


}

void make_vertexShaders() {

	GLchar* vertexSource;

	//버텍스 세이더 읽어와서 객체 만들고 컴파일하기
	vertexSource = filetobuf("vertext.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader); //버텍스 컴파일 


	//에러코드 작성
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}

}



void make_fragmentShaders() {

	GLchar* fragmentSource;

	//프래그먼트 세이더 읽어와서 객체 만들고 컴파일하기
	fragmentSource = filetobuf("fregmentt.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader); //버텍스 컴파일 


	//에러코드 작성
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}

}




void make_shaderProgram() {

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);


	//프로그램 생성 후 객체 삭제 가능
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//에러코드 작성
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
	glUseProgram(shaderProgram);

}

void InitTexture(const char* filename ,unsigned int texture)
{


	glGenTextures(1, &texture);
	stbi_set_flip_vertically_on_load(true);
	//glBindTexture(GL_TEXTURE_2D, texture);
	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

//오브젝트 파일 읽기
bool loadOBJ(const char* path, vector< glm::vec3 >& vertices, vector< glm::vec2 >& uvs, vector< glm::vec3 >& normals) {
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	float aveX, aveY, aveZ;
	float scaleX, scaleY, scaleZ;
	float minX = 0.0, minY = 0.0, minZ = 0.0;
	float maxX = 0.0, maxY = 0.0, maxZ = 0.0;
	float scaleAll;

	float sizeX, sizeY, sizeZ;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < minX) minX = vertex.x;
			if (vertex.y < minY) minY = vertex.y;
			if (vertex.z < minZ) minZ = vertex.z;
			if (vertex.x > maxX) maxX = vertex.x;
			if (vertex.y > maxY) maxY = vertex.y;
			if (vertex.z > maxZ) maxZ = vertex.z;
			sumX += vertex.x;
			sumY += vertex.y;
			sumZ += vertex.z;



			temp_vertices.push_back(vertex);
		}
		//정점 텍스쳐 읽기
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		} //정점 노멀벡터 읽기 
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);

		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

		}


		aveX = sumX / vertexIndices.size();
		aveY = sumY / vertexIndices.size();
		aveZ = sumZ / vertexIndices.size();
		scaleX = maxX - minX;
		scaleY = maxY - minY;
		scaleZ = maxZ - minZ;
	}
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		glm::vec3 temp;
		unsigned int vertexIndex = vertexIndices[i];
		temp = temp_vertices[vertexIndex - 1];
		temp.x = temp.x - minX;
		temp.y = temp.y - minY;
		temp.z = temp.z - minZ;

		temp.x = ((temp.x * 2.0f) / scaleX) - 1.0f;
		temp.y = ((temp.y * 2.0f) / scaleY) - 1.0f;
		temp.z = ((temp.z * 2.0f) / scaleZ) - 1.0f;


		vertices.push_back(temp);
		//glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		//outvertex.push_back(vertex);
	}





	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 vertex = temp_uvs[uvIndex - 1];
		uvs.push_back(vertex);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 vertex = temp_normals[normalIndex - 1];
		normals.push_back(vertex);
	}


	return true;
}


//파일 읽기-----------------------
char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr);// Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0;
	// Null terminator 
	return buf; // Return the buffer 
}

void blanksys() {
	if (hint.blank) {
		int blankdir = 1;
		if (blank_h.x == -0.5)
		{
			blankdir = 0;
		}
		blank_h.x -= 0.5 * blankdir;
	}

}
void opensys() {
	if (hint.hint_) {
		if (hint.num >= 0) {
			int blankdir = 1;
			if (blank_h.x == 5.0) {
				blankdir = 0;
				hint.hint_ = false;
				hint.hintsys();
				hint.blank = true;
			}
			blank_h.x += 0.5 * blankdir;
		}

	}

}
void TimerFunction(int value)
{

	blanksys();
	opensys();
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}
GLvoid keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		//힌트
	case 's'://시작키
		hint.hintsys();
		hint.blank = true;
		break;
	case 'h':
		hint.blank = false;
		hint.hint_ = true;
		hint.num -= 1;
		cout << "남은 힌트-" << hint.num << endl;

		break;
	case '1'://확인용
		hint.blank = false;
		blank_h.x += 0.5;
		break;
	}
	glutPostRedisplay();
}