/*
 * symkeygen.c
 *
 *  Created on: Aug. 20, 2019
 *      Author: oscar
 */

#include <stdbool.h>

#include "symkeygen.h"

#include "../util/sha256.h"


void symTripleKeyGen(const char *password, const uint8_t *salt, int saltlen, uint8_t *keyout1, uint8_t *keyout2, uint8_t *keyout3) {

	SHA256_CTX ctx;
	int passlen = strlen(password);
	int datalen = passlen + saltlen;
	uint8_t data = malloc(datalen);
	memcpy(data, password, passlen);
	memcpy((data + passlen), salt, saltlen);

	uint8_t hash[32];

	bool flag = false;
	for (int i = 0; i < SYM_KEY_GEN_ITERCNT; i++) {
		sha256_init(&ctx);
		sha256_update(&ctx, data, datalen);
		sha256_final(&ctx, hash);

		if (!flag) {
			flag = true;
			data = realloc(data, 32);
		}
		memcpy(data, hash, 32);
	}
	memcpy(keyout1, hash, 32);


	datalen = 32 + saltlen;
	data = realloc(data, datalen);
	memcpy((data + 32), salt, saltlen);

	flag = false;
	for (int i = 0; i < SYM_KEY_GEN_ITERCNT; i++) {
		sha256_init(&ctx);
		sha256_update(&ctx, data, datalen);
		sha256_final(&ctx, hash);

		if (!flag) {
			flag = true;
			data = realloc(data, 32);
		}
		memcpy(data, hash, 32);
	}
	memcpy(keyout1, hash, 32);

}
