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
#include "util/base64.h"
#include "util/sha256.h"

int main(void) {

	CSPRNGinit();


	/*SHA256_CTX ctx;
	sha256_init(&ctx);
	char *data = "pom[]";
	sha256_update(&ctx, data, 5);

	unsigned char hash[32];

	sha256_final(&ctx, hash);

	for (int i = 0; i < 32; i++) {
		printf("%d ", hash[i]);
	}
	puts("");*/

	NTRUKeyPair pair = NTRUEngine_generateKeyPair();
	FILE *fout = fopen("pub.asc", "w");
	FILE *fout2 = fopen("priv.asc", "w");

	NTRUEngine_exportPublicKey(fout, pair.pub);
	NTRUEngine_e



	/*NTRUKeyPair pair = NTRUEngine_generateKeyPair();

	FILE *fout = fopen("test.asc", "w");
	NTRUEngine_exportPublicKey(fout, &pair.pub);
	fclose(fout);

	Poly_print(pair.pub.h);*/

	/*FILE *fin = fopen("test.asc", "r");
	PubKey pub;
	NTRUEngine_importPublicKey(fin, &pub);
	fclose(fin);
	Poly_print(pub.h);*/
	/*Polynomial *msg = Poly_makeRandom(743, 512, 0);
	Poly_print(msg);

	Polynomial *e = NTRUEngine_encrypt(msg, pair.pub);
	Poly_print(e);

	Polynomial *d = NTRUEngine_decrypt(e, pair.priv);
	Poly_print(d);*/


	/*uint8_t k1[32];
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
	fseek(f, 0, SEEK_SET);  /* same as rewind(f);

	uint8_t *data = malloc(fsize + 1);
	fread(data, 1, fsize, f);
	fclose(f);




	size_t newlen = tripleAESEncrypt(data, fsize + 1, k1, k2, k3, iv);

	FILE *fout = fopen("enced", "wb");
	fwrite(data, 1, newlen, fout);
	fclose(fout);



	size_t out = tripleAESDecrypt(data, newlen, k1, k2, k3, iv);

	for (int i = 0 ;i < out; i++) {
		printf("%d ", data[i]);
	}
	puts("");

	FILE *fot = fopen("deced", "wb");
	fwrite(data, 1, out, fot);
	fclose(fot);

	uint8_t thing[10] = "abcdefghi\0";

	char *str = base64_encode(thing, 10);

	puts(str);

	size_t size = 0;
	uint8_t *outed = base64_decode(str, &size);
	for (int i = 0; i < size; i++) {
		printf("%d ", outed[i]);
	}
	puts("");
	puts(outed);*/


	CSPRNGdestroy();

	puts("done");

	return 0;
}

