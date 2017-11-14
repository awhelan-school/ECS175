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
#include <algorithm>
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

typedef struct
{
    Point p0, p1;
    float m; //slope
} Edge;

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
    Point center;// Center of Geometry
    std::vector<Point> VList;// Vertex List
    std::vector< touple_t > TList;// Number of Triangles
    std::vector< Vector > NormVecList;// Normal Vector for All vertices 
    std::vector< Vector > Ip0; // Intensity from xyz
    std::vector< Vector > Ip1; // Intensity from yz
    std::vector< Vector > Ip2; // Intensity from xy
    std::vector< Vector > Ip3; // Intensity from xz
}Object;


//floating point for world and object space
struct vpt{
    float x;
    float y;
    float z;
};

//index of triangles
struct tri{
    int i;
    int j;
    int k;
};

//edges for doing rasterization
struct edge{
    vpt p1;
    vpt p2;
    vpt v1;
    vpt v2;
    vpt n1;
    vpt n2;
    inline float length() const {
        return(sqrt( pow((p1.x - p2.x),2) + pow((p1.y - p2.y),2) + pow((p1.z - p2.z),2)));
    }//length of edge
    
};

void calculateIntensity(Object &o, Light &lfx);
Point calculateCenter(std::vector<Point> VList);
void calculateNormalV(Object &o);
void UpdateTList(std::vector<Object> &o, int &OID);
Vector Phong(Object &o, Vector &fp, Light &lfx, int v);

void fill_triangles(std::vector<Vector> &Ip, Object &obj, touple_t &TList, int ViewPort);

void scanline_edges(const edge &e1, const edge &e2);
bool intersect_edge(float scany, const edge& e, float &x_int, Vector &I, Vector &V1, Vector &V2);
void render_scanline(float y, float x1, float x2);
void project(int ViewPort, touple_t &TList, vpt &v0, vpt &v1, vpt &v2, std::vector<Vector> &Ip);
void draw_pix(float x, float y, float Ic);        
#endif /* DATA_H */

