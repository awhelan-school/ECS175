#include "data.h"

using namespace std;

Point calculateCenter(std::vector<Point> VList) {

    Point center;
    for (int i = 0; i < VList.size(); i++) {

        center.xyz[0][0] += VList[i].xyz[0][0] / VList.size();
        center.xyz[1][0] += VList[i].xyz[1][0] / VList.size();
        center.xyz[2][0] += VList[i].xyz[2][0] / VList.size();
    }
    
    return center;
}

void UpdateTList(std::vector<Object> &o, int &OID) {
    int p1=0, p2=0, p3=0;   
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

void calculateNormalV(Object &o){
    
    Vector avgNormal(0,0,0);
    int k = 0; // Adjacent Vectors
    
    for(int i = 0; i < o.VList.size(); i++){
        
        for(int j = 0; j < o.TList.size(); j++){
            
            if(o.VList[i].id == o.TList[j].p1.id || o.VList[i].id == o.TList[j].p2.id || o.VList[i].id == o.TList[j].p3.id){
                
                k++;
                Point p0 = o.VList[o.TList[j].p1.id];
                Point p1 = o.VList[o.TList[j].p2.id];
                Point p2 = o.VList[o.TList[j].p3.id];
                
                
                Vector v0(p0.xyz[0][0], p0.xyz[1][0], p0.xyz[2][0]);
                Vector v1(p1.xyz[0][0], p1.xyz[1][0], p1.xyz[2][0]);
                Vector v2(p2.xyz[0][0], p2.xyz[1][0], p2.xyz[2][0]);
                
                Vector N = v1-v0;
                Vector M = v2-v0;
                             
                Vector vN = Vector::crossProduct( N, M );
                
                if(vN.length())
                    Vector::normalize(vN);
                
                if(k == 1)
                    avgNormal = avgNormal + vN;
                
                            
                if(Vector::dotProduct(vN,avgNormal) >= 0){
                    
                    float nL = vN.length();
                    
                    if(nL > 0){
                       avgNormal = avgNormal + vN;
                       avgNormal = avgNormal / nL; 
                       avgNormal = avgNormal / k; // Average Adjacent Normals
                    }
                             
                }else{
                                      
                    vN.x *= -1;
                    vN.y *= -1;
                    vN.z *= -1;
                    float nL = vN.length();
                    
                    if(nL > 0){
                        avgNormal = avgNormal + vN;
                        avgNormal = avgNormal / nL;  
                        avgNormal = avgNormal / k; // Average Adjacent Normals
                    }
                                     
                }
      
            }//If Vertex is element of Triangle Edge          
        }//In each Triangle Tuple
        
        k = 0;// Reset for New Vertex
        o.NormVecList[i] = avgNormal;      
        avgNormal.x = avgNormal.y = avgNormal.z = 0;
        
    }//For All vertices
}//Calculate Normal Vectors