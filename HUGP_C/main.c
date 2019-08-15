/*
 * main.c
 *
 *  Created on: Aug. 14, 2019
 *      Author: oscar
 */

#include <stdio.h>


#include "ntru/polynomial.h"
#include "util/csprng.h"
#include "ntru/ntruengine.h"
#include "symmetric/tripleaes.h"

int main(void) {

	CSPRNGinit();



	/*NTRUKeyPair pair = NTRUEngine_generateKeyPair();

	Polynomial *msg = Poly_makeRandom(743, 512, 0);
	Poly_print(msg);

	Polynomial *e = NTRUEngine_encrypt(msg, pair.pub);
	Poly_print(e);

	Polynomial *d = NTRUEngine_decrypt(e, pair.priv);
	Poly_print(d);*/



	uint8_t k1[32];
	uint8_t k2[32];
	uint8_t k3[32];
	CSPRNGrandBytes(k1, 32);
	CSPRNGrandBytes(k2, 32);
	CSPRNGrandBytes(k3, 32);
	uint8_t iv[16];
	CSPRNGrandBytes(iv, 16);



	FILE *f = fopen("plain.txt", "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

	uint8_t *data = malloc(fsize + 1);
	fread(data, 1, fsize, f);
	fclose(f);




	size_t newlen = tripleAESEncrypt(data, fsize + 1, k1, k2, k3, iv);

	FILE *fout = fopen("enced", "wb");
	fwrite(data, 1, newlen, fout);
	fclose(fout);



	tripleAESDecrypt(data, newlen, k1, k2, k3, iv);

	/*for (int i = 0 ;i < newlen; i++) {
		printf("%d ", data[i]);
	}
	puts("");*/

	FILE *fot = fopen("deced", "wb");
	fwrite(data, 1, newlen, fot);
	fclose(fot);




	CSPRNGdestroy();

	puts("done");

	return 0;
}

