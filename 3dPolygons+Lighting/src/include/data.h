#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cmath>

typedef struct point{
    //Colum Major
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
    unsigned int vertices = 0;
    Point center;
    std::vector<Point> VList;
    std::vector<std::pair<int, int>> EMap;
    std::vector<std::pair<Point, Point>> EList;
}Object;

Point calculateCenter(std::vector<Point> VList);
        
#endif /* DATA_H */

