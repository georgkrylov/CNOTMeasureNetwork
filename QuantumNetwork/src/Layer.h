/*
 * Layer.h
 *
 *  Created on: May 22, 2018
 *      Author: george
 */

#ifndef LAYER_H_
#define LAYER_H_

class Layer {
public:
	/*
	 * Creates a new layer and connects it to its previous layer
	 * params: int - number of nodes, Layer& - previous node
	 */
	Layer(int,Layer*);
	virtual ~Layer();
private:
	Layer* prevLayer;
	Layer* nextLayer;
};

#endif /* LAYER_H_ */
