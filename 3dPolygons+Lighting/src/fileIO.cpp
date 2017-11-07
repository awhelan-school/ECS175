#include "fileIO.h"

std::string inputFile("./input/test_scene");

void readFile(std::vector<Object> &vec) {

    vec.clear();
    
    std::ifstream infile(inputFile, std::ios::in);
    std::string token;
    unsigned int objects = 0, vertices = 0, edges = 0;
    unsigned int p0 = 0, p1 = 0;

    unsigned int i = 0, j = 0, k = 0;

    if (infile.is_open()) {

        infile >> token;     
        objects = std::stoi(token);
        
        //Allocate for Objects
        vec.resize(objects);

        for (i = 0; i < objects; i++) {

            infile >> vertices;

            //Allocate Vertex List
            vec[i].vertices = vertices;
            vec[i].VList.resize(vertices);

            for (j = 0; j < vertices; j++) {
                infile >> vec[i].VList[j].xyz[0][0];//x-coordinate
                infile >> vec[i].VList[j].xyz[1][0];//y-coordinate
                infile >> vec[i].VList[j].xyz[2][0];//z-coordinate
            }//ScanVertices
            
            vec[i].center = calculateCenter(vec[i].VList);
            
            //EdgeCount
            infile >> edges;
            vec[i].EList.resize(edges);
            vec[i].EMap.resize(edges);
            
            for(k = 0; k < edges; k++){
                infile >> p0 >> p1;
                       
                std::pair<Point, Point> p = {vec[i].VList[p0-1], vec[i].VList[p1-1]};
                std::pair<int , int> m = {p0-1, p1-1};
                vec[i].EMap[k] = m;
                vec[i].EList[k] = p;
                
            }//Make_Edges            
        }//scanObjects

    } else {
        std::cout << "Unable to Open File " << inputFile << std::endl;
    }
    infile.close();
}//ReadData

void writeFile(std::vector<Object> &vec) {

    
    std::ofstream outfile(inputFile, std::ios::trunc);
    std::string token;
    unsigned int objects = 0, vertices = 0, edges = 0;
    unsigned int p0 = 0, p1 = 0;

    unsigned int i = 0, j = 0, k = 0;

    if (outfile.is_open()) {

        //Write Objects Count
        outfile << vec.size() << "\n";     

        for (i = 0; i < vec.size(); i++) {

            outfile << vec[i].VList.size() << "\n";

            for (j = 0; j < vec[i].VList.size(); j++) {
                outfile << vec[i].VList[j].xyz[0][0] << " ";//x-coordinate
                outfile << vec[i].VList[j].xyz[1][0] << " ";//y-coordinate
                outfile << vec[i].VList[j].xyz[2][0] << "\n";//z-coordinate
            }//ScanVertices
            outfile << "\n";
            
            //EdgeCount
            outfile << vec[i].EList.size() << "\n";
            
            for(k = 0; k < vec[i].EList.size(); k++){
                
                outfile << vec[i].EMap[k].first+1 << " " << vec[i].EMap[k].second+1 << "\n";                   
               
            }//Make_Edges
            outfile << "\n";
            
        }//scanObjects

    } else {
        std::cout << "Unable to Open File " << inputFile << std::endl;
    }
    outfile.close();
}//WriteData

