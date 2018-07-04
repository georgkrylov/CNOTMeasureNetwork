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
Node::Node(const char* nodeName) {
	loadFromCSVFile(nodeName);
}
void Node::loadFromCSVFile(std::string nodeName) {
	std::string fileName = nodeName + ".cls";
	std::ifstream inFile(fileName);
	if (!inFile) {
		std::cerr << "Unable to open file";
		exit(1); // terminate with error
	}
	std::string line;
	std::string token;
	std::vector<double> temp;
	std::string delimiter = ",";
	while (std::getline(inFile, line)) {
		size_t pos;
		temp.clear();
		while ((pos = line.find(delimiter)) != std::string::npos) {
			token = line.substr(0, pos);
			temp.push_back(std::stod(token));
			line.erase(0, pos + delimiter.length());
		}
		qpp::cplx t = { temp.at(0), 0 };
		qpp::cplx u = { temp.at(1), 0 };
		qpp::ket tmp(2);
		tmp << t, u;
		if (qpp::abssq(tmp).front() - 1 > qpp::eps) {
			std::cerr
					<< "File has wrong, contains weights that are not unitary!";
			exit(-1);
		}
		weights.push_back(tmp);
	}
}

void Node::propagateWithInputsAndGenerateOutput(std::vector<qpp::ket>& inputs,
		qpp::ket& output) {
	std::vector<qpp::ket> inputsPairedWithWeightQubits;
	std::cout.precision(20);
	unsigned int counter, size = weights.size();
	//Apply Kronecker product on qubit and weights to generate inputs paired with weight qubits
	for (counter = 0; counter < size; counter++) {
		inputsPairedWithWeightQubits.push_back(
				qpp::kron(inputs.at(counter), weights.at(counter)));
	}
	for (auto& it : inputsPairedWithWeightQubits)
		std::cout << std::endl << "Inputs paired with qubits are" << std::endl
				<< qpp::disp(it) << std::endl;
	//entangle the inputs paired with weight qubits by applying CNOT
	qpp::cmat u = qpp::gt.CNOT;
	for (counter = 0; counter < size; counter++) {
		inputsPairedWithWeightQubits.at(counter) = u
				* inputsPairedWithWeightQubits.at(counter);
	}
	for (auto& it : inputsPairedWithWeightQubits)
		std::cout << std::endl << "Inputs paired with qubits are" << std::endl
				<< qpp::disp(it) << std::endl;
}
std::ostream& operator <<(std::ostream& stream, const Node& node) {
	std::cout.precision(12);
	stream << std::endl << "Node weights are:" << std::endl;
	for (auto it : node.weights)
		stream << "State:" << std::endl << qpp::disp(it) << std::endl;
	return stream;
}

Node::~Node() {
// TODO Auto-generated destructor stub
}

