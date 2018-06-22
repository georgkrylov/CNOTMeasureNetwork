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
	Layer();
	virtual ~Layer();
private:
	Layer* prevLayer;
	Layer* nextLayer;
};

#endif /* LAYER_H_ */
