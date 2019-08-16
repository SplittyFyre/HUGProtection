/*
 * ntruengine.c
 *
 *  Created on: Aug. 14, 2019
 *      Author: oscar
 */


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>
#include "ntruengine.h"
#include "../util/base64.h"
#include "../util/hugdataio.h"


#define Q 2048
#define N 743


NTRUKeyPair NTRUEngine_generateKeyPair() {

	// generate g polynomial
	Polynomial *g = Poly_makeRandom(N, 247, 247 - 1);

	while (1) {
		Polynomial *p = Poly_inverseMod2(g);
		if (p != NULL) {
			Poly_delete(p);
			break;
		}
		Poly_delete(p);
		Poly_scramble(g);
	}

	Polynomial *f = Poly_withcoeflen(N);
	Polynomial *fq;
	Polynomial *t = Poly_makeRandom(N, 22, 22);

	// generate f and get fq
	while (1) {
		Poly_scramble(t);
		// copy contents of t into f
		memcpy(f->coef, t->coef, N * sizeof(pci_t));
		Poly_timesScalar(f, 3);
		Poly_addScalar(f, 1);

		fq = Poly_inverseModQ(f, Q);
		if (fq != NULL) {
			break;
		}
		Poly_delete(fq);
	}

	// g * fp mod q
	Polynomial *h = Poly_times(g, fq);
	Poly_modby(h, Q);
	Poly_timesScalar(h, 3); // p is always 3
	Poly_modby(h, Q);
	Poly_ensurePositive(h, Q);



	// clean up
	Poly_delete(f);
	Poly_delete(g);
	Poly_delete(fq);


	PubKey pub;
	pub.h = h;
	PrivKey priv;
	priv.ft = t;

	NTRUKeyPair keyPair;
	keyPair.pub = pub;
	keyPair.priv = priv;

	return keyPair;
}




Polynomial *NTRUEngine_encrypt(Polynomial *msg, PubKey pub) {

	Polynomial *r = Poly_makeRandom(N, 5, 5);

	Polynomial *e = Poly_times(r, pub.h);
	Poly_modby(e, Q);
	Poly_add_inplace(e, msg);
	Poly_modby(e, Q);
	Poly_ensurePositive(e, Q);

	Poly_delete(r);

	return e;
}


Polynomial *NTRUEngine_decrypt(Polynomial *cip, PrivKey priv) {
	Polynomial *a = Poly_times(priv.ft, cip);
	Poly_modby(a, Q);
	Poly_timesScalar(a, 3);
	Poly_add_inplace(a, cip);
	Poly_center(a, Q);
	Poly_modby(a, 3);

	Poly_center(a, 3);

	return a;
}








const char * const BEGIN_PUB = "-----BEGIN HUG PUBLIC KEY-----\n";
const char * const END_PUB = "-----BEGIN HUG PUBLIC KEY-----\n";


void NTRUEngine_exportPublicKey(FILE *fout, PubKey *pub) {

	uint8_t *data = (uint8_t*) pub->h->coef;
	size_t datalen = pub->h->coeflen * sizeof(pci_t);

	hugdataio_writedata(fout, BEGIN_PUB, END_PUB, data, datalen);

}


void NTRUEngine_importPublicKey(FILE *fin, PubKey *puboutp) {

	size_t datalen;
	uint8_t *decoded = hugdataio_extractdata(fin, BEGIN_PUB, END_PUB, &datalen);

	Polynomial *poly = malloc(sizeof(Polynomial));
	poly->coeflen = datalen / sizeof(pci_t);
	poly->coef = (pci_t*) decoded;

	puboutp->h = poly;

}















