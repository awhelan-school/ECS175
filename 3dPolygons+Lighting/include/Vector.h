/* 
 * File:   Vector.h
 * Author: Whelan
 *
 * Created on November 6, 2017, 7:39 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

#ifndef VECTOR_H
#define VECTOR_H

class Vector {
public:
    float x,y,z;
    Vector();
    Vector(float x, float y, float z) : x(x), y(y), z(z) {};
    Vector(const Vector& orig);
    virtual ~Vector();
    friend ostream& operator<<(ostream &out, Vector &v);
    Vector operator+ (Vector &v);
    Vector operator- (Vector &v);
    Vector operator/ (int k);
    static Vector crossProduct(Vector &v1, Vector &v2);
    static float dotProduct(Vector &v1, Vector &v2);
    static Vector normalize(Vector &v);
    float length();
private:
    
};

#endif /* VECTOR_H */

