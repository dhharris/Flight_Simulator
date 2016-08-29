/**
 * MP2
 * Written by David Harris (dhharri2)
 * Compile with Make
 */

/* C library includes */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* OpenGL library includes for Mac*/
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

/* OpenGL library includes for Linux */
#ifdef __linux__
#include <GL/gl.h>
#include <GL/glut.h>
#endif

/* Local includes */
#include "vector.h"
#include "quaternion.h"
#include "camera.h"

/* Constants */
#define ANGLE 30

/* Global variables */
float water_level;
float scale;
Camera cam;
double camera_dir[3] = {1.0, 0.0, 0.0};
double camera_up[3]= {0.0, 0.0, 1.0};
int paused = 0;

/**
 * Change camera as time goes on
 */
void updateVectors() {

    if (!paused) {
        /* Tick forward in time */
        cam.tick(0.05);

        /* Camera Direction */
        Vector v = cam.rotation * Vector(1.0,0.0,0.0);
        camera_dir[0] = v.x;
        camera_dir[1] = v.z;
        camera_dir[2] = v.y;

        /* Camera roll */
        v = cam.rotation * Vector(0.0,1.0,0.0);
        camera_up[0] = v.x;
        camera_up[1] = v.z;
        camera_up[2] = v.y;
    }
}

/**
 * Seed RNG for mountain
 */
int seed(float x, float y) {
    static int a = 1588635695, b = 1117695901;
    int xi = *(int *)&x;
    int yi = *(int *)&y;
    return ((xi * a) % b) - ((yi * b) % a);
}

/**
 * Draw the mountain
 */
void mountain(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float s) {
    float x01,y01,z01,x12,y12,z12,x20,y20,z20;

    glShadeModel(GL_SMOOTH);
    if (s < scale) {
        x01 = x1 - x0;
        y01 = y1 - y0;
        z01 = z1 - z0;

        x12 = x2 - x1;
        y12 = y2 - y1;
        z12 = z2 - z1;

        x20 = x0 - x2;
        y20 = y0 - y2;
        z20 = z0 - z2;

        float nx = y01*(-z20) - (-y20)*z01;
        float ny = z01*(-x20) - (-z20)*x01;
        float nz = x01*(-y20) - (-x20)*y01;

        float den = sqrt(nx*nx + ny*ny + nz*nz);

        if (den > 0.0) {
            nx /= den;
            ny /= den;
            nz /= den;
        }

        glNormal3f(nx,ny,nz);
        glBegin(GL_TRIANGLES);
        glVertex3f(x0,y0,z0);
        glVertex3f(x1,y1,z1);
        glVertex3f(x2,y2,z2);
        glEnd();

        return;
    }

    x01 = 0.5*(x0 + x1);
    y01 = 0.5*(y0 + y1);
    z01 = 0.5*(z0 + z1);

    x12 = 0.5*(x1 + x2);
    y12 = 0.5*(y1 + y2);
    z12 = 0.5*(z1 + z2);

    x20 = 0.5*(x2 + x0);
    y20 = 0.5*(y2 + y0);
    z20 = 0.5*(z2 + z0);

    s *= 0.5;

    srand(seed(x01,y01));
    z01 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);
    srand(seed(x12,y12));
    z12 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);
    srand(seed(x20,y20));
    z20 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);

    mountain(x0,y0,z0,x01,y01,z01,x20,y20,z20,s);
    mountain(x1,y1,z1,x12,y12,z12,x01,y01,z01,s);
    mountain(x2,y2,z2,x20,y20,z20,x12,y12,z12,s);
    mountain(x01,y01,z01,x12,y12,z12,x20,y20,z20,s);
}

/**
 * Initialize everything
 */
void init()  {

    glClearColor (0.5, 0.5, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    gluLookAt(0.0,0.0,0.0,
            1.0,0.0,0.0,
            0.0,0.0,1.0);
    water_level = 0.0;
    scale = 0.01;
}

/**
 * Lighting function
 */
void lights() {
    GLfloat white[] = {1.0,1.0,1.0,1.0};
    GLfloat lpos[] = {2.0,2.0,2.0,0.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
}

/**
 * Display everything to the screen 
 */
void display() {
    glLoadIdentity ();
    updateVectors();
    gluLookAt (cam.pos.x,cam.pos.z,cam.pos.y,
            cam.pos.x + camera_dir[0],
            cam.pos.z + camera_dir[1],
            cam.pos.y + camera_dir[2],
            camera_up [0],camera_up [1],camera_up [2]);
    lights();

    GLfloat tanamb[] = {0.2,0.15,0.1,1.0};
    GLfloat tandiff[] = {0.4,0.3,0.2,1.0};

    GLfloat seaamb[] = {0.0,0.0,0.2,1.0};
    GLfloat seadiff[] = {0.0,0.0,0.8,1.0};
    GLfloat seaspec[] = {0.5,0.5,1.0,1.0};


    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);
    glTranslatef (-0.5, -0.5, 0.0); 
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tanamb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tandiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tandiff);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

    mountain(0.0,0.0,0.0, 1.0,0.0,0.0, 0.0,1.0,0.0, 1.0);
    mountain(1.0,1.0,0.0, 0.0,1.0,0.0, 1.0,0.0,0.0, 1.0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, seaamb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, seadiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, seaspec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_QUADS);
    glVertex3f(0.0,0.0,water_level);
    glVertex3f(1.0,0.0,water_level);
    glVertex3f(1.0,1.0,water_level);
    glVertex3f(0.0,1.0,water_level);
    glEnd();

    glutSwapBuffers();
    glFlush ();


    glutPostRedisplay();
}

/**
 * Reshape function
 */
void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(90.0,(double)w / (double)h,0.0001,10.0);
    glMatrixMode (GL_MODELVIEW);
}

/**
 * Arrow key handler
 */
void arrowkeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            /* Roll left */
            cam.roll(ANGLE);
            break;
        case GLUT_KEY_RIGHT:
            /* Roll right */
            cam.roll(-ANGLE);
            break;
        case GLUT_KEY_UP:
            /* Pitch "up" */
            cam.pitch(-ANGLE);
            break;
        case GLUT_KEY_DOWN:
            /* Pitch "down" */
            cam.pitch(ANGLE);
            break;
    }
}

/**
 * Keyboard handler
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '-':
            /* Decrease sea level */
            water_level -= 0.01;
            break;
        case '=':
            /* Increase sea level */
            water_level += 0.01;
            break;
        case 'f':
            /* Increase scale */
            scale *= 0.5;
            break;
        case 'c':
            /* Decrease scale */
            scale *= 2.0;
            break;
        case 'e':
            /* Move down */
            cam.up(-0.005);
            break;
        case 'r':
            /* Move up */
            cam.up(0.005);
            break;
        case 'w':
            /* Speed up */
            cam.speed += 0.005;
            break;
        case 's':
            /* Slow down */
            cam.speed -= 0.005;
            break;
        case 'a':
            /* Yaw left */
            cam.yaw(-ANGLE);
            break;
        case 'd':
            /* Yaw right */
            cam.yaw(ANGLE);
            break;
        case 'p':
            paused = !paused;
            break;
        case 'q':
            exit(0);
            break;
        default:
            break;  // Do nothing
    }
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500); 
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrowkeys);
    glutMainLoop();
    return 0;
}



