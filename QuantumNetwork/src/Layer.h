/*
 * Layer.h
 *
 *  Created on: May 22, 2018
 *      Author: george
 */

#ifndef LAYER_H_
#define LAYER_H_
#include "qpp.h"
#include "Node.h"
#include "./support/utils.h"
#include <vector>
#include <iostream>
class Layer {

public:
	/*
	 * Creates a new layer and connects it to its previous layer
	 * params: int - number of nodes, Layer& - previous node
	 */
	Layer(std::string,unsigned int,unsigned int);
	Layer(std::string layerName);
	std::vector<qpp::ket> getOutputs();
	void generateOutputs(std::vector<qpp::ket>&);
	int getNodesCount();
	virtual ~Layer();
private:

	std::vector<Node> nodes;
	std::vector<qpp::ket> outputs;
};

#endif /* LAYER_H_ */
