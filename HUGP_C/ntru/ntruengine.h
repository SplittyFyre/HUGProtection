/*
 * ntruengine.h
 *
 *  Created on: Aug. 14, 2019
 *      Author: oscar
 */

#ifndef NTRUENGINE_H_
#define NTRUENGINE_H_

#include <stdio.h>
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



void NTRUEngine_exportPublicKey(FILE *fout, PubKey *pub);
void NTRUEngine_importPublicKey(FILE *fin, PubKey *puboutp);


#endif /* NTRUENGINE_H_ */
