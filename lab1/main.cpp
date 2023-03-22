// Interaction:
// Press +/- to add/erase random point ---> good practice to add interaction as a comment at the top of source code

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <glm/vec3.hpp>
#include <random>
#include <iostream>

//points: data structure to store current points
std::vector<glm::vec3> points;
// window dimension
float windowWidth = 100.0;
float windowHeight = 100.0;

float generate_random(float window_dim){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(0, window_dim); // define the range
    return distribution(gen);
}
// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(1.0); // Default line width.

    glPointSize(5);

    glBegin(GL_POINTS);
    for(int i=0;i<points.size();++i){
        glVertex3f(points[i].x,points[i].y,0.0);
    }
    glEnd();
    glutPostRedisplay();

    glBegin(GL_LINES);
    for(int j=0;j<points.size();++j){
        glVertex3f(points[j].x,points[j].y,0.0);
    }
    glEnd();
    glutPostRedisplay();

    glFlush();
}

// Initialization routine.
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
}
// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, windowWidth, 0.0, windowHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    glm::vec3 point1;
    switch (key)
    {
        case 27:
            exit(0);
        case '+':
            x=generate_random(windowWidth);
            y=generate_random(windowHeight);
            point1= {glm::vec3(x,y,0) };
            points.push_back(point1);
            drawScene();
            break;
        case '-':
            points.pop_back();
            drawScene();
            break;
        default:
            break;
    }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    std::cout << "Interaction:" << std::endl;
    std::cout << "Press +/- to add/erase random point" << std::endl;
}

// Main routine.
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    printInteraction(); // good practice to print how to interact
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("RandomLines.cpp");
    glutDisplayFunc(drawScene);//drawing function
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);//handle keyboard events

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}