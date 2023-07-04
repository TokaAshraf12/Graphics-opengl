#include <cmath>
#ifdef __SUN__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <string>

#define LEFT 0
#define RIGHT 1

static int width, height; // Size of the OpenGL window.
static double t; // time
static float xVal = 4, zVal = 27 , yVal=0; // Co-ordinates of the spacecraft.
static unsigned int spacecraft; // Display lists base index.
// Planet class that will hold all planets data
class Planet {
public:
    std::string name = "";
    double x = 0;
    double y = 0;
    double c1 = 0;
    double c2 = 0;
    double c3 = 0;
    double speed = 0;
    double orbit_radius = 0;
    double planet_radius = 0;
    double ring_radius = 0;
    double ring_thickness = 0;

    Planet(std::string,double,double,double,double,double,double,double,double);
    void drawPlanets(double t ,double R);
};

Planet::Planet(std::string name,double c1=0,double c2=0,double c3=0,double speed=0,double orbit_radius=0,double planet_radius=0,double ring_radius=0,double ring_thickness=0) {
    this->name = name;
    this->x = 0;
    this->y = 0;
    this->c1 = c1;
    this->c2 = c2;
    this->c3 = c3;
    this->speed = speed;
    this->orbit_radius = orbit_radius;
    this->planet_radius = planet_radius;
    this->ring_radius = ring_radius;
    this->ring_thickness = ring_thickness;
}
void drawSun( double R) {
    // Draw the sun
    glColor3d(3, 3, 0.0);
    glPushMatrix();
    glEnable(GL_LIGHTING);

    if(R == 1 ){
        glTranslated(0, 0, -4);
    }else{
        glTranslated(0, -4, 0);
    }
    glutSolidSphere(1.0, 20, 20);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    GLfloat sun_ambient[] = { 1.0f, 1.0f, 0.0f, 0.0f };
    GLfloat sun_diffuse[] = { 1.0f, 1.0f, 0.0f, 0.0f };
    GLfloat sun_specular[] ={ 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat sun_emissive[] = { 4.0f, 4.0f, 0.0f, 1.0f };
    GLfloat sun_shininess = 100.0f;
    glMaterialfv(GL_BACK, GL_AMBIENT, sun_ambient);
    glMaterialfv(GL_BACK, GL_DIFFUSE, sun_diffuse);
    glMaterialfv(GL_BACK, GL_SPECULAR, sun_specular);
    glMaterialfv(GL_BACK, GL_EMISSION, sun_emissive);
    glMaterialf(GL_BACK, GL_SHININESS, sun_shininess);

    // Set up a light source that illuminates the sun
    GLfloat sun_light_position[] = { 0, 0, 0, 1.0f };
    GLfloat sun_light_color[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    GLfloat sun_light_ambient[] = { 0.2f, 0.2f, 0.0f, 1.0f };
    GLfloat sun_light_specular[] = { 1.0f, 1.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_color);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

    glPopMatrix();
}

void Planet::drawPlanets(double t ,double R) {
    double aspect_ratio = width/height ;
    double theta = t * speed;

    glDisable(GL_LIGHT0);
    // Update planet position
    x = orbit_radius * sin(theta);
    y = orbit_radius * cos(theta) / aspect_ratio;

    // Draw the orbit
    glColor3d(c1, c2, c3);
    glPushMatrix();
    if(R == 1){
        glTranslated(0,0, -4);
    }else{
        glTranslated(0,-4, 0);
    }
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        double theta = i * M_PI / 180.0;
        double x = orbit_radius * sin(theta);
        double y = orbit_radius * cos(theta) / aspect_ratio;
        if(R==1){
            glVertex3d(x, y, 0);
        }else{
            glVertex3d(x, 0, y);
        }
    }
    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHT0);

    // Draw the planet
    glColor3d(c1,c2,c3);
    glPushMatrix();
    if(R==1){
        glTranslated(x, y,-4);
    }else{
        glTranslated(x, -4,y);
    }
    glRotated(-theta * 180.0 / M_PI, 0, 0, 1);
    glutSolidSphere(planet_radius,20,20);
    glPopMatrix();

    // Draw the ring
    glColor3d(c1,c2,c3);
    glPushMatrix();
    if(R==1){
        glTranslated(x, y,-4);
    }else{
        glTranslated(x, -4,y);
    }
    glRotated(60.0, 1.0, 0.0, 0.0);
    glutSolidTorus(ring_thickness, ring_radius, 20, 20);
    glPopMatrix();
}

Planet** planets;

static void drawSolarSystem(double R){
    t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    drawSun(R);

    for(int i = 0; i < 8; i++) {
        (*planets[i]).drawPlanets(t, R);
    }
    // moon orbit calculation around the earth
    double moon_theta = t * 4;
    double moon_x = planets[2]->x;
    double moon_y = planets[2]->y + 0.5 * cos(moon_theta);
    double moon_z = 0.5 * sin(moon_theta);

    // Moon
    glColor3d(0.7, 0.7, 0.7);
    glPushMatrix();
    //in case of right viewport
    if(R ==1){
        glTranslated(moon_x, moon_y, moon_z - 4 );
        glRotated(90, 0, 0, 1);
        glRotated(50.0 * t, 0, 0, 1);
    }else{
        //in case of left view port
        glTranslated(moon_x, moon_z - 4, moon_y);
        glRotated(90, 0, 1, 0);
        glRotated(50.0 * t, 0, 1, 0);
    }
    glutSolidSphere(0.1, 20, 20);
    glPopMatrix();
}

static void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Begin left viewport.
    glEnable(GL_SCISSOR_TEST);
    glScissor(0,0,width,height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, width, height );

    glLoadIdentity();
    gluLookAt(xVal ,yVal ,zVal - 10 ,
              xVal ,yVal ,zVal - 11 ,
              1.0,0.0,0.0);
    glPushMatrix();
    glRotated(90 , 0,0,1);
    drawSolarSystem(LEFT);
    glPopMatrix();

    // Begin right viewport.
    glEnable(GL_SCISSOR_TEST);
    glScissor(width - width / 4.0,0,width / 4.0,height / 4.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    glViewport(width - width / 4.0, 0, width / 4.0, height / 4.0);
    glLoadIdentity();
    
    gluLookAt(0, 0, 19, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    drawSolarSystem(RIGHT);
    // Draw spacecraft.
    glPushMatrix();
    glTranslatef(yVal, xVal, zVal-30 );
    glRotatef(90, 0.0, 1.0, 0.0);
    glCallList(spacecraft);
    glPopMatrix();

    //drawing the separator lines between the both viewports
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(-15, -15, -5.0);
    glVertex3f(-15, 15, -5.0);
    glVertex3f(-15, 12, -5.0);
    glVertex3f(23, 12, -5.0);
    glEnd();

    glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
    spacecraft = glGenLists(1);
    glNewList(spacecraft, GL_COMPILE);
    glPushMatrix();
    glRotatef(180.0, 0.0, 1.0, 0.0); // To make the spacecraft point down the $z$-axis initially.
    glColor3f(1.0, 1.0, 1.0);
    glutWireCone(1.0, 2.0, 5, 5);
    glPopMatrix();
    glEndList();

    const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 50.0f };

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_BACK, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_BACK, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_BACK, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_BACK, GL_SHININESS, high_shininess);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
}


void mouse(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                xVal++;
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
                xVal--;
            break;
        default:
            break;
    }
}
void specialKeyInput(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            yVal--;
            break;
        case GLUT_KEY_RIGHT:
            yVal++;
            break;
        case GLUT_KEY_UP:
            zVal--;
            break;
        case GLUT_KEY_DOWN:
            zVal++;
            break;
    }
    glutPostRedisplay();
}
/* GLUT callback Handlers */
static void resize(int w, int h)
{
    const float ar = (float) w / (float) h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    width=w;
    height=h;
}

static void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    planets = new Planet*[8];
    planets[0]=new Planet("mercury", 0.5, 0.4, 0.4, 1.0, 2.0, 0.18);
    planets[1]=new Planet("venus", 0.6, 0.6, 0.2, 0.5, 3.0, 0.21);
    planets[2]=new Planet("earth", 0.0, 0.0, 0.7, 0.2, 4.0, 0.23);
    planets[3]=new Planet("mars", 1.0, 0.2, 0.0, 0.15, 5.0, 0.18);
    planets[4]=new Planet("jupiter", 0.8, 0.6, 0.3, 0.12, 7.5, 0.53);
    planets[5]=new Planet("saturn", 0.9, 0.8, 0.3, 0.1, 9.0, 0.33, 0.4, 0.02);
    planets[6]=new Planet("uranus", 0.4, 0.4, 1.0, 0.08, 10.0, 0.38);
    planets[7]=new Planet("neptune", 0.0, 0.0, 0.5, 0.05, 11.0, 0.38);

    glutInit(&argc, argv);
    glutInitWindowSize(1100,600);
    glutInitWindowPosition(100,20);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Solar System");

    glutReshapeFunc(resize);
    glutDisplayFunc(drawScene);
    glutSpecialFunc(specialKeyInput);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    setup();

    glutMainLoop();
}
