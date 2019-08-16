/*
 * base64.c
 *
 *  Created on: Aug. 15, 2019
 *      Author: oscar
 */

#include <string.h>
#include "base64.h"

static const char const b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const int b64invs[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
	59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
	6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, 50, 51 };


size_t get_base64_size(size_t bytelen) {
	size_t retval = bytelen;
	if (bytelen % 3 != 0)
		retval += 3 - (bytelen % 3);
	retval /= 3;
	retval *= 4;
	return retval;
}

size_t get_decoded_size(const char *in) {

	size_t len = strlen(in);
	size_t retval = len / 4 * 3;

	for (int i = len; i-- > 0;) {
		if (in[i] == '=') {
			retval--;
		}
		else {
			break;
		}
	}

	return retval;
}




char *base64_encode(const unsigned char *data, size_t len) {

	char *out;

	size_t outlen = get_base64_size(len);
	out  = malloc(outlen + 1);
	out[outlen] = '\0';

	for (size_t i = 0, j = 0; i < len; i += 3, j += 4) {
		uint32_t v = data[i];
		v = i + 1 < len ? v << 8 | data[i + 1] : v << 8;
		v = i + 2 < len ? v << 8 | data[i + 2] : v << 8;

		out[j]   = b64chars[(v >> 18) & 0x3F];
		out[j + 1] = b64chars[(v >> 12) & 0x3F];

		if (i + 1 < len)
			out[j + 2] = b64chars[(v >> 6) & 0x3F];
		else
			out[j + 2] = '=';

		if (i + 2 < len)
			out[j + 3] = b64chars[v & 0x3F];
		else
			out[j + 3] = '=';
	}

	return out;
}


uint8_t *base64_decode(const char *strin, size_t *lenp) {
	size_t len = strlen(strin);
	size_t i;
	size_t j;
	int    v;

	size_t decodedSize = get_decoded_size(strin);

	*(lenp) = decodedSize;

	uint8_t *out = malloc(decodedSize);

	for (i=0, j=0; i<len; i+=4, j+=3) {
		v = b64invs[strin[i]-43];
		v = (v << 6) | b64invs[strin[i+1]-43];
		v = strin[i+2]=='=' ? v << 6 : (v << 6) | b64invs[strin[i+2]-43];
		v = strin[i+3]=='=' ? v << 6 : (v << 6) | b64invs[strin[i+3]-43];

		out[j] = (v >> 16) & 0xFF;
		if (strin[i+2] != '=')
			out[j+1] = (v >> 8) & 0xFF;
		if (strin[i+3] != '=')
			out[j+2] = v & 0xFF;
	}

	return out;
}



