/* write user interaction here for good practice */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
enum choice {ortho=1, prespective=2};
enum userInput {zoomIn='i', zoomOut='o', stopSpinning=' ', CCW=GLUT_LEFT_BUTTON, CW= GLUT_RIGHT_BUTTON};
int userChoice = 0;
float orthoLeft = -50;
float orthoRight = 50;
float orthoBottom = -50;
float orthoTop = 50;
float orthoNear = -5;
float orthoFar = 5;
float fruLeft = -5;
float fruRight = 5;
float fruBottom = -5;
float fruTop = 5;
float fruNear = 5;
float fruFar = 100;
float offsetX = 100;
float offsetY = 100;
float zOffset = -15;//zoom out/in
float windowWidth = 500;
float windowHeight = 500;
float spinY = 0;
float spinZ = 0;
float spinSpeed = 5;
float currentSpin = 0;
float prevTime = 0;
// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(0.0, 0.0, 0.0);
	glLineWidth(1.0); // Default line width.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	switch (userChoice) {
	case ortho:
            glRotatef(spinZ, 0, 0, 1);
            glBegin(GL_TRIANGLES);
            glVertex3f(0, 10, 0);
            glVertex3f(-30, 0, 0);
            glVertex3f(30, 0, 0);
            glEnd();

		break;
	case prespective:
            glRotatef(spinY, 0, 1, 0);
            glTranslatef( -5, 0, zOffset*2);
            glBegin(GL_TRIANGLES);
            glVertex3f( 0, 5, 0);
            glVertex3f(-5, -5, 5);
            glVertex3f(5, -5, 5);

            glVertex3f(0, 5, 0);
            glVertex3f(5, -5, 5);
            glVertex3f(5, -5, -5);

            glVertex3f(0, 5, 0);
            glVertex3f(5, -5, -5);
            glVertex3f(-5, -5, -5);

            glVertex3f( 0, 5, 0);
            glVertex3f(-5,-5,-5);
            glVertex3f(-5,-5, 5);
            glEnd();
		break;
	default:
		break;
	}
	glFlush();
}
// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}
//spin logic
void spinDisplay(void)
{
    glMatrixMode(GL_PROJECTION);
    currentSpin += spinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 100;
    prevTime = glutGet(GLUT_ELAPSED_TIME);
    switch (userChoice) {
        case ortho :
            spinZ=currentSpin;
            break;
        case prespective:
            spinY=currentSpin;
            break;
        default:
            break;

    }
    glMatrixMode(GL_MODELVIEW);
    drawScene();
    glutPostRedisplay();
	/*
		write code below:
		1- change currentSpin according to spinSpeed (note: spinSpeed unit is dgree/second)
		2- mark window to be rerendered (hint: glutPostRedisplay, prevTime)
	*/
}

void spinDisplayReverse(void)
{
    glMatrixMode(GL_PROJECTION);
    currentSpin -= spinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 100;
    prevTime = glutGet(GLUT_ELAPSED_TIME);
    switch (userChoice) {
        case ortho :
            spinZ=currentSpin;
            break;
        case prespective:
            spinY=currentSpin;
            break;
        default:
            break;
    }
    glMatrixMode(GL_MODELVIEW);
    drawScene();
    glutPostRedisplay();
	/*
		write code below:
		1- change currentSpin according to spinSpeed (note: spinSpeed unit is dgree/second)
		2- mark window to be rerendered
	*/
}

//keyboard & mouse
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case CCW:
       glutIdleFunc(spinDisplay);
		/*
		write code below:
			1- assign spin logic to be invoked regularly (hint: glutIdleFunc)
		*/
		break;
	case CW:
        glutIdleFunc(spinDisplayReverse);
		/*
		write code below:
			1- assign reverse spin logic to be invoked regularly
		*/
		break;
	default:
		break;
	}
}
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
	case zoomIn:
            zOffset+=0.1;
            drawScene();
            glutPostRedisplay();
		/*
		write code below:
			1- zoom in
			2- mark window for rerendering
		*/
		break;
	case zoomOut:
          zOffset-=0.1;
            drawScene();
            glutPostRedisplay();
		/*
		write code below:
			1- zoom out
			2- mark window for rerendering
		*/
		break;
	case stopSpinning:
        glutIdleFunc(NULL);
		/*
		write code below:
			1- stop spinning (hint: use NULL)
		*/
		break;
	default:
		break;
	}
}
// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (userChoice) {
	case ortho:
        glOrtho(orthoLeft , orthoRight, orthoBottom , orthoTop , orthoNear , orthoFar);
		/*
		write code below:
			1- initiate viewing box for parallel projection(use ortho variables (orthoLeft, orthoRight, ...))
		*/

		break;
	case prespective:
        glFrustum(fruLeft , fruRight , fruBottom , fruTop , fruNear ,fruFar);
		/*
		write code below:
			1- initiate frustum for perspective projection (use fru variables (fruLeft, fruRight, ...))
		*/
		break;
	default:
		break;
	}

	glMatrixMode(GL_MODELVIEW);

}
// user interaction
void printUserInteraction() {
    std::cout << "click left mouse to spin around specific axis in counter-clockwise(CCW) \n"
                 "click right mouse to spin around specific axis in clockwise(CCW) \n"
                 "press space button to stop spinning \n"
                 "press i button to zoom in \n"
                 "press o button to zoom out\n";
    /*write code below:
     1- print user interaction(good practice)
    */
}


// Main routine.
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	printUserInteraction();
	std::cout << "Which projection type do you want\n1) parallel projection\n2) perspective projection\n>> ";

	/*
	write code below:
		1- accept input from user and assign the value to userChoice variable
	*/
    // code here
    std::cin >> userChoice;
    std::cout << "choice is ";
    std::cout << userChoice;
	//---------------------------------------------------------------------
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(offsetX, offsetY);
	glutCreateWindow("Lab 2.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouse);
	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
//Note: At first user interaction, if scene spins unexpectadly you need to handle this behaviour (Hint: use prevTime variable)