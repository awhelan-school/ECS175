/* 
 * File:   Vector.cpp
 * Author: Whelan
 * 
 * Created on November 6, 2017, 7:39 PM
 */

#include "Vector.h"

Vector::Vector() {
}

Vector::Vector(const Vector& orig) {
    this->x = orig.x;
    this->y = orig.y;
    this->z = orig.z;   
}//Copy-Constructor

Vector::~Vector() {
    //Virtual Destructor
}

ostream& operator<<(ostream &out, Vector &v) {
    
    out << "[ " << v.x << " ]" << "\n";
    out << "[ " << v.y << " ]" << "\n";
    out << "[ " << v.z << " ]" << "\n";   
    out << "\n";   
    return out;
}

Vector Vector::crossProduct(Vector &v1, Vector &v2) {
    
    float x = v1.y * v2.z - v1.z * v2.y;
    float y = v1.z * v2.x - v1.x * v2.z;
    float z = v1.x * v2.y - v1.y * v2.x;
    
    Vector v(x,y,z);
    //cout << "Product of v1 x v2 \n" << v1 << v2 << " = \n" << v << endl;
    return v;
}

float Vector::dotProduct(Vector &v1, Vector &v2) {
    
    float scalar = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    //cout << "\n Scaler Quantity of v1 * v2 " << v1 << v2 << " equals \n" << scalar << "\n";
    return scalar;

}

Vector Vector::normalize(Vector &v){
   
    Vector copy(v);   
    float length = copy.length();
    copy.x /= length;
    copy.y /= length;
    copy.z /= length;
    
    v.x /= length;
    v.y /= length;
    v.z /= length;
    
    //cout << "Normalized Vector V \n" << v << "equals\n" << copy;
    return copy;
    
}

float Vector::length(){
    return(sqrt( (x * x) + (y * y) + (z * z)));
}

Vector Vector::operator+ (Vector &v){
    Vector out;   
    out.x = x + v.x;
    out.y = y + v.y;
    out.z = z + v.z;
    return out;
    
}
Vector Vector::operator -(Vector& v){
    Vector out;
    out.x = x - v.x;
    out.y = y - v.y;
    out.z = z - v.z;
    return out;    
}

Vector Vector::operator /(int k){
    Vector out;
    
    out.x = x / k;
    out.y = y / k;
    out.z = z / k;
    
    return out;    
}
