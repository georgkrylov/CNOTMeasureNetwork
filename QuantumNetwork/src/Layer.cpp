/*
 * Layer.cpp
 *
 *  Created on: May 22, 2018
 *      Author: george
 */

#include "Layer.h"
using namespace std;
using namespace qpp;
Layer::Layer(std::string layerName) {
	checkFolder(layerName);
	unsigned int numberOfNodes = loadDescription(layerName);
	for (unsigned int i = 0; i < numberOfNodes; i++) {

		std::string weightsFolderName = layerName + weightsFolder;
		checkFolder(weightsFolderName);
		std::string weightsFileName = weightsFolderName + weightsFile
				+ to_string(i);
		Node t = Node(weightsFileName);
		nodes.push_back(t);
	}
}
Layer::Layer(std::string layerName, unsigned int numberOfNodes,unsigned int numberOfInputNodes) {
	checkAndCreateFolder(layerName);
	saveDescription(layerName, numberOfNodes);
	for (unsigned int i = 0; i < numberOfNodes; i++) {

		std::string weightsFolderName = layerName + weightsFolder;
		checkAndCreateFolder(weightsFolderName);
		std::string weightsFileName = weightsFolderName + weightsFile
				+ to_string(i);
		Node t =Node(weightsFileName,numberOfInputNodes);
	}
}
void Layer::generateOutputs(std::vector<qpp::ket>& inputs) {
	outputs.clear();
	for (unsigned int i = 0; i < nodes.size(); i++) {
		nodes.at(i).propagateWithInputsAndGenerateOutput(inputs);
		outputs.push_back(nodes.at(i).getOutputQubit());
	}

}
int Layer::getNodesCount() {
	return nodes.size();
}
std::vector<qpp::ket> Layer::getOutputs() {
	return outputs;
}

Layer::~Layer() {
}

