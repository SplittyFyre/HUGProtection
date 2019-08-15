/*
 * main.c
 *
 *  Created on: Aug. 14, 2019
 *      Author: oscar
 */

#include "ntru/polynomial.h"
#include "util/csprng.h"
#include "ntru/ntruengine.h"

int main(void) {

	CSPRNGinit();



	NTRUKeyPair pair = NTRUEngine_generateKeyPair();

	Polynomial *msg = Poly_makeRandom(743, 16, 0);
	Poly_print(msg);

	Polynomial *e = NTRUEngine_encrypt(msg, pair.pub);
	Poly_print(e);

	Polynomial *d = NTRUEngine_decrypt(e, pair.priv);
	Poly_print(d);


	CSPRNGdestroy();

	return 0;
}

