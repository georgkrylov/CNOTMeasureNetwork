/*
 * Node.h
 *
 *  Created on: May 22, 2018
 *      Author: george
 */

#ifndef NODE_H_
#define NODE_H_
#include "params.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sys/stat.h>
#include <math.h>
#include "qpp.h"
#include "string.h"

class Node {

public:
	//This constructor is used to create a random node with @1 param incoming connections
	Node(std::string,int);
	Node(std::string);
	//First argument is vector of inputs, second argument is the resulting state
	void propagateWithInputsAndGenerateOutput(std::vector<qpp::ket>&);
	//The argument of function will be modified to produce measured final state which is returned. It is an output of a node
	void measureTheOutputState(qpp::ket&);
	double getLogError(std::vector<qpp::ket>& output);
	qpp::ket getOutputQubit();
	void updateWeights(std::vector<std::vector<qpp::ket>>&,double);
	virtual ~Node();
	qpp::ket makeExp(double);

	std::vector<qpp::cplx> getErrorPerOutputVariable(std::vector<qpp::ket>& output);
	friend std::ostream& operator <<(std::ostream& stream, const Node& matrix) ;
private:
	qpp::ket selectStateOnProbability(std::tuple<qpp::idx, std::vector<double>, std::vector<qpp::cmat>>& resultOfMeasurement);
	void loadFromCSVFile(std::string);

	std::vector<double> historyData;
	void saveToInStateFile(std::string nodeName);
	void saveToCSVFile(std::string);
	std::vector<qpp::ket> weights;
	double calculateDelta(std::vector<qpp::cplx>& errors,int);
	double logFault;
	int maxSel;
	std::string name;
	qpp::ket outputQubit;
	//TODO for multi output functions, selection should be vector of vectors
	std::vector<int> selection;

};

#endif /* NODE_H_ */
