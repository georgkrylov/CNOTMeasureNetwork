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
Layer::Layer(std::string layerName, unsigned int numberOfNodes,
		unsigned int numberOfInputNodes) {
	checkAndCreateFolder(layerName);
	saveDescription(layerName, numberOfNodes);
	for (unsigned int i = 0; i < numberOfNodes; i++) {

		std::string weightsFolderName = layerName + weightsFolder;
		checkAndCreateFolder(weightsFolderName);
		std::string weightsFileName = weightsFolderName + weightsFile
				+ to_string(i);
		Node t = Node(weightsFileName, numberOfInputNodes);
	}
	//std::cout << "Layer with "<<numberOfNodes<< " nodes is created and connected to layer with "<<numberOfInputNodes<<std::endl;
}
void Layer::processInputAndProduceOutput(std::vector<qpp::ket>& inputs) {
	outputs.clear();
	for (unsigned int i = 0; i < nodes.size(); i++) {
		nodes.at(i).propagateWithInputsAndGenerateOutput(inputs);
//		std::cout<<"Layer outputs::"<<std::endl<<qpp::disp(nodes.at(i).getOutputQubit())<<std::endl;
		outputs.push_back(nodes.at(i).getOutputQubit());
	}

}
void Layer::updateLayer(std::vector<qpp::ket>& outputs, double learningRate) {
	for (unsigned int i = 0; i < nodes.size(); i++) {
//		std::cout<<"Before update"<<std::endl;
//		std::cout<<nodes[i];
		nodes[i].updateWeights(outputs, learningRate);
//		std::cout<<n.getLogError()<<std::endl;
//		std::cout<<"After update";
//		std::cout<<nodes[i];
	}

}

int Layer::getNodesCount() {
	return nodes.size();
}
std::vector<qpp::ket> Layer::getOutputs() {
	return outputs;
}
void Layer::printLayer() {
	for (Node n : nodes) {
		std::cout << n;
	}
}

double Layer::getAccumulatedLogError(std::vector<qpp::ket>& output) {
	//std::cout<<"here i am trying to get accumulated log error"<<std::endl;
	double err = 0.0;
	for (unsigned int i = 0; i < nodes.size(); i++) {
		err += nodes.at(i).getLogError(output);
//		std::cout << nodes[i] << std::endl;
	}
	//std::cout << err << std::endl;
	return err / nodes.size();
}
Layer::~Layer() {
}

