/*
 * Node.cpp
 *
 *  Created on: May 22, 2018
 *      Author: george
 */

#include "Node.h"

Node::Node(std::string nodeName, int inputConnectionsCount) {
	for (int i = 0; i < inputConnectionsCount; i++) {
		qpp::ket t = qpp::randket(1);
		weights.push_back(t);

	}
	saveToCSVFile(nodeName);
	saveToInStateFile(nodeName);
}
void Node::saveToCSVFile(std::string nodeName) {
	std::string fileName = nodeName + extension;
	std::ofstream outFile;
	outFile.open(fileName);
	if (!outFile) {
		std::cerr << "Unable to create file " + fileName;
		exit(1); // terminate with error
	}
	for (unsigned int i = 0; i < weights.size(); i++) {
		outFile << weights.at(i).real()<<","<< weights.at(i).imag()<<std::endl;
	}
	outFile.close();
}
void Node::saveToInStateFile(std::string nodeName) {
	std::string fileName = nodeName + origState;
	std::ofstream outFile(fileName);
	if (!outFile) {
		std::cerr << "Unable to create file " + fileName;
		exit(1); // terminate with error
	}
	for (unsigned int i = 0; i < weights.size(); i++) {
		outFile << weights.at(i).real()<<","<< weights.at(i).imag()<<std::endl;
	}
	outFile.close();
}
Node::Node(std::string nodeName) {
	loadFromCSVFile(nodeName);
}
void Node::loadFromCSVFile(std::string nodeName) {
	std::string fileName = nodeName + extension;
	std::ifstream inFile(fileName);
	if (!inFile) {
		std::cerr << "Unable to open file " + fileName;
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
		temp.push_back(std::stod(line));
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
	std::cout << nodeName << " loaded successfully" << std::endl;
}

void Node::propagateWithInputsAndGenerateOutput(std::vector<qpp::ket>& inputs) {
	std::vector<qpp::ket> inputsPairedWithWeightQubits;
	if (inputs.size() != weights.size()) {
		std::cerr
				<< "Inputs function does not match selected vectors, please select another one or train new!";
		exit(-1);
	}
	std::cout.precision(20);
	unsigned int counter, size = weights.size();
	//Apply Kronecker product on qubit and weights to generate inputs paired with weight qubits
	for (counter = 0; counter < size; counter++) {
		inputsPairedWithWeightQubits.push_back(
				qpp::kron(inputs.at(counter), weights.at(counter)));
	}
	//Debug
//	for (auto& it : inputsPairedWithWeightQubits)
//		std::cout << std::endl << "Inputs paired with qubits are" << std::endl
//				<< qpp::disp(it) << std::endl;

	//entangle the inputs paired with weight qubits by applying CNOT
	qpp::cmat u = qpp::gt.CNOT;
	for (counter = 0; counter < size; counter++) {
		inputsPairedWithWeightQubits.at(counter) = u
				* inputsPairedWithWeightQubits.at(counter);
	}

	//Debug
//	for (auto& it : inputsPairedWithWeightQubits)
//		std::cout << std::endl << "Inputs paired with qubits after CNOT are"
//				<< std::endl << qpp::disp(it) << std::endl;

	qpp::ket output = qpp::kron(inputsPairedWithWeightQubits.at(0),
			inputsPairedWithWeightQubits.at(1));
	for (unsigned int i = 2; i < size; i++) {
		output = qpp::kron(output, inputsPairedWithWeightQubits.at(i));
	}
	for (unsigned int i = 2; i < size * 2; i += 2) {
		int firstIndexSWAP = i - 1;
		int firstDimSWAP = 1 << firstIndexSWAP;
		int secondDimSWAP = 1 << (size * 2 - firstIndexSWAP - 2);
		int firstIndexCNOT = i;
		int firstDimCNOT = 1 << firstIndexCNOT;
		int secondDimCNOT = 1 << (size * 2 - firstIndexCNOT - 2);

//		std::cout<<"SECOND DIM CNOT IS:"<<secondDimCNOT<<std::endl;

		qpp::cmat firstPart, thirdPart, expandedSwap, expandedCNOT, finalGate;

		firstPart = qpp::cmat::Identity(firstDimSWAP, firstDimSWAP);
		thirdPart = qpp::cmat::Identity(secondDimSWAP, secondDimSWAP);
		expandedSwap = qpp::kron(qpp::kron(firstPart, qpp::gt.SWAP), thirdPart);

		firstPart = qpp::cmat::Identity(firstDimCNOT, firstDimCNOT);
		thirdPart = qpp::cmat::Identity(secondDimCNOT, secondDimCNOT);
		expandedCNOT = qpp::kron(qpp::kron(firstPart, qpp::gt.CNOTba),
				thirdPart);
		finalGate = expandedSwap * expandedCNOT * expandedSwap;
		output = finalGate * output;

		//Debug
//		std::cout << std::endl << "EXPANDED SWAP" << qpp::disp(expandedSwap)
//				<< std::endl;
//		std::cout << std::endl << "EXPANDED CNOT" << qpp::disp(expandedCNOT)
//				<< std::endl;
//		std::cout << std::endl << "Final Gate" << qpp::disp(finalGate)
//				<< std::endl;

	}
	measureTheOutputState(output);
}
void Node::measureTheOutputState(qpp::ket& stateToMeasure) {
	qpp::ket result = stateToMeasure;
//	std::cout << "hi";
	unsigned int size = (int) (1.0 * log(result.size()) / log(2));

	for (unsigned int i = 0; i < size / 2; i++) {
//		std::cout << i << std::endl << qpp::disp(result) << " end";
		auto res = qpp::measure(result, qpp::gt.Z, { i });
//		std::cout << std::endl << "Here " << i << std::endl;
		result = selectStateOnProbability(res);
	}

	size = (int) (1.0 * log(result.size()) / log(2));
//	std::cout << "hello" << size << std::endl;
	for (unsigned int i = 0; i < size - 1; i++) {
		auto res = qpp::measure(result, qpp::gt.Z, { 0 });
		result = selectStateOnProbability(res);
	}
//	std::cout << std::endl << qpp::disp(result);
	outputQubit = result;
}
qpp::ket Node::getOutputQubit() {
	return outputQubit;
}
qpp::ket Node::selectStateOnProbability(
		std::tuple<qpp::idx, std::vector<double>, std::vector<qpp::cmat>>& resultOfMeasurement) {
	std::vector<double> probs = std::get<1>(resultOfMeasurement);
	int selection[probs.size()];
	for (unsigned int i = 0; i < probs.size(); i++) {
		selection[i] = 0;
	}
//	std::cout << std::endl << "prob distribution is" << probs.at(0) << ","
//			<< probs.at(1) << std::endl;

	for (int i = 0; i < numberOfMeasurements; i++) {
		double mres = qpp::rand(0.0, 1.0);
		double r = 0;
		unsigned int idx = 0;

		while (mres > r) {
			if (idx + 1 == probs.size())
				break;
			r = r + probs.at(idx);
			idx = idx + 1;

//			std::cout << "r is:" << r << std::endl;
//			std::cout << "mres is:" << mres << std::endl;
//			std::cout << "idx is:" << idx << std::endl;
		}
		selection[idx]++;
	}
//	std::cout << "selection is:" << selection[0]<<","<< selection[1] << std::endl;
	int max = 0;
	int maxI = 0;
	for (unsigned int i = 0; i < probs.size(); i++) {
		if (max < selection[i]) {
			max = selection[i];
			maxI = i;
		}
	}
	return std::get<2>(resultOfMeasurement).at(maxI);

}
std::ostream& operator <<(std::ostream& stream, const Node& node) {
	stream << std::endl << "Node weights are:" << std::endl;
	for (auto it : node.weights)
		stream << "State:" << std::endl << qpp::disp(it) << std::endl;
	return stream;
}

Node::~Node() {
}

