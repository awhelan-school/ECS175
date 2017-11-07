#ifndef FILEIO_H
#define FILEIO_H

#include "data.h"

#include <iostream>
#include <fstream>


void readFile(std::vector<Object> &vec);
void writeFile(std::vector<Object> &vec);

#endif /* FILEIO_H */

