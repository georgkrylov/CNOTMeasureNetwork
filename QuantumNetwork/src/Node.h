/*
 * Node.h
 *
 *  Created on: May 22, 2018
 *      Author: george
 */

#ifndef NODE_H_
#define NODE_H_
#include "params.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sys/stat.h>
#include "qpp.h"

class Node {
public:
	//The first integer describes the number of input connections, The second integer describes the number of output connections
	Node(int,int);
	Node(char*);
	virtual ~Node();
private:
	void loadFromFile(char*);
	std::vector<qpp::ket> weights;
};

#endif /* NODE_H_ */
