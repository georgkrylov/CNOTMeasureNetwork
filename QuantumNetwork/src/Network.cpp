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
	saveDescription(networkPath, layersConfiguration.size()-2);
	std::string layersPath = networkPath + layersFolder;
	checkAndCreateFolder(layersPath);
	for (unsigned int i = 1; i < layersConfiguration.size()-1; i++) {
		std::string layerName = layersPath + std::to_string(i-1);
		Layer newLayer = Layer(layerName,layersConfiguration.at(i),layersConfiguration.at(i-1));
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
void Network::forward(std::vector<qpp::ket>& inputs){
	for (unsigned int i = 0 ;  i < Layers.size();i++){

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

