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

void testMakeExp(){
	//Method was tested, works properly
	double t = 0.3;
	Node n = Node("test.txt",2);
	cout<<"MakeExp Test"<<endl<<qpp::disp(n.makeExp(t))<<std::endl;
}

void testNodeProcess(){
Node t = Node ("n1");
}
int main() {
	vector<vector<ket>> inputs;
	vector<vector<ket>> outputs;
	loadFunction(inputs, outputs);
	//int numberOfInputs = inputs.at(0).size();
	//int numberOfOutputs = outputs.at(0).size();
	//vector<int> config = { numberOfInputs, numberOfOutputs };
	//Network create = Network(config);
	const char* networkName = "testCreate";
//	//Debug inputs
//	for (auto& t : inputs) {
//		cout << "Input:" << endl;
//		for (auto& q : t) {
//			cout << disp(q);
//			cout << endl;
//		}
//	}
//	//Debug outputs
//	for (auto& t : outputs) {
//		cout << "Output:" << endl;
//		for (auto& q : t) {
//			cout << disp(q);
//			cout << endl;
//		}
//	}
	Network test = Network(networkName);
	test.setTolerance(0.95);
	//std::cout<<"here i am"<<std::endl;

	test.train(inputs, outputs);
	test.printNetwork();
	test.test(inputs,outputs,10);
	//test.forward(inputs.at(0));
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
