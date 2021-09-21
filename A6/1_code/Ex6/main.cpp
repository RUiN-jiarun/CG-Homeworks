// glutEx1.cpp : 定义控制台应用程序的入口点。
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
// 纹理标示符数组，保存两个纹理的标示符
// 描述: 通过指针，返回filename 指定的bitmap文件中数据。
// 同时也返回bitmap信息头.（不支持-bit位图）

/* 读入纹理贴图文件函数 */
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;	// 文件指针
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap文件头
	unsigned char	*bitmapImage;		// bitmap图像数据
	int	imageIdx = 0;		// 图像位置索引
	unsigned char	tempRGB;	// 交换变量

	// 以“二进制+读”模式打开文件filename 
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) return NULL;
	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// 读入bitmap信息头
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// 为装载图像数据创建足够的内存
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// 验证内存是否创建成功
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 读入bitmap图像数据
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// 确认读入成功
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
	for (imageIdx = 0;
		imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// 关闭bitmap图像文件
	fclose(filePtr);
	return bitmapImage;
}

/* 加载纹理函数 */
void texload(int i, char *filename)
{

	BITMAPINFOHEADER bitmapInfoHeader;                   // bitmap信息头
	unsigned char*   bitmapData;                         // 纹理数据

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap层次(通常为，表示最上层) 
		GL_RGB,	//我们希望该纹理有红、绿、蓝数据
		bitmapInfoHeader.biWidth, //纹理宽带，必须是n，若有边框+2 
		bitmapInfoHeader.biHeight, //纹理高度，必须是n，若有边框+2 
		0, //边框(0=无边框, 1=有边框) 
		GL_RGB,	//bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		bitmapData);	//bitmap数据指针  
}

GLubyte tex[TEXH][TEXW][3];		// 自定义纹理

/* 绘制自定义纹理 */
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

/* 定义纹理函数 */
void init()
{
	glGenTextures(4, texture);   
	// 第一参数是需要生成标示符的个数, 第二参数是返回标示符的数组
	texload(0, "Monet.bmp");
	texload(1, "Crack.bmp");
	texload(2, "Spot.bmp");
	
	// 下面生成自定义纹理
	// XXXXXX();  参考opengl red book，理解后请解释函数的步骤。
	MyTex();
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
	//设置像素存储模式控制所读取的图像数据的行对齐方式.
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

/* 重写一个有纹理坐标的方块 */
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

/* 多重纹理方块 */
void DrawMultiCube()
{
	// 获得扩展库接口函数指针
	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
	
	// 混合纹理设置
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);// 替换

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);// 调整
	
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
	/* 茶壶 */
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

	/* 桌面 */
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

	/* 桌子其他部分 */
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
		0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

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


