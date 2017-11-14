/* 
 * File:   transformations.cpp
 * Author: alex
 * 
 * Created on November 11, 2017, 8:21 PM
 */

#include "transformations.h"
#define PI 3.14159265
Matrix MatRot;

using namespace std;

void translate(char id, int &Mode, int OID, std::vector<Object> &Objects, GLfloat &tx, GLfloat &ty, GLfloat &tz) {

    if (id == 't' && Mode == 0) {
        std::cout << "TMode Enabled" << "\n";
        Mode = 1;

    } else if (id == 't' && Mode == 1) {
        std::cout << "TMode Disabled" << "\n";
        Mode = 0;
    }

    if (id == '+' && Mode) {
        tz += 0.1;
    }//positive
    else if (id == '-' && Mode) {
        tz -= 0.1;
    }//negative    

    Matrix MT; //Identity Translation Matrix
    MT.mat[0][3] = tx;
    MT.mat[1][3] = ty;
    MT.mat[2][3] = tz;

    if (OID < Objects.size() && Mode) {
        for (int index = 0; index < Objects[OID].VList.size(); index++) {

            Point Pt = MT * Objects[OID].VList[index];
            Objects[OID].VList[index] = Pt;

        }//Update Vertices
        Objects[OID].center = MT * Objects[OID].center;

        UpdateTList(Objects, OID);        
    }
    tx = ty = tz = 0;
       
}//Translate

void scaleObject(char id, int &Mode, float Sf, int OID, std::vector<Object> &Objects) {
    std::cout << "Scale Factor \n" << Sf << "\n";

    if (id == '+' && Mode) {
        Sf += 0.1;
    }//positive
    else if (id == '-' && Mode) {
        Sf -= 0.1;
    }//negative 

    Matrix MS;
    MS.mat[0][0] = Sf;
    MS.mat[0][3] = (1 - Sf) * Objects[OID].center.xyz[0][0];
    MS.mat[1][1] = Sf;
    MS.mat[1][3] = (1 - Sf) * Objects[OID].center.xyz[1][0];
    MS.mat[2][2] = Sf;
    MS.mat[2][3] = (1 - Sf) * Objects[OID].center.xyz[2][0];

    if (OID < Objects.size() && Mode) {
        for (int index = 0; index < Objects[OID].VList.size(); index++) {

            Point Pt = MS * Objects[OID].VList[index];
            Objects[OID].VList[index] = Pt;
        }//Update Vertices

        UpdateTList(Objects, OID);
    }
}

void RotateObject(char id, int &Mode, int OID, GLfloat angle, std::vector<Object> &Objects, Point &RX0, Point &RX1, bool &RotAxis) {
      
    if (id == 'r' && RotAxis == false) {
        std::cout << "RMode Enabled" << "\n";
        std::cout << "Enter the Coordinate of 1st Point (separated by spaces) : ";
        std::cin >> RX0.xyz[0][0] >> RX0.xyz[1][0] >> RX0.xyz[2][0];
        std::cout << "Enter the Coordinate of 2nd Point (separated by spaces) : ";
        std::cin >> RX1.xyz[0][0] >> RX1.xyz[1][0] >> RX1.xyz[2][0];
        Mode = 1;
        RotAxis = true;

    } else if (id == 'r' && Mode == 1) {
        std::cout << "RMode Disabled" << "\n";
        Mode = 0;
        RotAxis = false;
    } else {
        Mode = 1;
    }

    if (id == '+' && Mode == 1) {
        angle *= 1;
    }//positive
    else if (id == '-' && Mode == 1) {
        angle *= -1;
    }//negative   

    Matrix MR;

    if (OID < Objects.size() && Mode) {
        for (int index = 0; index < Objects[OID].VList.size(); index++) {

            setIdentity();
            Point Pt = Objects[OID].VList[index];
            rotate3D(RX0, RX1, angle, MatRot);
            CopyMatrix(MR);
            Pt = MR * Pt;
            Objects[OID].VList[index] = Pt;
            setIdentity();

        }//Update Vertices

        Objects[OID].center = calculateCenter(Objects[OID].VList); //recalculate CenterPt
        UpdateTList(Objects, OID);
    }
}

void rotate3D(Point RX0, Point RX1, GLfloat theta, Matrix &MatRot) {

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

    Matrix MT((-1) * RX0.xyz[0][0], (-1) * RX0.xyz[1][0], (-1) * RX0.xyz[2][0]);

    MatRot = MT * MatRot;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
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
    
    MatRot = MTinv * MatRot;

}

void CopyMatrix(Matrix &m) {
    GLint row, col;

    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            m.mat[row][col] = MatRot.mat[row][col];
        }
    }
}

void setIdentity() {
    Matrix MI;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            MatRot.mat[i][j] = MI.mat[i][j];

}