#include "gl/glut.h"
#include <cmath>
const GLfloat PI = 3.14159f;	// ����Բ����Ϊ������

void quad(GLfloat, GLfloat, GLfloat, GLfloat);
void rotate2d(GLfloat, GLfloat, GLfloat*, GLfloat*, GLfloat);
void star(GLfloat, GLfloat, GLfloat, GLfloat);

/* ����quad():����һ������ǵĵ�λ�ı��� */
/* ������˵����(cx,cy)Ϊ����ǵ��������꣬(vx,vy)Ϊ�����ĳһ����������� */
void quad(GLfloat cx, GLfloat cy, GLfloat vx, GLfloat vy)
{
	GLfloat coe1 = tan(PI / 10) / (tan(PI / 5) + tan(PI / 10));
	GLfloat coe2 = tan(PI / 5);
	
	GLfloat x1 = cx + coe1 * (vx - cx) + coe1 * coe2 * (vy - cy);
	GLfloat y1 = cy + coe1 * (vy - cy) + coe1 * coe2 * (cx - vx);
	GLfloat x2 = cx + coe1 * (vx - cx) - coe1 * coe2 * (vy - cy);
	GLfloat y2 = cy + coe1 * (vy - cy) - coe1 * coe2 * (cx - vx);

	glBegin(GL_POLYGON);
	glVertex3f(cx, cy, 0.5);
	glVertex3f(x1, y1, 0.5);
	glVertex3f(vx, vy, 0.5);
	glVertex3f(x2, y2, 0.5);
	glEnd();
}

/* ����rotate2d():ȷ����תһ���ǶȺ�λ�ı��εĶ���λ�ã����ڻ��� */
/* ������˵����(cx,cy)Ϊ����ǵ��������꣬(vx,vy)Ϊ�����ĳһ����������꣬thetaΪ��ת�Ƕȡ�ʵ�ʲ�����Ϊ72�� */
void rotate2d(GLfloat cx, GLfloat cy, GLfloat* vx, GLfloat* vy, GLfloat theta)
{
	GLfloat tx = cos(theta) * (*vx - cx) + sin(theta) * (*vy - cy);
	GLfloat ty = cos(theta) * (*vy - cy) + sin(theta) * (cx - *vx);
	*vx = cx + tx;
	*vy = cy + ty;
}

/* ����star():����һ������� */
/* ������˵����(cx,cy)Ϊ����ǵ��������꣬(vx,vy)Ϊ�����ĳһ����������� */
void star(GLfloat cx, GLfloat cy, GLfloat vx, GLfloat vy)
{
	/* ���÷ֱ����֮һ���ı��εķ�������һ������� */
	GLfloat rad = 2 * PI / 5;
	for (int i = 0; i < 5; i++)
	{
		quad(cx, cy, vx, vy);
		rotate2d(cx, cy, &vx, &vy, rad);
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glEnd();

	/* ���濪ʼ��������� */
	glColor3f(1, 1, 0);
	/* �ֱ�������cx, cy������vx, vy����¼ÿ������ǵ����ĺ�ĳһ���� */
	GLfloat cx[5] = { -0.5 / 3 * 2, -0.5 / 3, -0.5 / 6, -0.5 / 6, -0.5 / 3 };
	GLfloat cy[5] = { 0.5 / 2, 0.5 / 5 * 4, 0.5 / 5 * 3, 0.5 / 10 * 3, 0.5 / 10 };	// ���ĵ���������ֱ��ȷ��
	GLfloat vx[5], vy[5];
	vx[0] = -0.5 / 3 * 2;
	vy[0] = 0.5 / 5 * 4;		// ���ڶ������ֻ꣬����ȷ����������ǣ�����ĸ��ݼ��ι�ϵ�Ƶ������ʵ�鱨��
	for (int i = 1; i < 5; i++)
	{
		vx[i] = cx[i] - 0.5 / 10 * cos(atan((cy[0] - cy[i]) / (cx[0] - cx[i])));
		vy[i] = cy[i] - 0.5 / 10 * sin(atan((cy[0] - cy[i]) / (cx[0] - cx[i])));
	}
	star(cx[0], cy[0], vx[0], vy[0]);	
	star(cx[1], cy[1], vx[1], vy[1]);	
	star(cx[2], cy[2], vx[2], vy[2]);	
	star(cx[3], cy[3], vx[3], vy[3]);	
	star(cx[4], cy[4], vx[4], vy[4]);
	
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(400, 300);
	glutCreateWindow("Simple GLUT App");

	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}