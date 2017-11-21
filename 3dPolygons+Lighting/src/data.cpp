#include "data.h"

using namespace std;

int VP = 0;
int N;
std::ofstream debug("./debug.txt", std::ios::trunc);

//map this square to the framebuffer when projecting
vpt min_vbox = {0.0, 0.0, 0.0};
vpt max_vbox = {1.0, 1.0, 0.0};


float gridW;
float gridH;

float stepy;
float stepx;

Vector I1;
Vector I2;
Vector I3;
Vector IL;
Vector IR;

void draw_pix(float x, float y, float Ic)
{

    int iPix;
    int a[10];
    int ra[10];
    int pa[10];

    if (Ic < 0) {
        Ic = 0;
    }
    if (Ic > 1) {
        Ic = 1;
    }

    if (Ic >= 0 && Ic <= 1) {

        if (N == 1) {


            if (Ic >= 0 && Ic <= 0.1)
                iPix = 0;
            else if (Ic > 0.1 && Ic <= 0.2)
                iPix = 1;
            else if (Ic > 0.2 && Ic <= 0.3)
                iPix = 2;
            else if (Ic > 0.3 && Ic <= 0.4)
                iPix = 3;
            else if (Ic > 0.4 && Ic <= 0.5)
                iPix = 4;
            else if (Ic > 0.5 && Ic <= 0.6)
                iPix = 5;
            else if (Ic > 0.6 && Ic <= 0.7)
                iPix = 6;
            else if (Ic > 0.7 && Ic <= 0.8)
                iPix = 7;
            else if (Ic > 0.8 && Ic <= 0.9)
                iPix = 8;
            else if (Ic > 0.9 && Ic <= 1.0)
                iPix = 9;

            for (int i = 0; i < 10; i++) {
                a[i] = i;
            }//Generate Numbers

            //Randomize
            std::random_shuffle(a, a + 10);
            for (int i = 0; i < 10; i++) {
                if (i < iPix)
                    ra[i] = a[i];
                else
                    ra[i] = -1;
            }

            for (int itr = 0; itr < 10; itr++) {

                for (int k = 0; k < 10; k++) {

                    if (itr == ra[k]) {
                        pa[itr] = 1;
                        break;
                    }
                    else
                        pa[itr] = 0;
                }//check if on/off pixel
            }//make pixel array

            /*
             *  [-,-][0,-][+,-]
             *  [-,0][0,0][+,0]
             *  [-,+][0,+][+,+]
             */
            
            
            for(int i = 0; i < 9; i++){
                
                if(i == 0){
                    glBegin(GL_POINTS);
                    glColor3f(pa[i], pa[i], pa[i]);
                    glVertex3f(x - 1.0, y - 1.0, 0); // index 1
                    glEnd();       
                }
                if(i == 1){
                    glBegin(GL_POINTS);
                    glColor3f(pa[i], pa[i], pa[i]);
                    glVertex3f(x, y - 1.0, 0); // index 2 
                    glEnd();
                }
                if(i == 2){
                    glBegin(GL_POINTS);
                    glColor3f(pa[i], pa[i], pa[i]);
                    glVertex3f(x + 1.0, y - 1.0, 0); //index 3 
                    glEnd();
                    
                }
                if(i == 3){
                    glBegin(GL_POINTS);
                    glColor3f(pa[i], pa[i], pa[i]);
                    glVertex3f(x - 1.0, y, 0); // index 4
                    glEnd();
                    
                }
                if(i == 4){
                    glBegin(GL_POINTS);
                    glColor3f(pa[i], pa[i], pa[i]);
                    glVertex3f(x, y, 0); //center
                    glEnd();
                    
                }
                if(i == 5){
                    glBegin(GL_POINTS);
                    glColor3f(pa[i], pa[i], pa[i]);
                    glVertex3f(x + 1.0, y, 0); //index 6
                    glEnd();
                    
                }
                if(i == 6){
                    glBegin(GL_POINTS);
                    glColor3f(pa[i], pa[i], pa[i]);
                    glVertex3f(x - 1.0, y + 1.0, 0); // index 7
                    glEnd();
                    
                }
                if(i == 7){
                    glBegin(GL_POINTS);
                    glColor3f(pa[i], pa[i], pa[i]);
                    glVertex3f(x, y + 1.0, 0); // index 8
                    glEnd();
                    
                }
                if(i == 8){
                    glBegin(GL_POINTS);
                    glColor3f(pa[i], pa[i], pa[i]);
                    glVertex3f(x + 1.0, y + 1.0, 0); //index 9
                    glEnd();
                    
                }        
            }

        }// Half Toning
        else {
            glBegin(GL_POINTS);
            glColor3f(Ic, Ic, Ic);
            glVertex3f(x + .5, y + .5, 0);
            glEnd();
        }//Regular Shading


    }//clamp Intensity 0 < i < 1

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

    //o.NormVecList.clear();
    //o.NormVecList.resize(o.VList.size());

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
                Vector::normalize(vN);

                avgNormal = avgNormal + vN;
                avgNormal = avgNormal / k; // Average by Adjacent Normals

                Vector::normalize(avgNormal);

            }//If Vertex is element of Triangle Edge          
        }//In each Triangle Tuple


        //cout << "\nK value for index ==" << i << " =====>>> " << k;
        Vector::normalize(avgNormal);
        //cout << "\nNORMALIZED VECTOR ==" << avgNormal;

        k = 0; // Reset for New Vertex
        o.NormVecList[i] = avgNormal;
        avgNormal.x = avgNormal.y = avgNormal.z = 0;

    }//For All vertices
}//Calculate Normal Vectors

void calculateIntensity(Object &o, Light &lfx)
{
    int v;
    o.Ip0.clear();
    o.Ip1.clear();
    o.Ip2.clear();
    o.Ip3.clear();


    for (int i = 0; i < 4; i++) {

        //cout << "\n******* VIEWPORT @ index = " << i << "**************\n";
        if (i == 0) {
            Vector fp(0, 0, (-1.0 * lfx.f));

            for (v = 0; v < o.VList.size(); v++) {
                o.Ip0[v] = Phong(o, fp, lfx, v);
            }//calculate Intensity 

        }//GeneralViewport
        else if (i == 1) {
            Vector fp(lfx.f, lfx.f - lfx.K, lfx.f - lfx.K);

            for (v = 0; v < o.VList.size(); v++) {
                o.Ip1[v] = Phong(o, fp, lfx, v);
            }//calculate Intensity 

        }//YZ Viewport
        else if (i == 2) {
            Vector fp(lfx.f - lfx.K, lfx.f, lfx.f - lfx.K);

            for (v = 0; v < o.VList.size(); v++) {
                o.Ip2[v] = Phong(o, fp, lfx, v);
            }//calculate Intensity 

        }//XZ Viewport
        else if (i == 3) {
            Vector fp(lfx.f - lfx.K, lfx.f - lfx.K, lfx.f);

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
    float n = o.mat.n;
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


    Vector distance = fp - vp;
    float C = (distance.length()) + lfx.K; //Constant Distance Formula


    Vector v0(lfx.Ia.x * ka, lfx.Ia.y * ka, lfx.Ia.z * ka);
    Vector v1(lfx.Il.x / C, lfx.Il.y / C, lfx.Il.z / C); //Diffuse 
    Vector v2(lfx.Il.x / C, lfx.Il.y / C, lfx.Il.z / C); //Specular

    float s1 = Vector::dotProduct(vl, vn) * kd; //Diffuse Term
    v1 = v1 * s1;


    float s2 = pow(Vector::dotProduct(vr, vv), n) * ks; //Specular Term
    v2 = v2 * s2;


    Vout = v0 + v1 + v2;

    if (Vout.x < 0)
        Vout.x = Vout.y = Vout.z = 0;
    if (Vout.x > 1)
        Vout.x = Vout.y = Vout.z = 1;


    //cout << Vout;

    return Vout;
}

void fill_triangles(std::vector<Vector> &Ip, Object &obj, touple_t &TList, int ViewPort, int NMode, int w, int h)
{
    //project triangle into pixel space
    vpt v1, v2, v3;
    project(ViewPort, TList, v1, v2, v3, Ip);

    //IR.x = IR.y = IR.z = 0;
    //IL.x = IL.y = IL.z = 0;

    VP = ViewPort;
    N = NMode;
    gridW = w;
    gridH = h;

    if (N) {
        stepy = stepx = 3.0;
    } else
        stepy = stepx = 1.0;

    //create edges, make sure that the points are sorted
    //by their y values
    edge e1;
    if (v1.y < v2.y) {
        e1 = {v1, v2, 0, 0, 0, 0};
        e1.iL = I1.x; //left Intensity as Vertex 1
        e1.iR = I2.x; //right Intensity as Vertex 2       

    } else {
        e1 = {v2, v1, 0, 0, 0, 0};
        e1.iL = I2.x; //left Intensity as Vertex 2
        e1.iR = I1.x; //right Intensity as Vertex 1         
    }


    edge e2;
    if (v1.y < v3.y) {
        e2 = {v1, v3, 0, 0, 0, 0};
        e2.iL = I1.x; //left Intensity as Vertex 1
        e2.iR = I3.x; //right Intensity as Vertex 3      

    } else {
        e2 = {v3, v1, 0, 0, 0, 0};
        e2.iL = I3.x; //left Intensity as Vertex 3
        e2.iR = I1.x; //right Intensity as Vertex 1           

    }


    edge e3;
    if (v2.y < v3.y) {
        e3 = {v2, v3, 0, 0, 0, 0};
        e3.iL = I2.x; //left Intensity as Vertex 2
        e3.iR = I3.x; //right Intensity as Vertex 3           

    } else {
        e3 = {v3, v2, 0, 0, 0, 0};
        e3.iL = I3.x; //left Intensity as Vertex 3
        e3.iR = I2.x; //right Intensity as Vertex 2         
    }


    //    cout << "e1-1: x  I =>  " << e1.p1.x << " | " << e1.iL << "\n";
    //    cout << "e1-1: y  I =>  " << e1.p1.y << " | " << e1.iL << "\n";
    //    cout << "e1-2: x  I =>  " << e1.p2.x << " | " << e1.iR << "\n";
    //    cout << "e1-2: y  I =>  " << e1.p2.y << " | " << e1.iR << "\n";
    //    


    //intersect edges
    scanline_edges(e1, e2);
    scanline_edges(e1, e3);
    scanline_edges(e2, e3);

}

//assumes e*.p1.y <= e*.p2.y

void scanline_edges(const edge &e1, const edge &e2)
{
    if (e1.p1.y == e1.p2.y) {//parallel line test
        render_scanline(e1.p1.y, e1.p1.x, e1.p2.x, e1.iL, e1.iR);
        return;
    }
    if (e2.p1.y == e2.p2.y) {//parallel line test
        render_scanline(e2.p2.y, e2.p1.x, e2.p2.x, e2.iL, e2.iR);
        return;
    }

    float ymin = max(e1.p1.y, e2.p1.y);
    float ymax = min(e1.p2.y, e2.p2.y);

    float iL;
    float iR;
    //scan from bottom to top of edges
    for (float y = ymin; y <= ymax; y += stepy) {
        float x1, x2; //intersection points

        intersect_edge(y, e1, x1, iL, 1);
        intersect_edge(y, e2, x2, iR, 0);

        if (x1 < x2)
            render_scanline(y, x1, x2, iL, iR);
        else
            render_scanline(y, x2, x1, iR, iL);
    }

}

//assumes e1 isn't parallel to the x axis
//could do interpolation on the edge here as well and return the color intensity

bool intersect_edge(float scany, const edge& e, float &x_int, float &I, int t)
{
    if (e.p2.y == e.p1.y) return false; //parallel lines (0 or inf intersection points)
    x_int = (scany - e.p1.y)*(e.p2.x - e.p1.x) / (e.p2.y - e.p1.y) + e.p1.x;


    float delta = (e.p2.y - scany) / (e.p2.y - e.p1.y);
    I = ((e.iR * (1.0 - delta)) + (e.iL * (delta)));

    return true;
}

//could include intensity values and use those to interpolate here

void render_scanline(float y, float x1, float x2, const float &iL, const float &iR)
{

    int py = (int) floor(y);
    int px = (int) floor(x1);
    float Ic;

    for (px; px < x2; px += stepx) {

        float delta = (x2 - px) / (x2 - x1);


        if (delta < 1 || delta > 0) {
            Ic = ((iR * (1.0 - delta)) + (iL * (delta)));

            draw_pix(px, py, Ic);
        }

    }

}


//projects into pixel/framebuffer space

void project(int ViewPort, touple_t &TList, vpt &v0, vpt &v1, vpt &v2, std::vector<Vector> &Ip)
{
    v0.x = TList.p1.xyz[0][0];
    v0.y = TList.p1.xyz[1][0];
    v0.z = TList.p1.xyz[2][0];

    v1.x = TList.p2.xyz[0][0];
    v1.y = TList.p2.xyz[1][0];
    v1.z = TList.p2.xyz[2][0];

    v2.x = TList.p3.xyz[0][0];
    v2.y = TList.p3.xyz[1][0];
    v2.z = TList.p3.xyz[2][0];

    switch (ViewPort) {
    case 1:
        v0 = {v0.y, v0.z, 0.0};
        v1 = {v1.y, v1.z, 0.0};
        v2 = {v2.y, v2.z, 0.0};
        I1 = {Ip[TList.p1.id].x, Ip[TList.p1.id].z, 0};
        I2 = {Ip[TList.p2.id].x, Ip[TList.p2.id].z, 0};
        I3 = {Ip[TList.p3.id].x, Ip[TList.p3.id].z, 0};
        break;
    case 2:
        v0 = {v0.x, v0.z, 0.0};
        v1 = {v1.x, v1.z, 0.0};
        v2 = {v2.x, v2.z, 0.0};
        I1 = {Ip[TList.p1.id].x, Ip[TList.p1.id].z, 0};
        I2 = {Ip[TList.p2.id].x, Ip[TList.p2.id].z, 0};
        I3 = {Ip[TList.p3.id].x, Ip[TList.p3.id].z, 0};
        break;
    case 3:
        v0 = {v0.x, v0.y, 0.0};
        v1 = {v1.x, v1.y, 0.0};
        v2 = {v2.x, v2.y, 0.0};
        I1 = {Ip[TList.p1.id].x, Ip[TList.p1.id].y, 0};
        I2 = {Ip[TList.p2.id].x, Ip[TList.p2.id].y, 0};
        I3 = {Ip[TList.p3.id].x, Ip[TList.p3.id].y, 0};
        break;
    default:
        cout << "This shouldn't happen\n";
        exit(0);
    }


    //scale/translate into pixel/framebuffer space
    v0.x = (v0.x - min_vbox.x) / (max_vbox.x - min_vbox.x) * gridW;
    v0.y = (v0.y - min_vbox.y) / (max_vbox.y - min_vbox.y) * gridH;
    v1.x = (v1.x - min_vbox.x) / (max_vbox.x - min_vbox.x) * gridW;
    v1.y = (v1.y - min_vbox.y) / (max_vbox.y - min_vbox.y) * gridH;
    v2.x = (v2.x - min_vbox.x) / (max_vbox.x - min_vbox.x) * gridW;
    v2.y = (v2.y - min_vbox.y) / (max_vbox.y - min_vbox.y) * gridH;

}

void sortZ(vector<touple_t> TList, vector<touple_t> &sortedList)
{
    sortedList.resize(sortedList.size() + TList.size());
    int i;
    float max = TList[0].p1.xyz[2][0];

    for (i = 0; i < TList.size(); i++) {

        if (max < TList[i].p1.xyz[2][0])
            max = TList[i].p1.xyz[2][0];
        if (max < TList[i].p2.xyz[2][0])
            max = TList[i].p2.xyz[2][0];
        if (max < TList[i].p3.xyz[2][0])
            max = TList[i].p3.xyz[2][0];

        TList[i].max = max;
        max = -1;
    }//Assign Max Depth to each Set


    sort(TList.begin(), TList.end(), comparator);

    for (i = 0; i < TList.size(); i++) {

        sortedList.push_back(TList[i]);

    }//Assign Max Depth to each Set
    
    sort(sortedList.begin(), sortedList.end(), comparator);

}

void sortX(vector<touple_t> TList, vector<touple_t> &sortedList)
{
    sortedList.resize(sortedList.size() + TList.size());
    int i;
    float max = TList[0].p1.xyz[0][0];

    for (i = 0; i < TList.size(); i++) {

        if (max < TList[i].p1.xyz[0][0])
            max = TList[i].p1.xyz[0][0];
        if (max < TList[i].p2.xyz[0][0])
            max = TList[i].p2.xyz[0][0];
        if (max < TList[i].p3.xyz[0][0])
            max = TList[i].p3.xyz[0][0];

        TList[i].max = max;
        max = -1;
    }//Assign Max Depth to each Set


    sort(TList.begin(), TList.end(), comparator);

    for (i = 0; i < TList.size(); i++) {

        sortedList.push_back(TList[i]);

    }//Assign Max Depth to each Set
    
    sort(sortedList.begin(), sortedList.end(), comparator);

}

void sortY(vector<touple_t> TList, vector<touple_t> &sortedList)
{
    sortedList.resize(sortedList.size() + TList.size());
    int i;
    float max = TList[0].p1.xyz[1][0];

    for (i = 0; i < TList.size(); i++) {

        if (max < TList[i].p1.xyz[1][0])
            max = TList[i].p1.xyz[1][0];
        if (max < TList[i].p2.xyz[1][0])
            max = TList[i].p2.xyz[1][0];
        if (max < TList[i].p3.xyz[1][0])
            max = TList[i].p3.xyz[1][0];

        TList[i].max = max;
        max = -1;
    }//Assign Max Depth to each Set


    sort(TList.begin(), TList.end(), comparator);

    for (i = 0; i < TList.size(); i++) {

        sortedList.push_back(TList[i]);

    }//Assign Max Depth to each Set
    
    sort(sortedList.begin(), sortedList.end(), comparator);

}

bool comparator(touple_t i, touple_t j)
{

    if (i.max < j.max)
        return true;

    return false;
}
