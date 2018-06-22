/*
 * Node.h
 *
 *  Created on: May 22, 2018
 *      Author: george
 */

#ifndef NODE_H_
#define NODE_H_

class Node {
public:
	//The first integer describes the number of input connections, The second integer describes the number of output connections
	Node(int,int);
	Node();
	virtual ~Node();
};

#endif /* NODE_H_ */
