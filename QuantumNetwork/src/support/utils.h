/*
 * AuxilaryFunctions.h
 *
 *  Created on: 28 ��� 2018 �.
 *      Author: yuriy
 */

#ifndef UTILS_H_
#define UTILS_H_
#include <iostream>
#include <fstream>
#include "../params.h"
#include <iomanip>
#include <sys/stat.h>
#include <vector>
#include "qpp.h"


/*
 * Checks if required folder exist, if they don't
 * creates directories
 */
void checkFolders();
void checkAndCreateFolder(const char* pathname);
/*
 * Function loads the contents of targetFunction.txt as an input and encodes it as vector of ket vectors
 */
void loadFunction(std::vector<std::vector<qpp::ket>>&,std::vector<std::vector<qpp::ket>>&);

#endif /* UTILS_H_ */
