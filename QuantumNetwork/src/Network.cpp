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
	saveDescription(networkPath, layersConfiguration.size() - 1);
	std::string layersPath = networkPath + layersFolder;
	checkAndCreateFolder(layersPath);
	for (unsigned int i = 1; i < layersConfiguration.size(); i++) {
		std::string layerName = layersPath + std::to_string(i - 1);
		Layer newLayer = Layer(layerName, layersConfiguration.at(i),
				layersConfiguration.at(i - 1));
	}

	//std::cout << "Layers of size " << Layers.size() << " are created"
	//		<< std::endl;
	lambda = 0.01;
	tolerance = 0.3;

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
	lambda = 0.01;
	tolerance = 0.3;
}
void Network::setTolerance(double tolerance) {
	this->tolerance = tolerance;
}
void Network::setLearningRate(double learningRate) {
	this->lambda = learningRate;
}

double Network::getTolerance() {
	return tolerance;
}
double Network::getLearningRate() {
	return tolerance;
}
double Network::calculateTheError() {
	double result = 0;
	for (double err : logFaults) {
		result += err;
	}
	return result / logFaults.size();
}
double Network::forward(std::vector<std::vector<qpp::ket>>& functionInputs,
		std::vector<std::vector<qpp::ket>>& functionOutputs) {
	double meanError = 0;
	for (unsigned int i = 0; i < functionInputs.size(); i++) {
//		std::cout<<std::endl<<"Working with input"<< i<<std::endl;
		std::vector<qpp::ket> temp = functionInputs.at(i);
		std::vector<qpp::ket> output = functionOutputs.at(i);
		for (unsigned int l = 0; l < Layers.size(); l++) {
			Layers.at(l).processInputAndProduceOutput(temp);
			temp = Layers.at(l).getOutputs();
		}
		double finalLogError = 0;
		for (unsigned int l = 0; l < Layers.size(); l++) {
			finalLogError = Layers.at(l).getAccumulatedLogError(output);
		}
		for (Layer& l : Layers)
			l.updateLayer(functionOutputs[i], lambda);
		//EachLayer???
		logFaults[i]=finalLogError;
//		std::cout<<"LogFaults = "<<std::endl;
//		for(double l:logFaults)
//			std::cout<<l<<std::endl;
//		std::cout<<std::endl;

		meanError = calculateTheError();
//		std::cout << "Error is:" << meanError << std::endl;

	}

//	std::cout<<"Layer size is: "<<Layers.size()<<std::endl;
	return meanError;

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
	logFaults = std::vector<double>(functionInputs.size());
	int i = 0;
	while (1 - meanError < tolerance) {

//	while (i < 30) {
//		i++;
		i++;
		meanError = forward(functionInputs, functionOutputs);
		if (i%100==0)
			lambda = lambda +0.01;
	}

}
void Network::printNetwork() {
	for (Layer l : Layers) {
		l.printLayer();
	}
}
void Network::test(std::vector<std::vector<qpp::ket>> functionInputs,
		std::vector<std::vector<qpp::ket>> functionOutputs,
		int numberOfRepetitions) {
	std::vector<int> realOutp;
	int rll,mrl;
	std::vector<int>count= std::vector<int>(numberOfRepetitions);
	for (int& c:count)
		c=0;
	for (unsigned int i = 0; i < functionInputs.size(); i++) {

		std::cout << std::endl << "For inputs:" << std::endl;
		for (unsigned int j = 0; j < functionInputs[i].size(); j++) {
			auto result = qpp::measure(functionInputs[i][j], qpp::gt.Z, { 0 });
			std::cout << std::get<0>(result) << " ";

		}
		std::cout << ":";
		for (unsigned int j = 0; j < functionOutputs[i].size(); j++) {
			for (int q = 0; q < 1; q++) {
				auto result = qpp::measure(functionOutputs[i][j], qpp::gt.Z,
						{ 0 });
				rll=std::get<0>(result) ;
				std::cout << rll << " ";

			}
		}
		std::cout << "Got outputs:";
		for (int qq = 0; qq < numberOfRepetitions; qq++) {
			std::vector<qpp::ket> temp = functionInputs[i];
			for (unsigned int i = 0; i < Layers.size(); i++) {
				Layers.at(i).processInputAndProduceOutput(temp);
				temp = Layers.at(i).getOutputs();
			}
			for (unsigned int j = 0; j < temp.size(); j++) {
				auto result = qpp::measure(temp[j], qpp::gt.Z, { 0 });
				mrl = std::get<0>(result);
				std::cout<<mrl;

			}
			if (mrl==rll){
				count[qq]+=1<<i;
			}
		}
		std::cout<<std::endl;
	}
	double percent = 0.0;
	int limit = (1<<(functionInputs.size()))-1;
	for(int c:count){
		if (c==limit)
			percent = percent+1;
	}
	std::cout<<"This network will approximate the function in "<<percent/numberOfRepetitions*100<<" percents"<<std::endl;
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

