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
#include "Node.h"

using namespace qpp;
using namespace std;
int main() {
	checkFolders();
	std::cout << "Hello Quantum++!\nThis is the |0> state:\n";
    std::cout << disp(st.z0) << '\n';
    ket myState= kron(st.z1,st.z0);

   auto res=measure(myState, gt.Z, {0});
   idx myResult=get<0>(res);
   cout<<myResult<<endl;
   auto probs = get<1>(res);
   cout<<disp(probs,",")<<endl;
   for (auto&& it : get<2>(res))
	   cout << disp(it) << "\n\n";
  vector <ket> inputs;
  vector <ket> outputs;
  loadFunction(inputs,outputs);
  cplx x = {0.8,0};
  cplx y = {0.6,0};
  ket t(2);
  t<< x,y;
  cout<<disp(t);
  const char* nodeName = "./weights/n1";
  Node tmp = Node(nodeName);
  cout<<tmp<<endl;

}
