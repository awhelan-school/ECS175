#ifndef MATRIX_H
#define MATRIX_H

#include <GL/gl.h>
#include <iostream>

#include "data.h"

typedef GLfloat Matrix4x4 [4][4];

class Matrix {
public:    
    Matrix4x4 mat;
    Matrix();
    Matrix(GLfloat tx, GLfloat ty, GLfloat tz);
    Matrix(char type, float theta);
    virtual ~Matrix();
    
    static void Transpose(Matrix &m, Matrix &mt);
    friend std::ostream& operator <<(std::ostream &out, Matrix& m);
    friend std::ostream& operator <<(std::ostream &out, Point& p);
    Point  operator *(Point &P);
    Matrix operator *(Matrix &M);
private:   
    GLint row, col;
};

#endif /* MATRIX_H */

