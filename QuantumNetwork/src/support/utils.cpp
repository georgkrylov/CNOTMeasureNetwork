/*
 * AuxilaryFunctions.cpp
 *
 *  Created on: 28 ��� 2018 �.
 *      Author: yuriy
 */
#include "utils.h"

void checkAndCreateFolder(std::string pathname) {
	struct stat info;
	if (stat(pathname.c_str(), &info) != 0) {
		const int dir_err = mkdir(pathname.c_str(),
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
void checkFolder(std::string pathname) {
	struct stat info;
	if (stat(pathname.c_str(), &info) != 0) {
		std::cout << "Folder " << pathname << " does not exist" << std::endl;
		exit(1);
	} else if (info.st_mode & S_IFDIR) {
		std::cout << "Folder " << pathname << " exists" << std::endl;
	}
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
			std::vector<qpp::ket> tempStates;
			for (int i = 0; i < inputsCount; i++)
				if (tmp[i])
					tempStates.push_back(qpp::st.z1);
				else
					tempStates.push_back(qpp::st.z0);
			inputs.push_back(tempStates);
			tempStates.clear();
			for (int i = inputsCount; i < entriesCount; i++)
				if (tmp[i])
					tempStates.push_back(qpp::st.z1);
				else
					tempStates.push_back(qpp::st.z0);
			outputs.push_back(tempStates);
			tmp.clear();
		}
	}

}
int loadDescription(std::string path) {
	std::ifstream inFile;
	std::string t = path + descriptionFile;
	inFile.open(t);
	if (!inFile) {
		std::cout << "Unable to open file" << t;
		exit(1); // terminate with error
	}
	int result;
	inFile >> result;
	return result;
}
void saveDescription(std::string path, int size) {
	std::ofstream outFile;
	std::string t = path + descriptionFile;
	outFile.open(t);
	if (!outFile) {
		std::cout << "Unable to open file" << t;
		exit(1); // terminate with error
	}
	outFile << size;
	outFile.close();
}

bool checkZeroVector(qpp::ket& res) {
	double sum = 0.0;
	std::vector<double> vec = qpp::abssq(res);
	for (double it : vec)
		sum = sum + it;

	return sum < qpp::eps;

}
