// glutEx1.cpp : 定义控制台应用程序的入口点。
//
//注意FPS函数的应用

#include <stdlib.h>
#include "glut.h"
#include <stdio.h>
#include <string.h>

#include "stanford_bunny.h"

float eye[] = {0, 4, 6};		// 观察位置（眼睛）
float center[] = {0, 0, 0};		// 视点位置
float fDistance = 0.2f;
float fRotate = 0;
bool bAnim = false;

bool bDrawList = false;
GLint tableList = 0;	// 桌子列表
GLint rabbitList = 0;	// 兔子列表
int num = 1;			// 兔子数

void DrawTable()
{
	glPushMatrix();
	glTranslatef(0, 3.5, 0);
	glScalef(7.5, 0.5, 6);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 1.5, 1.75);
	glScalef(1, 3, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 1.5, 1.75);
	glScalef(1, 3, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 1.5, -1.75);
	glScalef(1, 3, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 1.5, -1.75);
	glScalef(1, 3, 1);
	glutSolidCube(1.0);
	glPopMatrix();
}

GLint GenTableList()
{
	GLint lid = glGenLists(2);	// 生成两个空列表的编号
	
	glNewList(lid, GL_COMPILE);	// 指定编号为lid，编译模式为仅编译
	DrawTable();
	glEndList();

	// 新添加第二个列表作为兔子列表
	glNewList(lid + 1, GL_COMPILE);
	DrawBunny();
	glEndList();
	
	return lid;
}

void Draw_Table_List()
{
	glPushMatrix();
	glTranslatef(2.5, 4.5, 2.2); //平移与缩放
	glScalef(2, 2, 2);

	for (int i = 1; i <= num; i++) {
		glCallList(rabbitList);					// 调用显示列表画兔子
		if (i % 4 == 0) {
			glTranslatef(2.7f, 0, -0.7f);		// 换行
		}
		else 
			glTranslatef(-0.9f, 0.0f, 0.0f);	// 平移
	}

	glPopMatrix();
	
	glCallList(tableList);
}

void DrawScene()
{
	glPushMatrix();
	glTranslatef(2.5, 4.5, 2.2);
	glScalef(2, 2, 2);
	
	for (int i = 1; i <= num; i++) {
		DrawBunny();							// 直接画兔子
		if (i % 4 == 0) {
			glTranslatef(2.7f, 0, -0.7f);		// 换行
		}
		else
			glTranslatef(-0.9f, 0.0f, 0.0f);	// 平移
	}
	glPopMatrix();

	DrawTable();
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	gluPerspective(45, whRatio, 1, 1000);	

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void idle()
{
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }

	case 'a': 
		{
			eye[0] += fDistance; 
			center[0] += fDistance;
			break;
		}
	case 'd': 
		{
			eye[0] -= fDistance;
			center[0] -= fDistance;
			break;
		 }
	case 'w': 
		{
			eye[1] -= fDistance;
			center[1] -= fDistance; 
			break;
		}
	case 's': 
		{
			eye[1] += fDistance;
			center[1] += fDistance;
			break;
		}
	case 'z': 
		{
			eye[2] *= 0.95;
			break; 
		}
	case 'c': 
		{
			eye[2] *= 1.05;
			break;
		 }
	case 'l':
		{
			bDrawList = !bDrawList;	// 切换显示列表和非显示列表绘制方式
			break;
		}
	case ' ':
		{
			bAnim = !bAnim; 
			break;
		}
	case 'i':		// 加一只兔子
		{
			if (num < 16)
				num++;
			break;
		}
	case 'k':		// 减一只兔子
		{
			if (num > 1)
				num--;
			break;
		}
	default: break;
	}
}

void getFPS()
{
	static int frame = 0, time, timebase = 0;	// 帧、时间、初始时间
	static char buffer[256];	// 字符串缓冲区
	char mode[64];				// 模式
	if (bDrawList)				// 是否是列表模式
		strcpy(mode, "display list");
	else
		strcpy(mode, "naive");
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);			// 得到绘制时间
	if (time - timebase > 1000) {				// 每次分析取两次调用的间隔值
		sprintf(buffer,"FPS:%4.2f %s",
			frame*1000.0/(time-timebase), mode);
		timebase = time;		
		frame = 0;
	}
	char *c;
	glDisable(GL_DEPTH_TEST);	  // 禁用深度测试
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glOrtho(0,480,0,480,-1,1);    // 位置正投影
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glRasterPos2f(10,10);
	for (c=buffer; *c != '\0'; c++) 		
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);	// 在GLUT窗口写字符
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glEnable(GL_DEPTH_TEST);	  // 重启深度测试
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0.5, 0, 1);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// 场景（0，0，0）的视点中心 (0, 5, 50)，Y轴向上

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat gray[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat light_pos[] = {10, 10, 10, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,gray);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
	glEnable(GL_LIGHT0);

	if (bAnim) 
		fRotate += 0.5f;
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis

	glScalef(0.4, 0.4, 0.4);
	if (!bDrawList)
		DrawScene();						// old way
	else 
		Draw_Table_List();                  // new way

	getFPS();
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Exercise 4");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	tableList = GenTableList();
	rabbitList = GenTableList() + 1;

	glutMainLoop();
	return 0;
}


