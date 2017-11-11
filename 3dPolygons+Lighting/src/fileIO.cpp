#include "fileIO.h"

vector<Material> Mat;

std::string inputFile("./input/test_objs.txt");

void readMaterials(){
    std::ifstream materials("./input/raw/raw_materials.txt", std::ios::in);
    int token;
    
    
    if(materials.is_open()){
       // Get # of Materials
       materials >> token;
       Mat.resize(token);
       
       for(int k = 0; k < token; k++){
           materials >> Mat[k].Ka.x >> Mat[k].Ka.y >> Mat[k].Ka.z;
           materials >> Mat[k].Kd.x >> Mat[k].Kd.y >> Mat[k].Kd.z;
           materials >> Mat[k].Ks.x >> Mat[k].Ks.y >> Mat[k].Ks.z;
           materials >> Mat[k].n;
       }
        
    }
    else{
        
        cout << "Unable to Open File: Materials\n";
    }
    materials.close();
    
}

void readLight(Light &lfx){
    
    std::ifstream lightfx("./input/raw/raw_light.txt", std::ios::in);
    
    if(lightfx.is_open()){
        
        lightfx >> lfx.src.x >> lfx.src.y >> lfx.src.z;
        lightfx >> lfx.Ia.x  >> lfx.Ia.y  >> lfx.Ia.z;
        lightfx >> lfx.Il.x  >> lfx.Il.y  >> lfx.Il.z;
        lightfx >> lfx.f;
        lightfx >> lfx.K;
    }
    else{
        cout << "Unable to open File LightFX\n";
    }
    lightfx.close();
}

void readFile(std::vector<Object> &vec, Light &lfx) {

    vec.clear();

    getRaw();
    std::ifstream infile("./input/raw/raw_test_objs.txt", std::ios::in);
    readLight(lfx);
    readMaterials();
    
    std::string token;
    unsigned int objects = 0, vertices = 0, edges = 0;
    unsigned int p1 = 0, p2 = 0, p3 = 0;

    unsigned int i = 0, j = 0, k = 0;

    if (infile.is_open()) {

        infile >> token;
        objects = std::stoi(token);

        //Allocate for Objects
        vec.resize(objects);

        for (i = 0; i < objects; i++) {

            //GET MaterialID
            infile >> vec[i].materialID;
            vec[i].mat = Mat[vec[i].materialID - 1];

            infile >> vertices;
            //Allocate Vertex List
            vec[i].vertices = vertices;
            vec[i].VList.resize(vertices);

            //Allocate for Normal Vectors for Each Vertex
            vec[i].NormVecList.resize(vertices);

            for (j = 0; j < vertices; j++) {
                infile >> vec[i].VList[j].xyz[0][0]; //x-coordinate
                infile >> vec[i].VList[j].xyz[1][0]; //y-coordinate
                infile >> vec[i].VList[j].xyz[2][0]; //z-coordinate
                vec[i].VList[j].id = j;
            }//ScanVertices

            vec[i].center = calculateCenter(vec[i].VList);

            //EdgeCount
            infile >> edges;
            vec[i].TList.resize(edges);

            for (k = 0; k < edges; k++) {
                infile >> p1 >> p2 >> p3;
                vec[i].TList[k].p1 = vec[i].VList[p1 - 1];
                vec[i].TList[k].p1.id = p1 - 1;
                vec[i].TList[k].p2 = vec[i].VList[p2 - 1];
                vec[i].TList[k].p2.id = p2 - 1;
                vec[i].TList[k].p3 = vec[i].VList[p3 - 1];
                vec[i].TList[k].p3.id = p3 - 1;
            }//Make_Edges    
            
            //Calculate Normals
            calculateNormalV(vec[i]);            
            
            
        }//scanObjects    
    } else {
        std::cout << "Unable to Open File " << inputFile << std::endl;
    }
    infile.close();
}//ReadData

void writeFile(std::vector<Object> &vec) {


    std::ofstream outfile(inputFile, std::ios::trunc);
    std::string token;

    unsigned int i = 0, j = 0, k = 0;

    if (outfile.is_open()) {

        //Write Objects Count
        outfile << vec.size() << "\n";

        for (i = 0; i < vec.size(); i++) {

            outfile << vec[i].VList.size() << "\n";

            for (j = 0; j < vec[i].VList.size(); j++) {
                outfile << vec[i].VList[j].xyz[0][0] << " "; //x-coordinate
                outfile << vec[i].VList[j].xyz[1][0] << " "; //y-coordinate
                outfile << vec[i].VList[j].xyz[2][0] << "\n"; //z-coordinate
            }//ScanVertices
            outfile << "\n";

            //EdgeCount
            outfile << vec[i].TList.size() << "\n";

            for (k = 0; k < vec[i].TList.size(); k++) {

                outfile << vec[i].TList[k].p1.id << " "
                        << vec[i].TList[k].p2.id << " "
                        << vec[i].TList[k].p3.id << "\n";

            }//Make_Edges
            outfile << "\n";

        }//scanObjects

    } else {
        std::cout << "Unable to Open File " << inputFile << std::endl;
    }
    outfile.close();
}//WriteData

void getRaw() {

    std::ofstream outfile("./input/raw/raw_test_objs.txt", std::ios::trunc);
    std::ifstream infile(inputFile, std::ios::in);
    std::string line;

    while (std::getline(infile, line)) {

        line.erase(std::find(line.begin(), line.end(), '#'), line.end());
        outfile << line << "\n";
    }//For All Lines in Object File Filter the Comments 

    std::ofstream outfile2("./input/raw/raw_light.txt", std::ios::trunc);
    std::ifstream infile2("./input/light.txt", std::ios::in);

    while (std::getline(infile2, line)) {

        line.erase(std::find(line.begin(), line.end(), '#'), line.end());
        outfile2 << line << "\n";
    }//For All Lines in Light File Filter the Comments 
    
    std::ofstream outfile3("./input/raw/raw_materials.txt", std::ios::trunc);
    std::ifstream infile3("./input/materials.txt", std::ios::in);

    while (std::getline(infile3, line)) {

        line.erase(std::find(line.begin(), line.end(), '#'), line.end());
        outfile3 << line << "\n";
    }//For All Lines in Light File Filter the Comments 

}

