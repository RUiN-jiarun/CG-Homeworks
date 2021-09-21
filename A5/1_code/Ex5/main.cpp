// glutEx1.cpp : 定义控制台应用程序的入口点。
//


#include <stdlib.h>
#include "glut.h"

float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

// Light
GLfloat baselight[] = { 5,5,5,1 };
GLfloat color1[] = { 1.0f,1.0f,1.0f,1 };	// Color1:white
GLfloat color2[] = { 1.0f,0,0,1 };			// Color2:red
bool colorChange = false;
bool lightChange = false;
bool spotOff = true;
GLfloat spotlight[] = { 0,5,0,1 };
GLfloat dir[] = { 0,-1,0 };
GLfloat ang = 60.0f;

void Draw_Leg();

void Draw_Triangle() // This function draws a triangle with RGB colors
{
	// Set material
	GLfloat specular[] = { 0.6f, 0.6f, 0.6f,1 };
	GLfloat diffuse[] = { 0.92f,0.89f,0.41f,1 };
	GLfloat face[] = { 1,0,0,1 };
	GLfloat leg1[] = { 0,1,0,1 };
	GLfloat leg2[] = { 1,1,0,1 };
	GLfloat leg3[] = { 0,1,1,1 };
	GLfloat leg4[] = { 0,0,1,1 };
	
	// Distribute the material to the objects
	// Teapot	
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);	//正反面的镜面反射颜色
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 50);		//镜面反射系数
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);	//正反面的漫反射颜色
	glTranslatef(0, 0, 4+1);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(1);
	glPopMatrix();

	// Table face
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, face);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, face);
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	// Table legs
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, leg1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, leg1);
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, leg2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, leg2);
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, leg3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, leg3);
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, leg4);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, leg4);
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

}

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
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
	
	case '5': {		// 操作对象切换
		lightChange = !lightChange;
		break;
	}
	case '.': {		// 灯光颜色切换
		colorChange = !colorChange;
		break;
	}
	case '*': {		// 打开或关闭聚光灯
		spotOff = !spotOff;
		break;
	}
	case '4': {		// 光源左移
		if (!lightChange)	// 移动环境光
			baselight[0] -= 0.2f;
		else				// 移动聚光灯
			dir[0] -= 0.2f;			// 旋转聚光灯方向
		break;
	}
	case '6': {		// 光源右移
		if (!lightChange)	 // 移动环境光
			baselight[0] += 0.2f;
		else				// 移动聚光灯
			dir[0] += 0.2f;			// 旋转聚光灯方向
		break;
	}
	case '8': {		// 光源上移
		if (!lightChange)	 // 移动环境光
			baselight[1] += 0.2f;
		else				// 移动聚光灯
			dir[1] += 0.2f;
		break;
	}
	case '2': {		// 光源下移
		if (!lightChange)	 // 移动环境光
			baselight[1] -= 0.2f;
		else				// 移动聚光灯
			dir[1] -= 0.2f;
		break;
	}
	case '1': {		// 光源前移
		if (!lightChange)	 // 移动环境光
			baselight[2] += 0.2f;
		else				// 移动聚光灯
			dir[2] += 0.2f;
		break;
	}
	case '3': {		// 光源后移
		if (!lightChange)	 // 移动环境光
			baselight[2] -= 0.2f;
		else				// 移动聚光灯
			dir[2] -= 0.2f;
		break;
	}
	case '+': {				// 增大聚光灯角度
		if (ang < 90.0) {
			ang += 3.0f;
		}
		break;
	}
	case '-': {				// 减小聚光灯角度
		if (ang > 0.0) {
			ang -= 3.0f;
		}
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
	
	// Baselight
	glLightfv(GL_LIGHT0, GL_POSITION, baselight);
	if (colorChange)
		glLightfv(GL_LIGHT0, GL_AMBIENT, color2);
	else
		glLightfv(GL_LIGHT0, GL_AMBIENT, color1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, color1);	// 反射属性都设置为白色
	glEnable(GL_LIGHT0);

	// Spotlight
	glLightfv(GL_LIGHT1, GL_POSITION, baselight);
	if (colorChange)
		glLightfv(GL_LIGHT1, GL_AMBIENT, color2);
	else
		glLightfv(GL_LIGHT1, GL_AMBIENT, color1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, color1);	// 反射属性都设置为白色
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, ang);	// 裁剪角度
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);	// 光源方向
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);	// 聚集程度
	if (spotOff)
		glDisable(GL_LIGHT1);
	else
		glEnable(GL_LIGHT1);

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

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}


