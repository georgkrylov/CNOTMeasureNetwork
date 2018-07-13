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
	qpp::ket getOutputQubit();

	virtual ~Node();

	friend std::ostream& operator <<(std::ostream& stream, const Node& matrix) ;
private:
	qpp::ket selectStateOnProbability(std::tuple<qpp::idx, std::vector<double>, std::vector<qpp::cmat>>& resultOfMeasurement);
	void loadFromCSVFile(std::string);
	void saveToInStateFile(std::string nodeName);
	void saveToCSVFile(std::string);
	std::vector<qpp::ket> weights;
	qpp::ket outputQubit;

};

#endif /* NODE_H_ */
