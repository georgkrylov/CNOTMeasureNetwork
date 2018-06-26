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
class Network {
public:
	/*
	 * Takes number of hidden layers, vector of number of nodes per layer in format input-hidden-..-output
	 */
	std::vector<Layer> Layers;
	Network(int,std::vector<int>&);
	void loadNetworkWeights();
	virtual ~Network();
};

#endif /* NETWORK_H_ */
