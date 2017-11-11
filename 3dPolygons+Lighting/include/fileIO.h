#ifndef FILEIO_H
#define FILEIO_H

#include "data.h"

#include <iostream>
#include <fstream>


void readFile(std::vector<Object> &vec, Light &lfx);
void readLight(Light &lfx);
void readMaterials();
void writeFile(std::vector<Object> &vec);
void getRaw();

#endif /* FILEIO_H */

