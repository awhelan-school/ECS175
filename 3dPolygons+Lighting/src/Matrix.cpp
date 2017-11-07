#define PI 3.14159265

#include "Matrix.h"

Matrix::Matrix() {
    row = 4, col=4;
    for(int r = 0; r < 4; r++)
        for(int c = 0; c < 4; c++)
            mat[r][c] = (r == c);
}

Matrix::Matrix(GLfloat tx, GLfloat ty, GLfloat tz) : Matrix() {

    mat[0][3] = tx;
    mat[1][3] = ty;
    mat[2][3] = tz;
    
}

Matrix::Matrix(char type, float theta) : Matrix() {
    
    if(type == 'x'){
        mat[1][1] =  cos(theta*PI/180);
        mat[1][2] = -sin(theta*PI/180);
        mat[2][1] =  sin(theta*PI/180);
        mat[2][2] =  cos(theta*PI/180);
    }
    else if(type == 'y'){
        mat[0][0] =   cos(theta*PI/180);
        mat[0][2] =   sin(theta*PI/180);
        mat[2][0] =  -sin(theta*PI/180);
        mat[2][2] =   cos(theta*PI/180);      
    }
    else if(type == 'z'){
        mat[0][0] =  cos(theta*PI/180);
        mat[0][1] = -sin(theta*PI/180);
        mat[1][0] =  sin(theta*PI/180);
        mat[1][1] =  cos(theta*PI/180);       
    }    
}

void Matrix::Transpose(Matrix& m, Matrix &mt){   
    for(int i = 0; i < m.row; i++){
        for(int j = 0; j < m.col; j++){      
            mt.mat[j][i] = m.mat[i][j];
        }
    }
}

Matrix::~Matrix() {
}

std::ostream& operator <<(std::ostream& out, Matrix &m){
    
    for(int i = 0; i < m.row; i++){
        
        for(int j = 0; j < m.col; j++){
            
            out << m.mat[i][j] << "\t";
        }
        out << "\n";
    }
    return out;
}


Point Matrix::operator *(Point& P){
    
    Point Pout;
    Pout.xyz[3][0] = 0;
    
    for(int i = 0; i < row; i++){
        
        for(int j = 0; j < col; j++){
            
            Pout.xyz[i][0] += mat[i][j] * P.xyz[j][0];          
        }       
    }       
    return Pout;
}

Matrix Matrix::operator *(Matrix& M){
    
    Matrix Mout;
    GLint row, col;
    Matrix4x4 matTemp;
    
    for(row = 0; row < 4; row++){
        for(col = 0; col < 4; col++){
            matTemp[row][col] = this->mat[row][0] * M.mat[0][col] + this->mat[row][1] * M.mat[1][col] +
                                this->mat[row][2] * M.mat[2][col] + this->mat[row][3] * M.mat[3][col];       
        }      
    }    
    for(row = 0; row < 4; row++)
        for(col = 0; col < 4; col++)
            Mout.mat[row][col] = matTemp[row][col];
    
    return Mout;
}
