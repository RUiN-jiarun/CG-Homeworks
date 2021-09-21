// main.cpp : 定义控制台应用程序的入口点。

#include "glut.h"

float fTranslate;		// 0.0
float fRotate;			// 0.0
float fScale = 1.0f;

/*void Draw_Triangle() // This function draws a triangle with RGB colors
{
	glBegin(GL_TRIANGLES);
		 glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		 glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f,-1.0f, 0.0f);
		 glColor3f(0.0f, 0.0f,1.0f);
		glVertex3f( 1.0f,-1.0f, 0.0f);			
	glEnd();
}*/

/* function: Draw_Hex() */
/* Draw a hexahedron as a leg of the table */
void Draw_Hex()
{
	glPushMatrix();
	glScalef(1.0f, 1.0f, 3.0f);
	glutSolidCube(0.25);
	glPopMatrix();
}

/* function: Draw_Table()*/
/* Draw a table */
void Draw_Table()
{
	glPushMatrix();
	glScalef(5.0f, 4.0f, 1.0f);
	glutSolidCube(0.25);
	glPopMatrix();			// The top of the table

	glPushMatrix();
	glTranslatef(-1.5 * 0.25, -1 * 0.25, -1.5 * 0.25);	// Use translation operation to find the position of each leg
	Draw_Hex();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5 * 0.25, 1 * 0.25, -1.5 * 0.25);
	Draw_Hex();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5 * 0.25, -1 * 0.25, -1.5 * 0.25);
	Draw_Hex();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5 * 0.25, 1 * 0.25, -1.5 * 0.25);
	Draw_Hex();
	glPopMatrix();
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

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void idle()
{
	glutPostRedisplay();
}

void redraw()
{
// If want display in wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix
	
	glPushMatrix();
	glTranslatef(-2.0f, 0.0f, -6.0f);		// Place the table Left
	glTranslatef(0.0f, fTranslate, 0.0f);	// Translate in Y direction
	Draw_Table();											
	glPopMatrix();

    glPushMatrix();
	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the table at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	Draw_Table();						
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0f, 0.0f, -6.0f);			// Place the table at Right
	glScalef(fScale, fScale, fScale);			// Scale according to the ratio
	Draw_Table();
	glPopMatrix();

	fTranslate += 0.0005f;
	fRotate    += 0.05f;
	fScale	   -= 0.0005f;

	if (fTranslate > 0.5f) fTranslate = 0.0f;
	if (fScale < 0.1f)	   fScale = 1.0f;
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(640,480);                                                  
	glutCreateWindow("Exercise2");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);		
	glutIdleFunc(idle);					

	glutMainLoop();

	return 0;
}


