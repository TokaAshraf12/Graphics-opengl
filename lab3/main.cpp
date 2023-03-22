#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <random>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Globals.
static float R_sphere = 5.0; // Radius of hemisphere.
static int p = 6; // Number of longitudinal slices.
static int q = 4; // Number of latitudinal slices.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.
static float offset = -10;
static float spinSpeed = 5;
static float prev_time = 0;
int userChoice = 0;
enum choice {Helix=1, Sphere=2};
static float Xc=0.0;
static float Yc=0.0;
static float pitch=0.3;
static int no_of_turns=5;
static float R_helix=1.5;
static float dt=0.1;
static int mode=GL_TRIANGLE_STRIP;
static int mode1=GL_LINE;
// Initialization routine.
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
}


void drawSphere(){
    glPolygonMode(GL_FRONT_AND_BACK, mode1);
    int  i, j;
    /////first half of the sphere

    for (j = 0; j < q; j++)
    {
        // One latitudinal triangle strip.
        glBegin(mode);
        for (i = 0; i <= p; i++)
        {
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(R_sphere * cos((float)(j + 1) / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
                       R_sphere * sin((float)(j + 1) / q * M_PI / 2.0),
                       -R_sphere * cos((float)(j + 1) / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));
            glColor3f(0.0, 50.0, 0.0);
            glVertex3f(R_sphere * cos((float)j / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
                       R_sphere * sin((float)j / q * M_PI / 2.0),
                       -R_sphere * cos((float)j / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));
        }
        glEnd();
    }
///second half of the sphere
    for (j = 0; j < q; j++)
    {
        // One latitudinal triangle strip.
        glBegin(mode);
        for (i = 0; i <= p; i++)
        {
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(R_sphere * cos((float)(j + 1) / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
                       -R_sphere * sin((float)(j + 1) / q * M_PI / 2.0),
                       -R_sphere * cos((float)(j + 1) / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));
            glColor3f(0.0, 1.0, 1.0);
            glVertex3f(R_sphere * cos((float)j / q * M_PI / 2.0) * cos(2.0 * (float)i / p * M_PI),
                       -R_sphere * sin((float)j / q * M_PI / 2.0),
                       -R_sphere * cos((float)j / q * M_PI / 2.0) * sin(2.0 * (float)i / p * M_PI));
        }
        glEnd();
    }
    glFlush();
}
// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(0.0, 0.0, 0.0);
    // Command to push the sphere, which is drawn centered at the origin,
    // into the viewing frustum.
    glTranslatef(0.0, 0.0, offset);
    // Commands to turn the sphere.
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);
    switch(userChoice){
        case Helix:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBegin(GL_LINE_STRIP);
            for(float t = -no_of_turns * M_PI; t <= no_of_turns * M_PI; t += dt){
                glColor3ub(rand()%255, rand()%255, rand()%255);
                glVertex3f(R_helix * cos(t), R_helix * sin(t), t * pitch);
            }
            glEnd();
            glFlush();
            break;
        case Sphere:
            drawSphere();
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
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void spinDisplay() {
    Yangle += spinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prev_time) / 1000;
    prev_time = glutGet(GLUT_ELAPSED_TIME); // to indicate refresh rate
    if (Yangle > 360.0) Yangle -= 360.0;
    glutPostRedisplay();
}
void spinDisplayReverse() {
    Yangle -= spinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prev_time) / 1000;
    prev_time = glutGet(GLUT_ELAPSED_TIME); // to indicate refresh rate
    std::cout << Yangle << std::endl;
    if (Yangle > 360.0) Yangle -= 360.0;
    glutPostRedisplay();
}
void mouse(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(spinDisplay);
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(spinDisplayReverse);
            break;
        default:
            break;
    }
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
        case 'P':
            p += 1;
            glutPostRedisplay();
            break;
        case 'p':
            if (p > 3) p -= 1;
            glutPostRedisplay();
            break;
        case 'Q':
            q += 1;
            glutPostRedisplay();
            break;
        case 'q':
            if (q > 3) q -= 1;
            glutPostRedisplay();
            break;
        case 'W':
            mode=GL_LINE_LOOP;
            mode1=GL_LINE;
            glutPostRedisplay();

            break;
        case 'w':
            mode1=GL_FILL;
            mode=GL_TRIANGLE_STRIP;
            glutPostRedisplay();
            break;
        case 'R':
            R_helix+=1;
            glutPostRedisplay();
            break;
        case 'r':
            R_helix-=1;
            glutPostRedisplay();
            break;
        case 'H':
            pitch+=0.1;
            glutPostRedisplay();
            break;
        case 'h':
            pitch-=0.1;
            glutPostRedisplay();
            break;
        case 'N':
            if(dt >= 0.1){
                dt-=0.1;
            }
            glutPostRedisplay();
            break;
        case 'n':
            dt+=0.1;
            glutPostRedisplay();
            break;
        case 'x':
            Xangle += 5.0;
            if (Xangle > 360.0) Xangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'X':
            Xangle -= 5.0;
            if (Xangle < 0.0) Xangle += 360.0;
            glutPostRedisplay();
            break;
        case 'y':
            Yangle += 5.0;
            if (Yangle > 360.0) Yangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Y':
            Yangle -= 5.0;
            if (Yangle < 0.0) Yangle += 360.0;
            glutPostRedisplay();
            break;
        case 'z':
            Zangle += 5.0;
            if (Zangle > 360.0) Zangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Z':
            Zangle -= 5.0;
            if (Zangle < 0.0) Zangle += 360.0;
            glutPostRedisplay();
            break;
        case 'O':
            offset += 1;
            glutPostRedisplay();
            break;
        case 'o':
            offset -= 1;
            glutPostRedisplay();
            break;
        case ' ':
            glutIdleFunc(NULL);
            break;
        default:
            break;
    }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    std::cout << "Interaction:" << std::endl;
    std::cout << "Press P/p to increase/decrease the number of longitudinal slices." << std::endl
              << "Press Q/q to increase/decrease the number of latitudinal slices." << std::endl
              << " press W/w to draw sphere in wireframe / draw filled sphere." << std::endl;
    std::cout << "Press R/r to increase/decrease radius of the helix" << std::endl
              << "Press H/h to increase/decrease pitch of helix" << std::endl
              << "Press N/n to increase/decrease number of vertices used to draw the helix." << std::endl;
}

// Main routine.
int main(int argc, char** argv)
{
    printInteraction();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    std::cout<< "Enter 1 for drawing Helix or Enter 2 for drawing Sphere: ";
    std::cin >> userChoice;
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("helix and sphere.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutMouseFunc(mouse);
    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}