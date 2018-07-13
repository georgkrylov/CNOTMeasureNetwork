//============================================================================
// Name        : QuantumNetwork.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "./support/utils.h"
#include "qpp.h"
#include "Network.h"

using namespace qpp;
using namespace std;
int main() {
	vector<vector<ket>> inputs;
	vector<vector<ket>> outputs;
	loadFunction(inputs, outputs);
	const char* networkName = "testCreate";
//	vector<int> config = {2,3,4,2};
//	Network create = Network(config);
	Network test = Network(networkName);
	/* Test Layer*/
//	Layer t = Layer(1,nodeName);
//	t.generateOutputs(inputs.at(0));
//	cout<<endl;
//	std::vector<qpp::ket> k = t.getOutputs();
//	for (int i = 0 ; i < 1; i++){
//		cout<<disp(k.at(i));
//	}

	/*	Test node*/
// 	 Node tmp = Node(nodeName);
// 	 vector<ket> in = inputs.at(0);
//	 tmp.propagateWithInputsAndGenerateOutput(in);
//	 ket out = tmp.getOutputQubit();
//	 cout<<disp(out);
}
