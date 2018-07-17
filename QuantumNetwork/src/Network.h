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
	Network(std::string);
	double forward(std::vector<qpp::ket>& inputs,std::vector<qpp::ket>& outputs);
	void train(std::vector<std::vector<qpp::ket>>functionInputs,std::vector<std::vector<qpp::ket>>functionOutputs);
	virtual ~Network();
	friend std::ostream& operator <<(std::ostream& stream, const Network& net) ;
private:
	std::vector<Layer> Layers;
};

#endif /* NETWORK_H_ */
