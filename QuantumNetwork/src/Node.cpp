/*
 * Node.cpp
 *
 *  Created on: May 22, 2018
 *      Author: george
 */

#include "Node.h"

Node::Node(std::string nodeName, int inputConnectionsCount) {
	historyData = std::vector<double>(inputConnectionsCount);
	for (int i = 0; i < inputConnectionsCount; i++) {
		historyData[i] = qpp::rand(0.0, 1.0);
		weights.push_back(makeExp(historyData.at(i)));
	}
	saveToCSVFile(nodeName);
	saveToInStateFile(nodeName);
	logFault = 0.0;
	name = nodeName;
}
qpp::ket Node::makeExp(double h) {
	qpp::cplx t = { h, 0 };
	t = t * qpp::pi;
	qpp::cplx t2 = { 0, -2 };
	t = t * t2;
	t = std::exp(t);
	qpp::ket result(2);
	result << t.real(), t.imag();
	return result;
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
		outFile << weights.at(i).real().row(0) << ","
				<< weights.at(i).real().row(1) << "," << historyData[i]
				<< std::endl;
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
		outFile << weights.at(i).real().row(0) << ","
				<< weights.at(i).real().row(1) << "," << historyData[i]
				<< std::endl;
	}
	outFile.close();
}
Node::Node(std::string nodeName) {
	logFault = 0.0;
	loadFromCSVFile(nodeName);
}
void Node::loadFromCSVFile(std::string nodeName) {
	name = nodeName;
	std::string fileName = nodeName + extension;
	std::ifstream inFile(fileName);
	if (!inFile) {
		std::cerr << "Unable to open file " + fileName;
		exit(1); // terminate with error
	}
	std::string line;
	std::string token;
	std::vector<double> temp;
	historyData.clear();
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
		historyData.push_back(temp.at(2));
	}
//	std::cout << qpp::disp(weights[0]) << std::endl;
//	std::cout << "history data is:" << historyData[0] << std::endl;
//	std::cout << nodeName << " loaded successfully" << std::endl;
}

void Node::propagateWithInputsAndGenerateOutput(std::vector<qpp::ket>& inputs) {
	std::vector<qpp::ket> inputsPairedWithWeightQubits;
	if (inputs.size() != weights.size()) {
		std::cerr
				<< "Inputs function does not match selected vectors, please select another one or train new!";
		exit(-1);
	}
//	std::cout.precision(20);
	unsigned int counter, size = weights.size();
//Apply Kronecker product on qubit and weights to generate inputs paired with weight qubits
	for (counter = 0; counter < size; counter++) {
		inputsPairedWithWeightQubits.push_back(
				qpp::kron(weights.at(counter), inputs.at(counter)));
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
//	std::cout <<std::endl<<"Measuring the control qubit "<< 1 << std::endl << qpp::disp(result) <<std::endl<< " end";
	for (unsigned int i = 0; i < size / 2; i++) {

		auto res = qpp::measure(result, qpp::gt.Z, { i });
//		std::cout << std::endl << "Here " << i << std::endl;
		result = selectStateOnProbability(res);
		//	std::cout <<std::endl<<"Measuring the control qubit "<< i << std::endl << qpp::disp(result) <<std::endl<< " end";
	}

	size = (int) (1.0 * log(result.size()) / log(2));
//	std::cout << "hello" << size << std::endl;
	for (unsigned int i = 0; i < size - 1; i++) {
		auto res = qpp::measure(result, qpp::gt.Z, { 0 });

		result = selectStateOnProbability(res);
//		std::cout <<std::endl<<"Measuring the target qubit "<< i << std::endl << qpp::disp(result) <<std::endl<< " end";
	}
//	std::cout << std::endl << qpp::disp(result);
	outputQubit = result;
}
qpp::ket Node::getOutputQubit() {
	return outputQubit;
}
void Node::updateWeights(std::vector<std::vector<qpp::ket>>& output,
		double learningRate) {
	for (std::vector<qpp::ket> out : output) {
		std::vector<qpp::cplx> errors = getErrorPerOutputVariable(out);
		double delta;
		if (logFault != 0) {
			for (unsigned int i = 0; i < historyData.size(); i++) {

				//square
				delta = calculateDelta(errors, i);

//		std::cout << "History data for node" << name << ", weight number " << i
//				<< " is: " << historyData[i] << std::endl;
				historyData[i] = historyData[i] + learningRate * delta;
//
//		std::cout << "History data for node" << name << ", weight number " << i
//				<< " after change is: " << historyData[i] << std::endl;
				if (historyData[i] > 1) {
					historyData[i] = historyData[i] - 1;
				} else if (historyData[i] < 0) {
					historyData[i] = 1 - historyData[i];
				}
				weights[i] = makeExp(historyData.at(i));
//		weights[i] = makeExp(0.3);
			}
		}
	}
}
double Node::calculateDelta(std::vector<qpp::cplx>& errors, int i) {
	//Here, errors are per output, so size of this vector is 1 for one output functions.
	qpp::ket q = weights[i];

	double w = weights.at(i).real()[0];
	double t = errors[0].real() - w;
//	std::cout << "Weights are" << std::endl << qpp::disp(q) << std::endl
//			<< "errors are " << errors[0] << std::endl << " delta is"
//			<< std::endl << t << std::endl;
	return t;
}
double Node::getLogError(std::vector<qpp::ket>& output) {
	std::vector<qpp::cplx> errors = getErrorPerOutputVariable(output);
	logFault = 0;
	for (qpp::cplx t : errors) {
		double real = t.real();
		double complex = t.imag();
		logFault = logFault + std::sqrt(real * real + complex * complex);
	}

	logFault = logFault / 2;
//	std::cout << "Log Faults are" << logFault << std::endl;
//	std::cout << "Errors are" << std::endl << qpp::disp(errors[0]) << std::endl
//			<< " Error size is" << errors.size() << std::endl;
	return logFault;
}
std::vector<qpp::cplx> Node::getErrorPerOutputVariable(
		std::vector<qpp::ket>& output) {
	std::vector<qpp::cplx> result;

	for (unsigned int i = 0; i < output.size(); i++) {
		qpp::cplx mesResult = { selection[0] * 1.0 / numberOfMeasurements,
				selection[1] * 1.0 / numberOfMeasurements };

		if (maxSel == 1)
			mesResult= {0,1};
			else
			mesResult= {1,0};
		qpp::ket t_t = output.at(i);
		qpp::cplx t = t_t[0];
		mesResult = t - mesResult;
//		std::cout << "I is " << i << std::endl << t_t[0] << std::endl
//				<< "Mes result is:" << mesResult << std::endl;
		result.push_back(mesResult);
	}
//	std::cout << std::endl << "Selection is:";
//	for (unsigned int i = 0; i < selection.size(); i++) {
//		std::cout << selection[i] << " ";
//	}

//	std::cout << std::endl;

	return result;
}
qpp::ket Node::selectStateOnProbability(
		std::tuple<qpp::idx, std::vector<double>, std::vector<qpp::cmat>> & resultOfMeasurement) {
	std::vector<double> probs = std::get<1>(resultOfMeasurement);
	selection = std::vector<int>(probs.size());
	for (unsigned int i = 0; i < probs.size(); i++) {
		selection[i] = 0;
	}
//	std::cout << std::endl << "prob distribution is" << probs.at(0) << ","
//			<< probs.at(1) << std::endl;

	for (int i = 0; i < numberOfMeasurements; i++) {
		double mres = qpp::rand(0.0, 1.0);
		double r = 0;
		unsigned int idx = 0;
		while (mres > r + probs.at(idx)) {
			if (idx + 1 == probs.size())
				break;
			r = r + probs.at(idx);
			idx = idx + 1;
		}
//		std::cout << "r is:" << r << std::endl;
//		std::cout << "mres is:" << mres << std::endl;
//		std::cout << "idx is:" << idx << std::endl;
		selection[idx]++;
	}
//	std::cout << "selection is:" << selection[0] << "," << selection[1]
//			<< std::endl;
	int max = 0;
	int maxI = 0;
	for (unsigned int i = 0; i < probs.size(); i++) {
		if (max < selection[i]) {
			max = selection[i];
			maxI = i;
		}
	}
	maxSel = maxI;
	return std::get<2>(resultOfMeasurement).at(maxI);

}
std::ostream& operator <<(std::ostream& stream, const Node& node) {
	stream << std::endl << "Node name:" << node.name << std::endl;
	stream << std::endl << "Node weights are:" << std::endl;
	for (auto it : node.weights) {
		std::vector<double> t = qpp::abssq(it);
		stream << "State probabilities are:" << std::endl;
		for (double tt : t)
			stream << tt << std::endl;
		stream << std::endl;
	}
	for (auto it : node.historyData)
		stream << "History data is:" << it << std::endl;
	std::cout.precision(12);
	stream << "Output qubit is:" << std::endl;
	stream << qpp::disp(node.outputQubit) << std::endl;
	return stream;
}

Node::~Node() {
}

