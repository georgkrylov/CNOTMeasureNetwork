/*
 * Network.h
 *
 *  Created on: May 22, 2018
 *      Author: george
 */

#ifndef NETWORK_H_
#define NETWORK_H_
#include <vector>
#include "Layer.h"
#include "./support/utils.h"
class Network {
public:
	/*
	 * Takes vector of number of nodes per layer, specifying number of nodes in array. Ints stands for number of inputs and outputs this network aims to train
	 */

	Network(std::vector<int>&);
	Network(std::string);double forward(std::vector<std::vector<qpp::ket>>&,
			std::vector<std::vector<qpp::ket>>&);
	void printNetwork();
	void train(std::vector<std::vector<qpp::ket>> functionInputs,
			std::vector<std::vector<qpp::ket>> functionOutputs);
	virtual ~Network();
	friend std::ostream& operator <<(std::ostream& stream, const Network& net);
	void setTolerance(double tolerance);
	void setLearningRate(double learningRate);
	double getTolerance();
	void test(std::vector<std::vector<qpp::ket>> functionInputs,
			std::vector<std::vector<qpp::ket>> functionOutputs,
			int numberOfRepetitions);
	double getLearningRate();
private:
	double calculateTheError();
	std::vector<Layer> Layers;
	double lambda;
	std::vector<double>logFaults;
	double tolerance;
};

#endif /* NETWORK_H_ */
