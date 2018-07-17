/*
 * Network.cpp
 *
 *  Created on: May 22, 2018
 *      Author: george
 */

#include "Network.h"

Network::Network(std::vector<int>& layersConfiguration) {
	checkAndCreateFolder(networksFolder);
	std::string networkName = "testCreate";
	std::string networkPath = networksFolder + networkName;
	checkAndCreateFolder(networkPath);
	saveDescription(networkPath, layersConfiguration.size() - 2);
	std::string layersPath = networkPath + layersFolder;
	checkAndCreateFolder(layersPath);
	for (unsigned int i = 1; i < layersConfiguration.size(); i++) {
		std::string layerName = layersPath + std::to_string(i - 1);
		Layer newLayer = Layer(layerName, layersConfiguration.at(i),
				layersConfiguration.at(i - 1));
	}

}
Network::Network(std::string networkName) {
	std::string networkPath = networksFolder + networkName;
	checkFolder(networkPath);
	unsigned int size = loadDescription(networkPath);
	for (unsigned int i = 0; i < size; i++) {
		std::string layerName = networkPath + layersFolder + std::to_string(i);
		Layer tmp = Layer(layerName);
		Layers.push_back(tmp);
	}
}
double Network::forward(std::vector<qpp::ket>& inputs,
		std::vector<qpp::ket>& outputs) {
	std::vector<qpp::ket> temp = inputs;
	for (unsigned int i = 0; i < Layers.size(); i++) {
		Layers.at(i).processInputAndProduceOutput(temp);
		temp = Layers.at(i).getOutputs();
	}
	double q = 0;
	for (unsigned int i = 0; i < Layers.size(); i++) {
		Layers.at(i).updateLayer(outputs);
		q = Layers.at(i).getAccumulatedLogError();
	}
//	std::cout<<"Layer size is: "<<Layers.size()<<std::endl;
	return q;

//	std::cout << "Input is:" << std::endl;
//	for (auto& t : inputs)
//		std::cout << qpp::disp(t) << std::endl;
//	std::cout << "Output is:" << std::endl;
//	for (auto& t : temp)
//		std::cout << qpp::disp(t)<<std::endl;
}
void Network::train(std::vector<std::vector<qpp::ket>> functionInputs,
		std::vector<std::vector<qpp::ket>> functionOutputs) {
	double meanError = 1.0;
	while(meanError>TOLERANCE){
	meanError = 0.0;
	for (unsigned int i = 0; i < functionInputs.size(); i++) {
		meanError += forward(functionInputs[i], functionOutputs[i]);
	}
	meanError /= functionInputs.size();
	std::cout<<meanError<<std::endl;
	}
}
std::ostream& operator <<(std::ostream& stream, const Network& net) {
	stream << net.Layers.size() << std::endl;
	for (Layer it : net.Layers)
		stream << it.getNodesCount() << " ";
	stream << std::endl;
	return stream;
}
Network::~Network() {
}

