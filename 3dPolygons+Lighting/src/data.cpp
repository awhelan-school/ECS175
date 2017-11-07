#include "data.h"


Point calculateCenter(std::vector<Point> VList){
    
    Point center;
    for(int i = 0; i < VList.size(); i++){
        
        center.xyz[0][0] += VList[i].xyz[0][0] / VList.size();
        center.xyz[1][0] += VList[i].xyz[1][0] / VList.size();
        center.xyz[2][0] += VList[i].xyz[2][0] / VList.size();
    }
    
    return center;
} 
