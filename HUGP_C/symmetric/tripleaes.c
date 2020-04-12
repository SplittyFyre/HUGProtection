/*
 * tripleaes.c
 *
 *  Created on: Aug. 15, 2019
 *      Author: oscar
 */


#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>

#include "tripleaes.h"

#include "raw/aes.h"


size_t tripleAESEncrypt(uint8_t *data, size_t datalen,
		uint8_t *kb1, uint8_t *kb2, uint8_t *kb3, uint8_t *iv) {

	size_t newsize;

	if (datalen % 16 == 0) {
		newsize = datalen + 16;
	}
	else {
		size_t next16th = ((datalen + 15) & (-16));
		newsize = next16th;
	}

	data = realloc(data, newsize);

	// pad using pkcs#7
	int padsneeded = newsize - datalen;
	for (int i = datalen; i < newsize; i++) {
		data[i] = (uint8_t) padsneeded;
	}
	//printf("datalen: %d\npadsneeded: %d\nnewlen: %d\n", datalen, padsneeded, newsize);

	struct AES_ctx ctx1, ctx2, ctx3;
	AES_init_ctx(&ctx1, kb1);
	AES_init_ctx(&ctx2, kb2);
	AES_init_ctx(&ctx3, kb3);

	uint8_t *prevblock = iv;

	for (size_t i = 0; i < newsize; i += 16) {

		uint8_t *bufferpos = (data + i);

		for (int j = 0; j < 16; j++) {
			bufferpos[j] ^= prevblock[j];
		}

		AES_ECB_encrypt(&ctx1, bufferpos);
		AES_ECB_decrypt(&ctx2, bufferpos);
		AES_ECB_encrypt(&ctx3, bufferpos);

		prevblock = bufferpos;
	}

	return newsize;
}







size_t tripleAESDecrypt(uint8_t *data, size_t datalen,
		uint8_t *kb1, uint8_t *kb2, uint8_t *kb3, uint8_t *iv) {

	struct AES_ctx ctx1, ctx2, ctx3;
	AES_init_ctx(&ctx1, kb1);
	AES_init_ctx(&ctx2, kb2);
	AES_init_ctx(&ctx3, kb3);

	uint8_t *prevblock = malloc(16);
	memcpy(prevblock, iv, 16);

	uint8_t *tmp = malloc(16);


	for (size_t i = 0; i < datalen; i += 16) {

		uint8_t *bufferpos = (data + i);

		memcpy(tmp, bufferpos, 16);

		AES_ECB_decrypt(&ctx3, bufferpos);
		AES_ECB_encrypt(&ctx2, bufferpos);
		AES_ECB_decrypt(&ctx1, bufferpos);

		for (int j = 0; j < 16; j++) {
			bufferpos[j] ^= prevblock[j];
		}

		memcpy(prevblock, tmp, 16);
	}

	free(prevblock);
	free(tmp);

	// return size of buffer excluding pkcs#7 padding
	return datalen - data[datalen - 1];
}



