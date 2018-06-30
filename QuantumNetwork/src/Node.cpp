/*
 * Node.cpp
 *
 *  Created on: May 22, 2018
 *      Author: george
 */

#include "Node.h"

Node::Node(int inputConnectionsCount, int outputConnectionsCount) {
	// TODO Auto-generated constructor stub

}
Node::Node(char* nodeName) {

}
void Node::loadFromFile(char* nodeName) {
	std::ifstream inFile;
	char* fileName = strcat(nodeName, ".cls");
	inFile.open(fileName);
	if (!inFile) {
		std::cout << "Unable to open file";
		exit(1); // terminate with error
	}
	double real, imag, update;
	char delim;
	while (inFile >> real >> delim >> imag >> delim >> update) {
		qpp::cplx t = { real, 0 };
		qpp::cplx u = { imag, 0 };
		qpp::ket tmp;
		tmp << t, u;
		if (qpp::abssq(tmp).front() - 1 > qpp::eps) {
			std::cerr << "File has wrong weights that are not unitary!";
			exit(-1);
		}
		weights.push_back(tmp);
	}
}

Node::~Node() {
	// TODO Auto-generated destructor stub
}

