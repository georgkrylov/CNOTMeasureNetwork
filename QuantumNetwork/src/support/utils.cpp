/*
 * AuxilaryFunctions.cpp
 *
 *  Created on: 28 ��� 2018 �.
 *      Author: yuriy
 */
#include "utils.h"

void checkAndCreateFolder(const char* pathname) {
	struct stat info;
	if (stat(pathname, &info) != 0) {
		const int dir_err = mkdir(pathname,
		S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (-1 == dir_err) {
			printf("Error creating directory!n");
			exit(1);
		}
		std::cout << "Folder " << pathname << " was successfully created"
				<< std::endl;
	} else if (info.st_mode & S_IFDIR) {
		std::cout << "Folder " << pathname << " exists" << std::endl;
	}
}
void checkFolders() {
	checkAndCreateFolder(weightsFolder);
}

void loadFunction(std::vector<std::vector<qpp::ket>>& inputs,
		std::vector<std::vector<qpp::ket>>& outputs) {
	std::ifstream inFile;
	inFile.open("targetFunction.txt");
	if (!inFile) {
		std::cout << "Unable to open file";
		exit(1); // terminate with error
	}
	int inputsCount;
	int outputsCount;
	inFile >> inputsCount;
	inFile >> outputsCount;
	int x;
	//std::cout << inputsCount << " :inputs; outputs:" << outputsCount << std::endl;
	int entriesCount = inputsCount + outputsCount;
	std::vector<int> tmp;
	int counter = 0;
	char delim;
	while (inFile >> x) {
		if (counter == (1 << inputsCount * entriesCount))
			std::cerr << "File contains weakly specified function";
		counter++;
		inFile >> delim;
		tmp.push_back(x);
		if (counter % entriesCount == 0 && counter != 0) {
			qpp::ket tempState;
			if (tmp[0])
				tempState = qpp::st.z1;
			else
				tempState = qpp::st.z0;

			for (int i = 1; i < inputsCount; i++)
				if (tmp[i])
					tempState = qpp::kron(tempState, qpp::st.z1);
				else
					tempState = qpp::kron(tempState, qpp::st.z0);
			inputs.push_back(tempState);
			if (tmp[inputsCount])
				tempState = qpp::st.z1;
			else
				tempState = qpp::st.z0;

			for (int i = inputsCount+1; i < entriesCount; i++)
				if (tmp[i])
					tempState = qpp::kron(tempState, qpp::st.z1);
				else
					tempState = qpp::kron(tempState, qpp::st.z0);
			outputs.push_back(tempState);
			tmp.clear();
		}
	}
}
