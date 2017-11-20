#ifndef FILEIO_H
#define FILEIO_H

#include "data.h"

#include <iostream>
#include <fstream>


void readFile(std::vector<Object> &vec, Light &lfx, vector<Material> &mat);
void readLight(Light &lfx);
void readMaterials(vector<Material> &mat);
void writeFile(std::vector<Object> &vec);
void getRaw();

#endif /* FILEIO_H */

