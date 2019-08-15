/*
 * ntruengine.h
 *
 *  Created on: Aug. 14, 2019
 *      Author: oscar
 */

#ifndef NTRUENGINE_H_
#define NTRUENGINE_H_

#include "polynomial.h"

typedef struct {
	Polynomial *h;
} PubKey;

typedef struct {
	Polynomial *ft;
} PrivKey;

typedef struct {
	PubKey pub;
	PrivKey priv;
} NTRUKeyPair;



NTRUKeyPair NTRUEngine_generateKeyPair();
Polynomial *NTRUEngine_encrypt(Polynomial *msg, PubKey pub);
Polynomial *NTRUEngine_decrypt(Polynomial *cip, PrivKey priv);



#endif /* NTRUENGINE_H_ */
