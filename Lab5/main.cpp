#include <cstdlib>
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Globals.
static unsigned int cylinder; // List index.
static unsigned int circle;

bool show=false;
float rotate_angle=0 , x_dir=0 , y_dir=0 , z_dir=0;


void draw_cylinder(GLfloat radius,GLfloat height){
  if(!show){
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }else{
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
    glBegin(GL_TRIANGLE_FAN);
    const int segments = 25;
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * M_PI * (float)i / (float)segments;
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);

        glVertex3f(x, height / 2.0f, z);
        glVertex3f(x, -height / 2.0f, z);

        glVertex3f(x, height / 2.0f, z);
        glVertex3f(radius * cosf(angle + 2.0f * M_PI / (float)segments), height / 2.0f,
                   radius * sinf(angle + 2.0f * M_PI / (float)segments));

        glVertex3f(x, -height / 2.0f, z);
        glVertex3f(radius * cosf(angle + 2.0f * M_PI / (float)segments), -height / 2.0f,
                   radius * sin(angle + 2.0f * M_PI / (float)segments));
    }
    glEnd();
}
// Initialization routine.
void setup(void)
{
    ///to create list of the legs
    cylinder = glGenLists(1); // Return a list index.
    // Begin create a display list.
    glNewList(cylinder, GL_COMPILE);
    draw_cylinder(2 ,25.0 );
    glEndList();
    // End create a display list.

   ///to create list for the top part
    circle = glGenLists(1); // Return a list index.
    // Begin create a display list.
    glNewList(circle, GL_COMPILE);
    draw_cylinder(15 ,2.0 );
    glEndList();
    // End create a display list.

    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glRotatef(rotate_angle , x_dir , y_dir , z_dir);
   ///draw the right leg
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(10.0, 0.0, -50.0);
    glRotatef(-45.0, 1.0, 0.0, 0.0);
    glCallList(cylinder); // Execute display list.
    glPopMatrix();
   //draw the left leg
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(-5.0, 0.0, -50.0);
    glRotatef(-45.0, 1.0, 0.0, 0.0);
    glCallList(cylinder); // Execute display list.
    glPopMatrix();

   //draw the middle leg
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(2.0, -7.0, -50.0);
    glCallList(cylinder); // Execute display list.
    glPopMatrix();

    //draw the top part of the chair
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(3.0, 10.0, -70.0);
    glRotatef(30.0, 1.0, 0.0, 0.0);
    glCallList(circle); // Execute display list.
    glPopMatrix();

    glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
        case 32:
            show=!show;
            rotate_angle=0 , x_dir=0 , y_dir=0 , z_dir=0;
            setup();
            glutPostRedisplay();
            break;
        case 'x':
            if(rotate_angle<=0){
                rotate_angle-=1;
            }else{
                rotate_angle=0;
            }
            x_dir=1;y_dir=0;z_dir=0;
            glutPostRedisplay();
            break;
        case 'X':
            if(rotate_angle>=0){
                rotate_angle+=1;
            }else{
                rotate_angle=0;
            }
            x_dir=1;y_dir=0;z_dir=0;
            glutPostRedisplay();
            break;
        case 'y':
            if(rotate_angle<=0){
                rotate_angle-=1;
            }else{
                rotate_angle=0;
            }
            y_dir=1;x_dir=0;z_dir=0;
            glutPostRedisplay();
            break;
        case 'Y':
            if(rotate_angle>=0){
                rotate_angle+=1;
            }else{
                rotate_angle=0;
            }
            y_dir=1; x_dir=0; z_dir=0;
            glutPostRedisplay();
            break;
        case 'z':
            if(rotate_angle<=0){
                rotate_angle-=1;
            }else{
                rotate_angle=0;
            }
            z_dir=1;y_dir=0;x_dir=0;
            glutPostRedisplay();
            break;
        case 'Z':
            if(rotate_angle>=0){
                rotate_angle+=1;
            }else{
                rotate_angle=0;
            }
            z_dir=1;y_dir=0;x_dir=0;
            glutPostRedisplay();
            break;
        default:
            rotate_angle=0 , x_dir=0 , y_dir=0 , z_dir=0;
            glutPostRedisplay();
            break;
    }
}

// Main routine.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Chair.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}