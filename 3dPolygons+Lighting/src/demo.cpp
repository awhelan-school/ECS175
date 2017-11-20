/*
 * Simple glut demo that can be used as a template for
 * other projects by Garrett Aldrich
 */


#ifdef WIN32
#include <windows.h>
#endif

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else //linux
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#endif

//other includes
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <exception>

#include "fileIO.h"
#include "Matrix.h"
#include "transformations.h"

#define PI 3.14159265
GLfloat angle = 1;

/****set in main()****/
//the number of pixels in the grid
float grid_width;
float grid_height;

//the size of pixels sets the initial window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
float pixel_size;

/*Window information*/
int win_height;
int win_width;

int options_width;

std::vector<Object> Objects;
static Light lfx;

Point RX0, RX1;

GLfloat xv = 0.0, yv = 0.0, zv = -1.0;
GLfloat tx = 0.0, ty = 0.0, tz = 0.0;
GLfloat scale = 1.0;
float Sf = 1.0;

int VMode = 0, TMode = 0, SMode = 0, RMode = 0;
int CObject = 0;
bool Gselect;
bool RotAxis = false;
int NMode = 0;

void rotateView(char id, int &Mode, float &rval);
void drawAxis();
void drawRotAxis();
void drawSegment(Point p0, Point p1, Point p2);
void displayOptions();
void output(GLfloat x, GLfloat y, char *text);

void init();
void idle();
void display();
void draw_lines();
void reshape(int width, int height);
void SpecialInput(int key, int x, int y);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();

int main(int argc, char **argv)
{

    readFile(Objects, lfx);

    options_width = 16;
    //the number of pixels in the grid
    grid_width = 1000;
    grid_height = 1000;

    //the size of pixels sets the inital window height and width
    //don't make the pixels too large or the screen size will be larger than
    //your display size


    pixel_size = 1;

    /*Window information*/
    win_height = grid_height * pixel_size;
    win_width = grid_width * pixel_size;



    /*Set up glut functions*/
    /** See https://www.opengl.org/resources/libraries/glut/spec3/spec3.html ***/

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    /*initialize variables, allocate memory, create buffers, etc. */
    //create window of size (win_width x win_height
    glutInitWindowSize(win_width, win_height);
    //windown title is "glut demo"
    glutCreateWindow("glut demo");


    glutSpecialFunc(SpecialInput);
    /*defined glut callback functions*/
    glutDisplayFunc(display); //rendering calls here
    glutReshapeFunc(reshape); //update GL on window size change
    glutMouseFunc(mouse); //mouse button events
    glutMotionFunc(motion); //mouse movement events
    glutKeyboardFunc(key); //Keyboard events
    glutIdleFunc(idle); //Function called while program is sitting "idle"

    //initialize opengl variables
    init();
    //start glut event loop
    glutMainLoop();
    return 0;
}

/*initialize gl stufff*/
void init()
{
    //set clear color (Default background to white)
    glClearColor(0.0, 0.0, 0.0, 0.0);
    //checks for OpenGL errors
    check();
}

//called repeatedly when glut isn't doing anything else

void idle()
{

    for (int i = 0; i < Objects.size(); i++) {
        for (int k = 0; k < Objects[i].TList.size(); k++){
            ;//calculateNormalV(Objects[i]);
            ;//calculateIntensity(Objects[i], lfx);
            
        }
            
    }

    //redraw the scene over and over again
    glutPostRedisplay();
}

//this is where we render the screen

void display()
{
    //clears the screen
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
    glLoadIdentity();

     /*****DRAW GRID ON SCREEN*******/
    //creates a rendering area across the window
    glViewport(0,0,win_width,win_height);
    // up an orthogonal projection matrix so that
    // the pixel space is mapped to the grid space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //map gl ortho so that each pixel is on an integer boundary
    //we will handle the screen space projection ourselves
    glOrtho(0,win_width,0,win_height,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //draw grid
    glBegin(GL_LINES);
        glColor3f(.4,.4,.8);
        glVertex3f(win_width/2,0,0);
        glVertex3f(win_width/2,win_height,0);
        glVertex3f(0,win_height/2,0);
        glVertex3f(win_width,win_height/2,0);
    glEnd();

    //displayOptions();
    int m;
    int k = 0;
    int n = 0;
    
        for (m = 0; m < Objects.size(); m++) {
            calculateIntensity(Objects[m], lfx);   
        }
    

    for (unsigned int i = 0; i < 4; i++) {


        if (i == 0) {
            //glLoadIdentity();
            glViewport(0, win_height / 2, win_width / 2, win_height / 2);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho((scale*-1), scale, (scale*-1), scale, (scale*-1), scale);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0.5, 0.5, 0.5, xv, yv, zv, 0.0, 1.0, 0.0);

            draw_lines();

            for (k = 0; k < Objects.size(); k++) {
                for (n = 0; n < Objects[k].TList.size(); n++) {
                    //fill_triangles(Objects[k].Ip0, Objects[k], Objects[k].TList[n], 0);
                }
            }//Raster_Triangles


        }
        if (i == 1) {    
            //Code Adapted From NEHE tutorial on Multiple Viewports
            // Set The Viewport To The Top Right.  
            //glLoadIdentity();
            glViewport((win_width / 2) + 1, (win_height / 2) + 1, (win_width / 2) - 1, (win_height / 2) - 1);
            glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
            glLoadIdentity(); // Reset The Projection Matrix
            // Set Up Ortho Mode To Fit 1/4 The Screen (Size Of A Viewport)
            //glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
          
            glOrtho(0,(grid_width/scale),0,(grid_height/scale),0,1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            //gluLookAt(0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0);

            //draw_lines();

            for (k = 0; k < Objects.size(); k++) {
                std::vector<touple_t> sortedList;
                sortX(Objects[k].TList, sortedList);
                for (n = 0; n < Objects[k].TList.size(); n++) {
                    fill_triangles(Objects[k].Ip1, Objects[k], sortedList[n], 1, NMode, grid_width, grid_height);
                }
            }//Raster_Triangles
            
        }//YZ Projection
        if (i == 2) {
      
            //glLoadIdentity();
            glViewport((win_width / 2) + 1, 1, (win_width / 2) - 1, (win_height / 2) - 1);   
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            //glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
            glOrtho(0,(grid_width/scale),0,(grid_height/scale),0,1);
 
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            //gluLookAt(0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0, 0.0);
            //draw_lines();

            for (k = 0; k < Objects.size(); k++) {
                std::vector<touple_t> sortedList;
                sortY(Objects[k].TList, sortedList);
                for (n = 0; n < Objects[k].TList.size(); n++) {
                    fill_triangles(Objects[k].Ip2, Objects[k], sortedList[n], 2, NMode, grid_width, grid_height);
                }
            }//Raster_Triangles
            
        }//XZ Projection
        if (i == 3) {
            
            //glLoadIdentity();
            //glViewport(0,win_height/2+1,win_width/2-1,win_height/2-1);
            glViewport(0,0,win_width/2-1,win_height/2-1);     
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();       
            //glOrtho(0.0, 1.0, 0.0, 1.0, (scale *-1.0), (scale * 1.0) );
            //glOrtho(0, scale, 0, scale, 0, scale);
            glOrtho(-1.0,(grid_width/scale),-1.0,(grid_height/scale),-1.0,1);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
                    
            //draw_lines();

            for (k = 0; k < Objects.size(); k++) {
                
                std::vector<touple_t> sortedList;
                sortZ(Objects[k].TList, sortedList);
                for (n = 0; n < Objects[k].TList.size(); n++) {
                    fill_triangles(Objects[k].Ip3, Objects[k], sortedList[n], 3, NMode, grid_width, grid_height);
                }
            }//Raster_Triangles
            
        }//XY Projection
    }


    //TODO ADD COMMENTS TO OUPUT
    //writeFile(Objects);
    //blitz the current opengl framebuffer on the screen
    glutSwapBuffers();
    //checks for opengl errors
    check();
}

void draw_lines()
{

    drawAxis();

    if (RotAxis == true)
        drawRotAxis();

    Point p1, p2, p3;

    //tells openGL to interpret every two calls to glVertex as a line

    for (int k = 0; k < Objects.size(); k++)
        for (unsigned int i = 0; i < Objects[k].TList.size(); i++) {


            p1 = Objects[k].TList[i].p1;
            p2 = Objects[k].TList[i].p2;
            p3 = Objects[k].TList[i].p3;

            if (k == CObject)
                Gselect = true;
            else
                Gselect = false;

            drawSegment(p1, p2, p3);
        }

    if(NMode)
    for (int m = 0; m < Objects.size(); m++) {
        for (int n = 0; n < Objects[m].VList.size(); n++) {

            Point p = Objects[m].VList[n];
            Vector f(p.xyz[0][0], p.xyz[1][0], p.xyz[2][0]);
            Vector t = Objects[m].NormVecList[n];

            glLineWidth(2.0); //sets the "width" of each line we are rendering
            glBegin(GL_LINES);
            //Draw Rotation Lines
            glColor3f(1.0, 0.07, 0.57);
            glVertex3f(f.x, f.y, f.z);
            glVertex3f(f.x + t.x, f.y + t.y, f.x + t.z);
            glEnd();
        }
    }//DrawNormals

}

void drawRotAxis()
{
    glLineWidth(3.0); //sets the "width" of each line we are rendering
    glBegin(GL_LINES);
    //Draw Rotation Lines
    glColor3f(1.0, 0.07, 0.57);
    glVertex3f(RX0.xyz[0][0], RX0.xyz[1][0], RX0.xyz[2][0]);
    glVertex3f(RX1.xyz[0][0], RX1.xyz[1][0], RX1.xyz[2][0]);
    glEnd();

}

void drawSegment(Point p0, Point p1, Point p2)
{
    glLineWidth(1.0); //sets the "width" of each line we are rendering
    glBegin(GL_LINES);
    //Draw Black Lines
    if (Gselect)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3f(1.0, 1.0, 1.0);
    glVertex3f(p0.xyz[0][0], p0.xyz[1][0], p0.xyz[2][0]);
    glVertex3f(p1.xyz[0][0], p1.xyz[1][0], p1.xyz[2][0]);

    glVertex3f(p1.xyz[0][0], p1.xyz[1][0], p1.xyz[2][0]);
    glVertex3f(p2.xyz[0][0], p2.xyz[1][0], p2.xyz[2][0]);

    glVertex3f(p2.xyz[0][0], p2.xyz[1][0], p2.xyz[2][0]);
    glVertex3f(p0.xyz[0][0], p0.xyz[1][0], p0.xyz[2][0]);
    glEnd();

}

/*Gets called when display size changes, including initial craetion of the display*/

/*this needs to be fixed so that the aspect ratio of the screen is consistant with the orthographic projection*/

void reshape(int width, int height)
{


    /*set up projection matrix to define the view port*/
    //update the ne window width and height
    win_width = width;
    win_height = height;
    
    grid_width = width/2;
    grid_height = height/2;

    //creates a rendering area across the window
    //glViewport(0, 0, width, height);

    // uses an orthogonal projection matrix so that
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();

    //you guys should look up this function
    //glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0);

    //clear the modelview matrix
    //the ModelView Matrix can be used in this project, to change the view on the projection
    //but you can also leave it alone and deal with changing the projection to a different view
    //for project 2, do not use the modelview matrix to transform the actual geometry, as you won't
    //be able to save the results
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    //check for opengl errors
    check();
}

void SpecialInput(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP:
        printf("User hit the UP Arrow\n");
        if (TMode == 1) {
            ty += 0.1;
            translate(' ', TMode, CObject, Objects, tx, ty, tz);
        } else if (VMode == 1) {
            yv += 0.1;
        }
        break;
    case GLUT_KEY_DOWN:
        printf("User hit the DOWN Arrow\n");
        if (TMode == 1) {
            ty -= 0.1;
            translate(' ', TMode, CObject, Objects, tx, ty, tz);
        } else if (VMode == 1) {
            yv -= 0.1;
        }
        break;
    case GLUT_KEY_LEFT:
        printf("User hit the LEFT Arrow\n");
        if (TMode == 1) {
            tx -= 0.1;
            translate(' ', TMode, CObject, Objects, tx, ty, tz);
        } else if (VMode == 1) {
            xv -= 0.1;
        }
        break;
    case GLUT_KEY_RIGHT:
        printf("User hit the RIGHT Arrow\n");
        if (TMode == 1) {
            tx += 0.1;
            translate(' ', TMode, CObject, Objects, tx, ty, tz);
        } else if (VMode == 1) {
            xv += 0.1;
        }
        break;
    }

    glutPostRedisplay();
}//Arrow-Keys

//gets called when a key is pressed on the keyboard

void key(unsigned char ch, int x, int y)
{
    switch (ch) {
    default:
        //prints out which key the user hit
        printf("User hit the \"%c\" key\n", ch);
        break;
    }

    if (isdigit(ch)) {
        unsigned char temp = ch;

        //Different Object = Reset
        if (CObject != temp - 48)
            tx = ty = tz = 0;

        CObject = temp - 48;

    }//Select_Object

    if (ch == 't' || TMode == 1) {
        SMode = VMode = RMode = 0;
        translate(ch, TMode, CObject, Objects, tx, ty, tz);
    }
    if (ch == 'v' || VMode == 1) {
        TMode = SMode = RMode = 0;
        rotateView(ch, VMode = 1, scale);
    }
    if (ch == 's' || SMode == 1) {
        TMode = VMode = RMode = 0;
        scaleObject(ch, SMode = 1, Sf, CObject, Objects);
    }
    if (ch == 'r' || RMode == 1) {
        TMode = VMode = SMode = 0;
        RotateObject(ch, RMode, CObject, angle, Objects, RX0, RX1, RotAxis);
    }
    if(ch == 'n'){
        NMode = NMode ? 0 : 1;
        if(NMode)
            pixel_size = 3;
        else 
            pixel_size = 1;
    }
        
            

    //redraw the scene after keyboard input
    glutPostRedisplay();
}


//gets called when a mouse button is pressed

void mouse(int button, int state, int x, int y)
{
    //print the pixel location, and the grid location
    printf("MOUSE AT PIXEL: %d %d, GRID: %d %d\n", x, y, (int) (x / pixel_size), (int) ((win_height - y) / pixel_size));
    switch (button) {
    case GLUT_LEFT_BUTTON: //left button
        printf("LEFT ");
        break;
    case GLUT_RIGHT_BUTTON: //right button
        printf("RIGHT ");
    default:
        printf("UNKNOWN "); //any other mouse button
        break;
    }
    if (state != GLUT_DOWN) //button released
        printf("BUTTON UP\n");
    else
        printf("BUTTON DOWN\n"); //button clicked

    //redraw the scene after mouse click
    glutPostRedisplay();
}

//gets called when the curser moves accross the scene

void motion(int x, int y)
{
    //redraw the scene after mouse movement
    glutPostRedisplay();
}

//checks for any opengl errors in the previous calls and
//outputs if they are present

void check()
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("GLERROR: There was an error %s\n", gluErrorString(err));
        exit(1);
    }
}

void drawAxis()
{
    glLineWidth(4.0); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
    glBegin(GL_LINES);

    // X-Axis (BLUE)  
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(scale, 0.0, 0.0);

    // Y-Axis (RED)
    glColor3f(1.0, 0.7, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, scale, 0.0);

    //Z-Axis (GREEN)
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, scale);

    glEnd();
}

void rotateView(char id, int &Mode, float &rval)
{
    if (id == '+') {
        rval += 0.1;
    }//positive
    else if (id == '-') {
        rval -= 0.1;
    }//negative    

    std::cout << " x = " << xv << " y = " << yv << " z = " << zv << "\n";
    std::cout << "S = " << scale << "\n";
}

void displayOptions()
{

    //glLoadIdentity();
    //glViewport(win_width - options_width*pixel_size, 0, options_width*pixel_size, win_height);
    //glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
    //glLoadIdentity(); // Reset The Projection Matrix

    //gluOrtho2D(0.0, options_width * 32, 0.0, options_width * 48);

    char text[] = "Slider 1";

    output(10, 10, text);

    glLineWidth(4.0); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
    glBegin(GL_LINES);

    // X-Axis (BLUE)  
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(100, 350);
    glVertex2f(500, 350);

    glEnd();
}

void output(GLfloat x, GLfloat y, char *text)
{
    char *p;

    glPushMatrix();
    glTranslatef(x, y, 0);
    for (p = text; *p; p++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
}
