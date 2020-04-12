/*
 * hugdataio.c
 *
 *  Created on: Aug. 16, 2019
 *      Author: oscar
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "hugdataio.h"
#include "base64.h"

#define BASE64_LINE_LEN 64

void hugdataio_writedata(FILE *fout, const char * const start, const char * const end, uint8_t *data, size_t datalen) {
	fputs(start, fout);

	char *b64str = base64_encode(data, datalen);
	int len = strlen(b64str);
	for (int i = 0, c = -1; i < len; i++) {
		c++;
		if (c == BASE64_LINE_LEN) {
			fputc('\n', fout);
			c = 0;
		}
		fputc(b64str[i], fout);
	}

	fputs("\n", fout);

	fputs(end, fout);

	free(b64str);
}






uint8_t *hugdataio_extractdata(FILE *fin, const char * const start, const char * const end, size_t *datalenp) {

	char buf[128];

	size_t b64buf_size = 256;
	char *b64str = malloc(b64buf_size);
	b64str[0] = '\0';
	size_t numacc = 0;

	bool begun = false;

	while (fgets(buf, 128, fin) != NULL) {
		if (begun) {

			if (strcmp(buf, end) == 0) {
				break;
			}

			int len = strlen(buf);
			buf[len - 1] = '\0'; // replace \n with \0
			// len now must be subtracted by 1

			numacc += (len - 1);
			if (numacc + 1 > b64buf_size) {
				b64buf_size *= 2;
				b64str = realloc(b64str, b64buf_size);
			}
			strcat(b64str, buf);
		}
		else {
			if (strcmp(buf, start) == 0) {
				begun = true;
			}
		}
	}

	//puts(b64str);

	uint8_t *decoded = base64_decode(b64str, datalenp);

	free(b64str);

	return decoded;
}
