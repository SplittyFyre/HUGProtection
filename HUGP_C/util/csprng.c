/*
 * csprng.c
 *
 *  Created on: Aug. 14, 2019
 *      Author: oscar
 */


#include "csprng.h"

#include <stdio.h>
#include <stdlib.h>

FILE *urandom = NULL;

void CSPRNGinit() {
	urandom = fopen("/dev/urandom", "rb");
}

void CSPRNGrandBytes(char *dest, int length) {
	if (urandom == NULL) {
		fprintf(stderr, "error in CSPRNGrandBytes(): CSPRNGinit() not called\n");
		exit(1);
	}
	fread(dest, 1, length, urandom);
}

unsigned int CSPRNGrandUInt() {
	if (urandom == NULL) {
		fprintf(stderr, "error in CSPRNGrandUInt(): CSPRNGinit() not called\n");
		exit(1);
	}
	unsigned int result = 0;
	fread(&result, sizeof(unsigned int), 1, urandom);
	return result;
}

void CSPRNGdestroy() {
	if (urandom == NULL) {
		fprintf(stderr, "error in CSPRNGdestroy(): CSPRNGinit() not called\n");
		exit(1);
	}
	fclose(urandom);
}
