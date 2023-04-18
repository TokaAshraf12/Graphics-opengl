#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

#define VERTICES 0
#define INDICES 1
#define T 0
#define O 1
#define K 2
#define A 3



// Begin globals.
// Vertex co-ordinate vectors for the (T) letter.
static float vertices1[] =
        {
                15.0, 30.0, 0.0,
                15.0, 70.0, 0.0,
                0.0, 70.0, 0.0,
                30.0, 70.0, 0.0
        };
// Vertex co-ordinate vectors for the (O) letter.
static float vertices2[] =
        {
                20.0, 30.0, 0.0,
                20.0, 50.0, 0.0,
                40.0, 50.0, 0.0,
                40.0, 30.0, 0.0,
                20.0, 30.0, 0.0
        };
// Vertex co-ordinate vectors for the (K) letter.
static float vertices3[] =
        {
                45.0, 30.0, 0.0,
                45.0, 70.0, 0.0,
                45.0, 50.0, 0.0,
                55.0, 70.0, 0.0,
                45.0, 50.0, 0.0,
                55.0, 30.0, 0.0
        };
// Vertex co-ordinate vectors for the (K) letter.
static float vertices4[] =
        {
                60.0, 30.0, 0.0,
                70.0, 70.0, 0.0,
                70.0, 70.0, 0.0,
                80.0, 30.0, 0.0,
                75.0, 50.0, 0.0,
                65.0, 50.0, 0.0
        };

static unsigned int vao[4]; // Array of VAO ids.
static std::vector<float> points;

// End globals.


// Function to draw a line using DDA algorithm
std::vector<float> drawLineDDA(float* vert , int size) {
    std::vector<float> vertices;
    for(int count=0;count< size/3-1 ;count++){
        float x1=vert[3*count];
        float x2=vert[3*(count+1)];
        float y1=vert[(3*count)+1];
        float y2=vert[3*(count+1)+1];
        float dx = x2 - x1;
        float dy = y2 - y1;
        float length = abs(dx) > abs(dy) ? abs(dx) : abs(dy); // choose the longer distance as the length
        float xinc = dx / length;
        float yinc = dy / length;
        float x = x1, y = y1;
        for (int i = 0; i < length; i++) {
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0);

            x += xinc;
            y += yinc;
        }
        vertices.push_back(x2);
        vertices.push_back(y2);
        vertices.push_back(0.0);
    }
    return vertices;
}
std::vector<float> drawLineBresenham(float* vert , int size) {
    std::vector<float> vertices;
    for(int count=0;count< size/3 -1 ;count++) {
        float x1 = vert[3 * count];
        float x2 = vert[3 * (count + 1)];
        float y1 = vert[(3 * count) + 1];
        float y2 = vert[3 * (count + 1) + 1];
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;
        while (x1 != x2 || y1 != y2) {
            vertices.push_back(x1);
            vertices.push_back(y1);
            vertices.push_back(0.0);
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
        vertices.push_back(x2); //endpoint
        vertices.push_back(y2);
        vertices.push_back(0.0);
    }
    return vertices;
}

// Timer function.
void animate(int someValue)
{
    glutPostRedisplay();
    glutTimerFunc(500, animate, 1);
}
// Function to create a vertex array object (VAO) for a line
GLuint createLineVAO(std::vector<float> vertices) {
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0,  0);
    glColorPointer(3, GL_FLOAT, 0, 0);



    //glBindVertexArray(0); // unbind the VAO
    return VAO;
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    // Get a pointer to the vertex buffer.
    float* bufferData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    //std::cout << points.size();

    // Randomly change the color values.
    for (int i = 0; i < points.size() / sizeof(float); i++){
        bufferData[(points.size() / sizeof(float)) + (3*i)] = (float)rand() / (float)RAND_MAX;
    }

    // Release the vertex buffer.
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindVertexArray(vao[T]); // bind the VAO for the horizontal line of T
    glDrawArrays(GL_POINTS, 0, points.size()/2); // draw the line
    glBindVertexArray(vao[O]);
    glDrawArrays(GL_POINTS, 0, points.size()/2);

    glBindVertexArray(vao[K]);
    glDrawArrays(GL_POINTS, 0, points.size()/2);
    glBindVertexArray(vao[A]);
    glDrawArrays(GL_POINTS, 0, points.size()/2);

    glutSwapBuffers();


    glFlush();
}

// Initialization routine.
void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
    glPointSize(5.0);

    points= drawLineDDA(vertices1,sizeof(vertices1)/sizeof(float));
    vao[T] = createLineVAO(points); // create a VAO for  T

    points =drawLineDDA(vertices2,sizeof(vertices2)/sizeof(float));
   vao[O] = createLineVAO(points); // create a VAO for  O

    points=drawLineBresenham(vertices3,sizeof(vertices3)/sizeof(float));
    vao[K]= createLineVAO(points);

    points=drawLineBresenham(vertices4,sizeof(vertices4)/sizeof(float));
    vao[A]= createLineVAO(points);

   glutTimerFunc(5, animate, 1);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
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
	default:
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
	glutCreateWindow("FirstNameUsingVAOs.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}