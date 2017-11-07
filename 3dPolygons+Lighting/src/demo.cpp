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

#define PI 3.14159265
GLfloat angle = 1;

/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;

//the size of pixels sets the initial window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
float pixel_size;

/*Window information*/
int win_height;
int win_width;

std::vector<Object> Objects;
Point RX0, RX1;

GLfloat xv = 0.0, yv = 0.0, zv = -1.0;
GLfloat tx = 0.0, ty = 0.0, tz = 0.0;
GLfloat scale = 1.0;
float Sf = 1.0;

int VMode = 0, TMode = 0, SMode = 0, RMode = 0;
int CObject = 0;
bool Gselect;
bool RotAxis = false;
Matrix MatRot;

void CopyMatrix(Matrix &m);
void rotateView(char id, int &Mode, float &rval);
void rotate3D(Point RX0, Point RX1, float theta);
void translate(char id, int &Mode, int OID);
void scaleObject(char id, int &Mode, float Sf, int OID);
void RotateObject(char id, int &Mode, int OID, GLfloat angle);
void drawAxis();
void drawRotAxis();
void drawSegment(Point p0, Point p1);
void setIdentity();

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
    readFile(Objects);
    setIdentity();
    
    //the number of pixels in the grid
    grid_width = 128;
    grid_height = 128;

    //the size of pixels sets the inital window height and width
    //don't make the pixels too large or the screen size will be larger than
    //your display size
    pixel_size = 5;

    /*Window information*/
    win_height = grid_height*pixel_size;
    win_width = grid_width*pixel_size;

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
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //checks for OpenGL errors
    check();
}

//called repeatedly when glut isn't doing anything else

void idle()
{
    //redraw the scene over and over again
    glutPostRedisplay();
}

//this is where we render the screen

void display()
{
    //clears the screen
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix

    for (unsigned int i = 0; i < 4; i++) {


        if (i == 0) {
            glLoadIdentity();
            glViewport(0, win_height / 2, win_width / 2, win_height / 2);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glOrtho((scale*-1), scale, (scale*-1), scale, (scale*-1), scale);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0.5, 0.5, 0.5, xv, yv, zv, 0.0, 1.0, 0.0);

            draw_lines();

        }
        if (i == 1) {
            //Code Adapted From NEHE tutorial on Multiple Viewports
            // Set The Viewport To The Top Right.  
            glLoadIdentity();
            glViewport((win_width / 2) + 4, (win_height / 2) + 4, (win_width / 2) - 16, (win_height / 2) - 16);
            glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
            glLoadIdentity(); // Reset The Projection Matrix
            // Set Up Ortho Mode To Fit 1/4 The Screen (Size Of A Viewport)
            //gluOrtho2D(1.0, 2.0, 1.0, 2.0);
            gluOrtho2D(0.0, 1.0, 0.0, 1.0);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0);

            draw_lines();

        }
        if (i == 2) {
            glLoadIdentity();
            glViewport((win_width / 2) + 4, 4, (win_width / 2) - 16, (win_height / 2) - 16);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            //gluOrtho2D(1.0, 2.0, 0.0, 1.0);
            gluOrtho2D(0.0, 1.0, 0.0, 1.0);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0, 0.0);

            draw_lines();
        }
        if (i == 3) {
            glLoadIdentity();
            glViewport(4, 4, (win_width / 2) - 16, (win_height / 2) - 16);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0.0, 1.0, 0.0, 1.0);

            draw_lines();
        }
    }

    writeFile(Objects);
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

    Point p0, p1;

    //tells openGL to interpret every two calls to glVertex as a line

    for (int k = 0; k < Objects.size(); k++)
        for (unsigned int i = 0; i < Objects[k].EList.size(); i++) {


            p0 = Objects[k].EList[i].first;
            p1 = Objects[k].EList[i].second;

            if (k == CObject)
                Gselect = true;
            else
                Gselect = false;

            drawSegment(p0, p1);
        }

}

void drawRotAxis()
{

    glLineWidth(5.0); //sets the "width" of each line we are rendering
    glBegin(GL_LINES);
    //Draw Rotation Lines
    glColor3f(1.0, 0.07, 0.57);
    glVertex3f(RX0.xyz[0][0], RX0.xyz[1][0], RX0.xyz[2][0]);
    glVertex3f(RX1.xyz[0][0], RX1.xyz[1][0], RX1.xyz[2][0]);
    glEnd();

}

void drawSegment(Point p0, Point p1)
{
    glLineWidth(1.0); //sets the "width" of each line we are rendering
    glBegin(GL_LINES);
    //Draw Black Lines
    if (Gselect)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3f(0.0, 0.0, 0.0);
    glVertex3f(p0.xyz[0][0], p0.xyz[1][0], p0.xyz[2][0]);
    glVertex3f(p1.xyz[0][0], p1.xyz[1][0], p1.xyz[2][0]);
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

    //creates a rendering area across the window
    glViewport(0, 0, width, height);

    // uses an orthogonal projection matrix so that
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //you guys should look up this function
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);

    //clear the modelview matrix
    //the ModelView Matrix can be used in this project, to change the view on the projection
    //but you can also leave it alone and deal with changing the projection to a different view
    //for project 2, do not use the modelview matrix to transform the actual geometry, as you won't
    //be able to save the results
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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
            translate(' ', TMode, CObject);
        }
        else if(VMode == 1){
            yv += 0.1;
        }
        break;
    case GLUT_KEY_DOWN:
        printf("User hit the DOWN Arrow\n");
        if (TMode == 1) {
            ty -= 0.1;
            translate(' ', TMode, CObject);
        }
        else if(VMode == 1){
            yv -= 0.1;
        }
        break;
    case GLUT_KEY_LEFT:
        printf("User hit the LEFT Arrow\n");
        if (TMode == 1) {
            tx -= 0.1;
            translate(' ', TMode, CObject);
        }
        else if(VMode == 1){
            xv -= 0.1;
        }
        break;
    case GLUT_KEY_RIGHT:
        printf("User hit the RIGHT Arrow\n");
        if (TMode == 1) {
            tx += 0.1;
            translate(' ', TMode, CObject);
        }
        else if(VMode == 1){
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
        translate(ch, TMode, CObject);
    }
    if (ch == 'v' || VMode == 1) {
        TMode = SMode = RMode = 0;
        rotateView(ch, VMode = 1, scale);
    }
    if (ch == 's' || SMode == 1) {
        TMode = VMode = RMode = 0;
        scaleObject(ch, SMode = 1, Sf, CObject);
    }
    if (ch == 'r' || RMode == 1) {
        TMode = VMode = SMode = 0;      
        RotateObject(ch, RMode, CObject, angle);
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

void scaleObject(char id, int &Mode, float Sf, int OID)
{
    std::cout << "Scale Factor \n" << Sf << "\n";

    if (id == '+' && SMode) {
        Sf += 0.1;
    }//positive
    else if (id == '-' && SMode) {
        Sf -= 0.1;
    }//negative 

    Matrix MS;
    MS.mat[0][0] = Sf;
    MS.mat[0][3] = (1 - Sf) * Objects[OID].center.xyz[0][0];
    MS.mat[1][1] = Sf;
    MS.mat[1][3] = (1 - Sf) * Objects[OID].center.xyz[1][0];
    MS.mat[2][2] = Sf;
    MS.mat[2][3] = (1 - Sf) * Objects[OID].center.xyz[2][0];

    if (OID < Objects.size() && SMode) {
        for (int index = 0; index < Objects[OID].VList.size(); index++) {

            Point Pt = MS * Objects[OID].VList[index];
            Objects[OID].VList[index] = Pt;
        }//Update Vertices

        for (int k = 0; k < Objects[OID].EList.size(); k++) {
            int p0, p1;
            p0 = Objects[OID].EMap[k].first;
            p1 = Objects[OID].EMap[k].second;
            std::pair<Point, Point> p = {Objects[OID].VList[p0], Objects[OID].VList[p1]};
            Objects[OID].EList[k] = p;
        }//Update_Edge_List          
    }
}

void translate(char id, int &Mode, int OID)
{

    if (id == 't' && TMode == 0) {
        std::cout << "TMode Enabled" << "\n";
        TMode = 1;

    } else if (id == 't' && TMode == 1) {
        std::cout << "TMode Disabled" << "\n";
        TMode = 0;
    }

    if (id == '+' && TMode) {
        tz += 0.1;
    }//positive
    else if (id == '-' && TMode) {
        tz -= 0.1;
    }//negative    

    Matrix MT; //Identity Translation Matrix
    MT.mat[0][3] = tx;
    MT.mat[1][3] = ty;
    MT.mat[2][3] = tz;

    if (OID < Objects.size() && TMode) {
        for (int index = 0; index < Objects[OID].VList.size(); index++) {

            Point Pt = MT * Objects[OID].VList[index];
            Objects[OID].VList[index] = Pt;
        }//Update Vertices
        Objects[OID].center = MT * Objects[OID].center;

        for (int k = 0; k < Objects[OID].EList.size(); k++) {
            int p0, p1;
            p0 = Objects[OID].EMap[k].first;
            p1 = Objects[OID].EMap[k].second;
            std::pair<Point, Point> p = {Objects[OID].VList[p0], Objects[OID].VList[p1]};
            Objects[OID].EList[k] = p;
        }//Update_Edge_List          
    }
    tx = ty = tz = 0;
}//Translate

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

void RotateObject(char id, int &Mode, int OID, GLfloat angle)
{
    if (id == 'r' && RotAxis == false) {
        std::cout << "RMode Enabled" << "\n";
        std::cout << "Enter the Coordinate of 1st Point (separated by spaces) : ";
        std::cin >> RX0.xyz[0][0] >> RX0.xyz[1][0] >> RX0.xyz[2][0];
        std::cout << "Enter the Coordinate of 2nd Point (separated by spaces) : ";
        std::cin >> RX1.xyz[0][0] >> RX1.xyz[1][0] >> RX1.xyz[2][0];
        RMode = 1;
        RotAxis = true;

    } else if (id == 'r' && RMode == 1) {
        std::cout << "RMode Disabled" << "\n";
        RMode = 0;
        RotAxis = false;
    } else {
        RMode = 1;
    }

    if (id == '+' && RMode == 1) {
        angle *= 1;
    }//positive
    else if (id == '-' && RMode == 1) {
        angle *= -1;
    }//negative   

    Matrix MR;
    
    if (OID < Objects.size() && RMode) {
        for (int index = 0; index < Objects[OID].VList.size(); index++) {
                                  
            Point Pt = Objects[OID].VList[index];        
            rotate3D(RX0, RX1, angle);     
            CopyMatrix(MR);          
            Pt = MR * Pt;       
            Objects[OID].VList[index] = Pt;
            setIdentity();
                       
        }//Update Vertices
        
        Objects[OID].center = calculateCenter(Objects[OID].VList);//recalculate CenterPt

        for (int k = 0; k < Objects[OID].EList.size(); k++) {
            int p0, p1;
            p0 = Objects[OID].EMap[k].first;
            p1 = Objects[OID].EMap[k].second;
            std::pair<Point, Point> p = {Objects[OID].VList[p0], Objects[OID].VList[p1]};
            Objects[OID].EList[k] = p;
        }//Update_Edge_List          
    }
}

void rotate3D(Point RX0, Point RX1, GLfloat theta)
{
    
    Matrix MQ;
    Matrix MI;
 
    GLfloat axisVecLength = sqrt((RX1.xyz[0][0] - RX0.xyz[0][0]) * (RX1.xyz[0][0] - RX0.xyz[0][0])
            +(RX1.xyz[1][0] - RX0.xyz[1][0]) * (RX1.xyz[1][0] - RX0.xyz[1][0])
            +(RX1.xyz[2][0] - RX0.xyz[2][0]) * (RX1.xyz[2][0] - RX0.xyz[2][0])
            );
    
    GLfloat cosA = cos(theta * PI / 180);
    GLfloat tempA = cosA;
    GLfloat oneC = 1 - tempA;
    GLfloat sinA = sin(theta * PI / 180);

    GLfloat ux = (RX1.xyz[0][0] - RX0.xyz[0][0]) / axisVecLength;
    GLfloat uy = (RX1.xyz[1][0] - RX0.xyz[1][0]) / axisVecLength;
    GLfloat uz = (RX1.xyz[2][0] - RX0.xyz[2][0]) / axisVecLength;

    Matrix MT((-1)*RX0.xyz[0][0], (-1)*RX0.xyz[1][0], (-1)*RX0.xyz[2][0]);
    
    std::cout << "MT: \n" << MT << std::endl;
    
    MatRot = MatRot * MT;
    
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++){
            MQ.mat[i][j] = MI.mat[i][j];
        }
             
    MQ.mat[0][0] = ux * ux * oneC + cosA;
    MQ.mat[0][1] = ux * uy * oneC - uz*sinA;
    MQ.mat[0][2] = ux * uz * oneC + uy*sinA;
    MQ.mat[1][0] = uy * ux * oneC + uz*sinA;
    MQ.mat[1][1] = uy * uy * oneC + cosA;
    MQ.mat[1][2] = uy * uz * oneC - ux*sinA;
    MQ.mat[2][0] = uz * ux * oneC - uy*sinA;
    MQ.mat[2][1] = uz * uy * oneC + ux*sinA;
    MQ.mat[2][2] = uz * uz * oneC + cosA;

    MatRot = MQ * MatRot;

    Matrix MTinv(RX0.xyz[0][0], RX0.xyz[1][0], RX0.xyz[2][0]);
    MatRot = MatRot * MTinv;

}

void CopyMatrix(Matrix &m){
    GLint row, col;
    
    for(row = 0; row < 4; row++){
        for(col = 0; col < 4; col++){
            m.mat[row][col] = MatRot.mat[row][col];
        }      
    }   
}

void setIdentity(){
    Matrix MI;
    
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            MatRot.mat[i][j] = MI.mat[i][j];
    
}
