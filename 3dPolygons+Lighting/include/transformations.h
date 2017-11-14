/* 
 * File:   transformations.h
 * Author: alex
 *
 * Created on November 11, 2017, 8:21 PM
 */

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "data.h"
#include "Matrix.h"

void translate(char id, int &Mode, int OID, std::vector<Object> &Objects, GLfloat &tx, GLfloat &ty, GLfloat &tz);
void scaleObject(char id, int &Mode, float Sf, int OID, std::vector<Object> &Objects);
void CopyMatrix(Matrix &m);
void rotate3D(Point RX0, Point RX1, float theta, Matrix &MatRot);
void RotateObject(char id, int &Mode, int OID, GLfloat angle, std::vector<Object> &Objects, Point &RX0, Point &RX1, bool &RotAxis);
void setIdentity();

#endif /* TRANSFORMATIONS_H */

