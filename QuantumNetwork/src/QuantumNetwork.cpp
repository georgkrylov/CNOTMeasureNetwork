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
  double real = 0.8;
  double imag = 0.6;
  ket ns = st.z0;
  cmat U = gt.X;
  cplx t = {real,0};
  cplx u = {imag,0};
  ns << t,u;
  ns = U*ns;
  cout<<ns;


}
