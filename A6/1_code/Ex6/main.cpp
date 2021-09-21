// glutEx1.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include <stdlib.h>
#include "glut.h"
#include "glext.h"
#include <stdio.h>
#include <Windows.h>

#define BITMAP_ID 0x4D42
#define TEXW 64
#define TEXH 64

float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

bool bMulti = false;
bool bMytex = false;
bool bLight = true;

int wHeight = 0;
int wWidth = 0;

void Draw_Leg();

PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB = NULL;
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB = NULL;

GLuint texture[] = { 0,0,0,0 };
// �����ʾ�����飬������������ı�ʾ��
// ����: ͨ��ָ�룬����filename ָ����bitmap�ļ������ݡ�
// ͬʱҲ����bitmap��Ϣͷ.����֧��-bitλͼ��

/* ����������ͼ�ļ����� */
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

/* ���������� */
void texload(int i, char *filename)
{

	BITMAPINFOHEADER bitmapInfoHeader;                   // bitmap��Ϣͷ
	unsigned char*   bitmapData;                         // ��������

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap���(ͨ��Ϊ����ʾ���ϲ�) 
		GL_RGB,	//����ϣ���������к졢�̡�������
		bitmapInfoHeader.biWidth, //��������������n�����б߿�+2 
		bitmapInfoHeader.biHeight, //����߶ȣ�������n�����б߿�+2 
		0, //�߿�(0=�ޱ߿�, 1=�б߿�) 
		GL_RGB,	//bitmap���ݵĸ�ʽ
		GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
		bitmapData);	//bitmap����ָ��  
}

GLubyte tex[TEXH][TEXW][3];		// �Զ�������

/* �����Զ������� */
void MyTex()
{
	for (int i = 0; i < TEXH; i++)
	{
		for (int j = 0; j < TEXW; j++)
		{
			if (!((i / 8) % 2))
			{
				if ((j / 8) % 2)
				{
					tex[i][j][0] = 0;
					tex[i][j][1] = 0;
					tex[i][j][2] = 0;
				}
				else
				{
					tex[i][j][0] = 255;
					tex[i][j][1] = 0;
					tex[i][j][2] = 0;
				}
			}
			else 
			{
				if ((j / 8) % 2)
				{
					tex[i][j][0] = 255;
					tex[i][j][1] = 0;
					tex[i][j][2] = 0;
				}
				else
				{
					tex[i][j][0] = 0;
					tex[i][j][1] = 0;
					tex[i][j][2] = 0;
				}
			}
		}
	}
}

/* ���������� */
void init()
{
	glGenTextures(4, texture);   
	// ��һ��������Ҫ���ɱ�ʾ���ĸ���, �ڶ������Ƿ��ر�ʾ��������
	texload(0, "Monet.bmp");
	texload(1, "Crack.bmp");
	texload(2, "Spot.bmp");
	
	// ���������Զ�������
	// XXXXXX();  �ο�opengl red book����������ͺ����Ĳ��衣
	MyTex();
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
	//�������ش洢ģʽ��������ȡ��ͼ�����ݵ��ж��뷽ʽ.
	glTexImage2D(GL_TEXTURE_2D, 0, 3, TEXW, TEXH, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

static const GLfloat vertex[8][3] = {
	   -0.5 ,-0.5 ,-0.5 ,
	   0.5 ,-0.5 ,-0.5 ,
	   -0.5 ,0.5 ,-0.5 ,
	   0.5 ,0.5 ,-0.5 ,
	   -0.5 ,-0.5 ,0.5 ,
	   0.5 ,-0.5 ,0.5 ,
	   -0.5 ,0.5 ,0.5 ,
	   0.5 ,0.5 ,0.5 ,
};

/* ��дһ������������ķ��� */
void Draw_Cube()
{
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3fv(vertex[0]);
	glTexCoord2f(0, 1); glVertex3fv(vertex[2]);
	glTexCoord2f(1, 1); glVertex3fv(vertex[3]);
	glTexCoord2f(1, 0); glVertex3fv(vertex[1]);

	glTexCoord2f(0, 0); glVertex3fv(vertex[0]);
	glTexCoord2f(1, 0); glVertex3fv(vertex[1]);
	glTexCoord2f(0, 1); glVertex3fv(vertex[5]);
	glTexCoord2f(1, 1); glVertex3fv(vertex[4]);

	glTexCoord2f(0, 0); glVertex3fv(vertex[4]);
	glTexCoord2f(1, 0); glVertex3fv(vertex[5]);
	glTexCoord2f(1, 1); glVertex3fv(vertex[7]);
	glTexCoord2f(0, 1); glVertex3fv(vertex[6]);

	glTexCoord2f(0, 0); glVertex3fv(vertex[0]);
	glTexCoord2f(0, 1); glVertex3fv(vertex[4]);
	glTexCoord2f(1, 1); glVertex3fv(vertex[6]);
	glTexCoord2f(1, 0); glVertex3fv(vertex[2]);

	glTexCoord2f(0, 0); glVertex3fv(vertex[1]);
	glTexCoord2f(1, 0); glVertex3fv(vertex[3]);
	glTexCoord2f(1, 1); glVertex3fv(vertex[7]);
	glTexCoord2f(0, 1); glVertex3fv(vertex[5]);

	glTexCoord2f(0, 0); glVertex3fv(vertex[2]);
	glTexCoord2f(0, 1); glVertex3fv(vertex[6]);
	glTexCoord2f(1, 1); glVertex3fv(vertex[7]);
	glTexCoord2f(1, 0); glVertex3fv(vertex[3]);
	glEnd();
}

/* ���������� */
void DrawMultiCube()
{
	// �����չ��ӿں���ָ��
	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
	
	// �����������
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);// �滻

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);// ����
	
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glBegin(GL_QUADS);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0);
	glVertex3fv(vertex[0]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 1);
	glVertex3fv(vertex[2]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 1); 
	glVertex3fv(vertex[3]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 0); 
	glVertex3fv(vertex[1]);

	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0);
	glVertex3fv(vertex[0]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 0);
	glVertex3fv(vertex[1]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 1); 
	glVertex3fv(vertex[5]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 1);
	glVertex3fv(vertex[4]);

	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0);
	glVertex3fv(vertex[4]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 0); 
	glVertex3fv(vertex[5]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 1);
	glVertex3fv(vertex[7]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 1); 
	glVertex3fv(vertex[6]);

	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0); 
	glVertex3fv(vertex[0]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 1);
	glVertex3fv(vertex[4]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 1);
	glVertex3fv(vertex[6]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 0); 
	glVertex3fv(vertex[2]);

	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0);
	glVertex3fv(vertex[1]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 0);
	glVertex3fv(vertex[3]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 1); 
	glVertex3fv(vertex[7]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 1); 
	glVertex3fv(vertex[5]);

	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0); 
	glVertex3fv(vertex[2]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 1); 
	glVertex3fv(vertex[6]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 1);
	glVertex3fv(vertex[7]);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 0); 
	glVertex3fv(vertex[3]);
	glEnd();
	glPopMatrix();

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

}


void Draw_Triangle() // This function draws a triangle with RGB colors
{
	/* ��� */
	if (!bMytex)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[3]);
	}	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	glTranslatef(0, 0, 4+1);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	/* ���� */
	if (!bMulti)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glPushMatrix();
		glTranslatef(0, 0, 3.5);
		glScalef(5, 4, 1);
		Draw_Cube();
		glPopMatrix();
	}
	else
	{
		DrawMultiCube();
	}
	glDisable(GL_TEXTURE_2D);

	/* ������������ */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

}

void Draw_Leg()
{
	glScalef(1, 1, 3);
	Draw_Cube();
}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	if (bPersp) {
		gluPerspective(45.0f, whRatio,0.1f,100.0f);
		//glFrustum(-3, 3, -3, 3, 3,100);
	} else {
		glOrtho(-3 ,3, -3, 3,-100,100);
	}

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = {0, 0, 8};
float center[] = {0, 0, 0};

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; break; }

	case ' ': {bAnim = !bAnim; break;}
	case 'o': {bWire = !bWire; break;}
	
	case '1': {bMytex = !bMytex; break; }
	case '2': {bMulti = !bMulti; break; }
	case '3': {bLight = !bLight; break; }

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
	case 'w': {
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
			  }
	case 's': {
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

	updateView(wHeight, wWidth);
}


void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// ������0��0��0�����ӵ����� (0,5,50)��Y������

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = {5,5,5,1};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white);
	if (bLight)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}	
	else
	{
		glDisable(GL_LIGHT0);
	}
	


	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Triangle();						// Draw triangle

	if (bAnim) fRotate    += 0.5f;
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Simple GLUT App");

	init();

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}


