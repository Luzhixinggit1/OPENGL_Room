#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include "glut.h"
using namespace std;

#define BITMAP_ID 0x4D42

float eye[] = { 15, 60, 8 };
float center[] = { 15, 0, -20 };

GLuint texture[10];

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;	// �ļ�ָ��
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap�ļ�ͷ
	unsigned char	*bitmapImage;		// bitmapͼ������
	int	imageIdx = 0;		// ͼ��λ������
	unsigned char	tempRGB;	// ��������

								// �ԡ�������+����ģʽ���ļ�filename 
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) return NULL;
	// ����bitmap�ļ�ͼ
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// ��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// ����bitmap��Ϣͷ
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// Ϊװ��ͼ�����ݴ����㹻���ڴ�
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// ��֤�ڴ��Ƿ񴴽��ɹ�
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}
	// ����bitmapͼ������
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// ȷ�϶���ɹ�
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ
	for (imageIdx = 0;
	imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// �ر�bitmapͼ���ļ�
	fclose(filePtr);
	return bitmapImage;
}

void texload(int i, char *filename)
{
	BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap��Ϣͷ
	unsigned char*   bitmapData;                                       // ��������

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
}

void init()
{
	glGenTextures(10, texture);                                         // ��һ��������Ҫ���ɱ�ʾ���ĸ���, �ڶ������Ƿ��ر�ʾ��������
	texload(0, "Crack.bmp");
}

void draw_cuboid(float x, float y, float z) {
	//1
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(x, 0, 0);
	glVertex3f(x, 0, -z);
	glVertex3f(0, 0, -z);
	glEnd();

	//2
	glBegin(GL_POLYGON);
	glVertex3f(0, y, 0);
	glVertex3f(x, y, 0);
	glVertex3f(x, y, -z);
	glVertex3f(0, y, -z);
	glEnd();

	//3
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(x, 0, 0);
	glVertex3f(x, y, 0);
	glVertex3f(0, y, 0);
	glEnd();

	//4
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, -z);
	glVertex3f(x, 0, -z);
	glVertex3f(x, y, -z);
	glVertex3f(0, y, -z);
	glEnd();

	//5
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(0, y, 0);
	glVertex3f(0, y, -z);
	glVertex3f(0, 0, -z);
	glEnd();

	//6
	glBegin(GL_POLYGON);
	glVertex3f(x, 0, 0);
	glVertex3f(x, y, 0);
	glVertex3f(x, y, -z);
	glVertex3f(x, 0, -z);
	glEnd();
}

void Draw(){
	glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	//glColor3f(1, 0, 1);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2i(10, 10); glVertex3f(100, 0, 100);
	glTexCoord2i(10, 0); glVertex3f(100, 0, -100);
	glTexCoord2i(0, 0); glVertex3f(-100, 0, -100);
	glTexCoord2i(0, 10); glVertex3f(-100, 0, 100);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	
	//glColor3f(0, 1, 0);
	glPushMatrix();
	draw_cuboid(30, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	draw_cuboid(15, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0, -15);
	draw_cuboid(7.5, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.5, 0, -15);
	glRotatef(90, 0, 1, 0);
	draw_cuboid(15, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7, 0, -30);
	draw_cuboid(30.5, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(37.5, 0, -12);
	glRotatef(90, 0, 1, 0);
	draw_cuboid(18, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, 0, -11.5);
	draw_cuboid(7.5, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, 0, -0.5);
	glRotatef(90, 0, 1, 0);
	draw_cuboid(11.5, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16, 0, -11.5);
	draw_cuboid(13.5, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16.5, 0, -5.5);
	glRotatef(90, 0, 1, 0);
	draw_cuboid(6, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16.5, 7, -2.5);
	glRotatef(90, 0, 1, 0);
	draw_cuboid(3, 2, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16.5, 0, -0.5);
	glRotatef(90, 0, 1, 0);
	draw_cuboid(2, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(7.5, 0, -16.5);
	draw_cuboid(10, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(17.5, 7, -16.5);
	draw_cuboid(3, 2, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20.5, 0, -16.5);
	draw_cuboid(3.5, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(24, 7, -16.5);
	draw_cuboid(3, 2, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(27, 0, -16.5);
	draw_cuboid(10, 9, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(22.5, 0, -16.5);
	glRotatef(90, 0, 1, 0);
	draw_cuboid(13.5, 9, 0.5);
	glPopMatrix();
}

void reshape(int width, int height){
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;
	gluPerspective(45.0f, whRatio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void idle(){
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y){
	switch (k)
	{
	case 'a': {
		eye[0] -= 0.2f;
		center[0] -= 0.2f;
		break;
	}
	case 'd': {
		eye[0] += 0.2f;
		center[0] += 0.2f;
		break;
	}
	case 's': {
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
	}
	case 'w': {
		eye[1] += 0.2f;
		center[1] += 0.2f;
		break;
	}
	case 'z': {
		eye[2] -= 0.2f;
		center[2] -= 0.2f;
		break;
	}
	case 'c': {
		eye[2] += 0.2f;
		center[2] += 0.2f;
		break;
	}
	}
}

void redraw(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);

	Draw();						// Draw triangle

	glutSwapBuffers();
}

int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	int windowHandle = glutCreateWindow("Room");

	init();

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}