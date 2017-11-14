#include "data.h"

using namespace std;

int VP = 0;
Vector I1;
Vector I2;
Vector I3;
Vector IL;
Vector IR;
Vector Ic;

void draw_pix(float x, float y, float Ic) {
    
    glBegin(GL_POINTS);
    
    glColor3f(Ic, Ic, Ic);
    
    if(VP == 3)
        glVertex3f(x, y, 0);
    else if(VP == 2)
        glVertex3f(x, 0, y);
    else if(VP == 1)
        glVertex3f(0, x, y);
    
    glEnd();
}

Point calculateCenter(std::vector<Point> VList)
{

    Point center;
    for (int i = 0; i < VList.size(); i++) {

        center.xyz[0][0] += VList[i].xyz[0][0] / VList.size();
        center.xyz[1][0] += VList[i].xyz[1][0] / VList.size();
        center.xyz[2][0] += VList[i].xyz[2][0] / VList.size();
    }

    return center;
}

void UpdateTList(std::vector<Object> &o, int &OID)
{
    int p1 = 0, p2 = 0, p3 = 0;
    for (int k = 0; k < o[OID].TList.size(); k++) {
        p1 = o[OID].TList[k].p1.id;
        p2 = o[OID].TList[k].p2.id;
        p3 = o[OID].TList[k].p3.id;

        o[OID].TList[k].p1 = o[OID].VList[p1];
        o[OID].TList[k].p2 = o[OID].VList[p2];
        o[OID].TList[k].p3 = o[OID].VList[p3];
        o[OID].TList[k].p1.id = p1;
        o[OID].TList[k].p2.id = p2;
        o[OID].TList[k].p3.id = p3;
    }//Update_Edge_List    
}

void calculateNormalV(Object &o)
{

    o.NormVecList.clear();

    Vector avgNormal(0, 0, 0);

    int k = 0; // Adjacent Vectors

    for (int i = 0; i < o.VList.size(); i++) {

        for (int j = 0; j < o.TList.size(); j++) {

            if (o.VList[i].id == o.TList[j].p1.id || o.VList[i].id == o.TList[j].p2.id || o.VList[i].id == o.TList[j].p3.id) {

                k++;
                Point p0 = o.VList[o.TList[j].p1.id];
                Point p1 = o.VList[o.TList[j].p2.id];
                Point p2 = o.VList[o.TList[j].p3.id];


                Vector v0(p0.xyz[0][0], p0.xyz[1][0], p0.xyz[2][0]);
                Vector v1(p1.xyz[0][0], p1.xyz[1][0], p1.xyz[2][0]);
                Vector v2(p2.xyz[0][0], p2.xyz[1][0], p2.xyz[2][0]);

                Vector N = v1 - v0;
                Vector M = v2 - v0;

                Vector vN = Vector::crossProduct(N, M);

                if (vN.length()) {
                    Vector::normalize(vN);
                    Vector::normalize(vN);
                }

                if (k == 1)
                    avgNormal = avgNormal + vN;


                if (Vector::dotProduct(vN, avgNormal) >= 0) {

                    float nL = vN.length();

                    if (nL > 0) {
                        avgNormal = avgNormal + vN;
                        avgNormal = avgNormal / nL;
                        avgNormal = avgNormal / k; // Average Adjacent Normals
                    }

                } else {
                    vN.x *= -1;
                    vN.y *= -1;
                    vN.z *= -1;
                    float nL = vN.length();

                    if (nL > 0) {
                        avgNormal = avgNormal + vN;
                        avgNormal = avgNormal / nL;
                        avgNormal = avgNormal / k; // Average Adjacent Normals
                    }
                }
                
            }//If Vertex is element of Triangle Edge          
        }//In each Triangle Tuple

        k = 0; // Reset for New Vertex
        o.NormVecList[i] = avgNormal;
        avgNormal.x = avgNormal.y = avgNormal.z = 0;

    }//For All vertices
}//Calculate Normal Vectors

void calculateIntensity(Object &o, Light &lfx)
{
    int v;

    for (int i = 0; i < 4; i++) {

        cout << "\n******* VIEWPORT @ index = " << i << "**************\n";
        if (i == 0) {
            Vector fp(0, 0, (-1 * lfx.f));

            for (v = 0; v < o.VList.size(); v++) {
                o.Ip0[v] = Phong(o, fp, lfx, v);
            }//calculate Intensity 

        }//GeneralViewport
        else if (i == 1) {
            Vector fp((-1 * lfx.f), lfx.K, lfx.K);

            for (v = 0; v < o.VList.size(); v++) {
                o.Ip1[v] = Phong(o, fp, lfx, v);
            }//calculate Intensity 

        }//YZ Viewport
        else if (i == 2) {
            Vector fp(lfx.K, (-1 * lfx.f), lfx.K);

            for (v = 0; v < o.VList.size(); v++) {
                o.Ip2[v] = Phong(o, fp, lfx, v);
            }//calculate Intensity 

        }//XZ Viewport
        else if (i == 3) {
            Vector fp(lfx.K, lfx.K, (-1 * lfx.f));

            for (v = 0; v < o.VList.size(); v++) {
                o.Ip3[v] = Phong(o, fp, lfx, v);
            }//calculate Intensity 
        }//XY Viewport

    }//For each viewPort

}//Calculate Intensity

Vector Phong(Object &o, Vector &fp, Light &lfx, int v)
{
    Vector Vout(0, 0, 0);

    float ka = o.mat.Ka.x; // scalar material properties
    float kd = o.mat.Kd.x;
    float ks = o.mat.Ks.x;
    int n = o.mat.n;
    Point p = o.VList[v];

    Vector vp(p.xyz[0][0], p.xyz[1][0], p.xyz[2][0]); // Vertex Position
    Vector vn = o.NormVecList[v]; // Normal Vector
    Vector vx(lfx.src.x, lfx.src.y, lfx.src.z); // Light Source Position


    Vector vv = fp - vp;
    Vector::normalize(vv);

    Vector vl = vx - vp;
    Vector::normalize(vl);

    float rs = (2 * Vector::dotProduct(vn, vl));
    Vector vr0 = vn * rs;
    Vector vr = vr0 - vl;
    Vector::normalize(vr);

    Vector distance = fp - vp;
    float C = distance.length() + lfx.K; //Constant Distance Formula


    Vector v0(lfx.Ia.x * ka, lfx.Ia.y * ka, lfx.Ia.z * ka);
    Vector v1(lfx.Il.x / C, lfx.Il.y / C, lfx.Il.z / C);

    float s1 = Vector::dotProduct(vl, vn) * kd;
    float s2 = pow(Vector::dotProduct(vr, vv), n) * ks;
    float s3 = s1 + s2;

    v1 = v1 * s3;

    Vout = v0 + v1;

    //Vector::normalize(Vout);
    if(Vout.x < 0)
        //Vout.x = Vout.y = Vout.z = 1;
    cout << "\nOutput Phong Vector @ index = " << v << "\n";
    cout << Vout;

    return Vout;
}

void fill_triangles(std::vector<Vector> &Ip, Object &obj, touple_t &TList, int ViewPort)
{   
    //project triangle into pixel space
    vpt v1,v2,v3;
    project(ViewPort,TList,v1,v2,v3, Ip);
    
    VP = ViewPort;
    
    //create edges, make sure that the points are sorted
    //by there y values
    edge e1;
    if(v1.y < v2.y )
        e1 = {v1,v2,0,0,0,0};
    else
        e1 = {v2,v1,0,0,0,0};

    edge e2;
    if(v1.y < v3.y)
        e2 = {v1,v3,0,0,0,0};
    else
        e2 = {v3,v1,0,0,0,0};

    edge e3;
    if(v2.y < v3.y)
        e3 = {v2,v3,0,0,0,0};
    else
        e3 = {v3,v2,0,0,0,0};
       
    //intersect edges
    scanline_edges(e1,e2);
    scanline_edges(e1,e3);
    scanline_edges(e2,e3);
     
}

//assumes e*.p1.y <= e*.p2.y
void scanline_edges(const edge &e1, const edge &e2){
    if(e1.p1.y == e1.p2.y){//parallel line test
        render_scanline(e1.p1.y,e1.p1.x,e1.p2.x);
        return;
    }
    if(e2.p1.y == e2.p2.y){//parallel line test
        render_scanline(e2.p2.y,e2.p1.x,e2.p2.x);
        return;
    }

    float ymin = max(e1.p1.y,e2.p1.y);
    float ymax = min(e1.p2.y,e2.p2.y);
    //scan from bottom to top of edges
    for(float y = ymin; y <= ymax; y+=0.001){
        float x1,x2; //intersection points
        intersect_edge(y,e1,x1,IL,I1,I2);
        intersect_edge(y,e2,x2,IR,I1,I3);
        if(x1 < x2)
           render_scanline(y,x1,x2);
        else
           render_scanline(y,x2,x1);
    }

}

//assumes e1 isn't parallel to the x axis
//could do interpolation on the edge here as well and return the color intensity
bool intersect_edge(float scany, const edge& e, float &x_int, Vector &I, Vector &V1, Vector &V2){
    if(e.p2.y==e.p1.y) return false;//parallel lines (0 or inf intersection points)
     x_int = (scany-e.p1.y)*(e.p2.x-e.p1.x)/(e.p2.y-e.p1.y)+e.p1.x;
     
     
     I = { (V1.x * ((scany - e.n2.x)/(e.length()))) + (V2.x * ((e.n1.x - scany)/(e.length()))), 
           (V1.y * ((scany - e.n2.y)/(e.length()))) + (V2.y * ((e.n1.y - scany)/(e.length()))),
           (V1.z * ((scany - e.n2.z)/(e.length()))) + (V2.z * ((e.n1.z - scany)/(e.length())))
         };
     
     if(I.x < 0){
         //I.x = I.y = I.z = abs(I.x);
     }
     //cout << "\nI == \n" << I << "\n******************"; 
     //IF NEGATIVE SET TO ZERO 0
             
     return true;
}

//could include intensity values and use those to interpolate here
void render_scanline(float y, float x1, float x2){
    float py= y;
    float px= x1;
    
 
    Ic = { (IL.x * ((IR.x - px)/(IR.x - IL.x))) + (IR.x * ((px - IL.x)/(IR.x - IL.x))), 
           (IL.y * ((IR.y - px)/(IR.y - IL.y))) + (IR.y * ((px - IL.y)/(IR.y - IL.y))),
           (IL.z * ((IR.z - px)/(IR.z - IL.z))) + (IR.z * ((px - IL.z)/(IR.z - IL.z)))    
    };
    //cout << "\nIc == \n" << Ic << "\n******************";
    
    for(px; px < x2; px += 0.001)
        draw_pix(px,py,Ic.x);
}


//projects into pixel/framebuffer space
void project(int ViewPort, touple_t &TList, vpt &v0, vpt &v1, vpt &v2, std::vector<Vector> &Ip){
    v0.x = TList.p1.xyz[0][0];
    v0.y = TList.p1.xyz[1][0];
    v0.z = TList.p1.xyz[2][0];
    
    v1.x = TList.p2.xyz[0][0];
    v1.y = TList.p2.xyz[1][0];
    v1.z = TList.p2.xyz[2][0];
    
    v2.x = TList.p3.xyz[0][0];
    v2.y = TList.p3.xyz[1][0];
    v2.z = TList.p3.xyz[2][0];

    switch(ViewPort){
    case 1:
        v0 = {v0.y,v0.z,0.0};
        v1 = {v1.y,v1.z,0.0};
        v2 = {v2.y,v2.z,0.0};
        I1 = {Ip[TList.p1.id].y, Ip[TList.p1.id].z, 0};
        I2 = {Ip[TList.p2.id].y, Ip[TList.p2.id].z, 0};
        I3 = {Ip[TList.p3.id].y, Ip[TList.p3.id].z, 0};
        break;
    case 2:
        v0 = {v0.x,v0.z,0.0};
        v1 = {v1.x,v1.z,0.0};
        v2 = {v2.x,v2.z,0.0};
        I1 = {Ip[TList.p1.id].x, Ip[TList.p1.id].z, 0};
        I2 = {Ip[TList.p2.id].x, Ip[TList.p2.id].z, 0};
        I3 = {Ip[TList.p3.id].x, Ip[TList.p3.id].z, 0};
        break;
    case 3:
        v0 = {v0.x,v0.y,0.0};
        v1 = {v1.x,v1.y,0.0};
        v2 = {v2.x,v2.y,0.0};
        I1 = {Ip[TList.p1.id].x, Ip[TList.p1.id].y, 0};
        I2 = {Ip[TList.p2.id].x, Ip[TList.p2.id].y, 0};
        I3 = {Ip[TList.p3.id].x, Ip[TList.p3.id].y, 0};
        break;
    default:
        cout << "This shouldn't happen\n";
        exit(0);
    }
}



