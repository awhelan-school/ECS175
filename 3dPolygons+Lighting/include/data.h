#ifndef DATA_H
#define DATA_H

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>

#else //linux
#include <GL/gl.h>
#endif

#include <vector>
#include <tuple>
#include <utility>
#include <string>
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cmath>
#include "Vector.h"

typedef struct point{
    
    int id;
    float xyz[4][1] = { {0}, {0}, {0}, {1} };
    
    inline friend std::ostream& operator<< (std::ostream& out, const point &p){
    
        for(int i = 0; i < 4; i++){        
            for(int j = 0; j < 1; j++){            
            out << p.xyz[i][j] << "\t";
        }
        out << "\n";
    }
    return out;
    }
}Point;

typedef struct{
    float x,y,z;
}vector_t;

typedef struct{
    Point p1,p2,p3;
}touple_t;

typedef struct{
    vector_t src; // X Light Source Position
    vector_t Ia; // Ambient Component
    vector_t Il; // Light Intensity
    float f; // From point
    float K; // Constant Distance
}Light;

typedef struct{
    vector_t Ka; // Ambient 
    vector_t Kd; // Diffuse
    vector_t Ks; // Specular
    int n; // Phong Exponent
}Material;

typedef struct{
    Material mat;
    unsigned int materialID = 0;
    unsigned int vertices = 0;
    Point center;//Center of Geometry
    std::vector<Point> VList;//Vertex List
    std::vector< touple_t > TList;//number of Triangles
    std::vector< Vector > NormVecList;//Normal Vector for All vertices 
}Object;


Point calculateCenter(std::vector<Point> VList);
void calculateNormalV(Object &o);
void UpdateTList(std::vector<Object> &o, int &OID);
        
#endif /* DATA_H */

