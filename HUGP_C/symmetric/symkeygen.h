/*
 * symkeygen.h
 *
 *  Created on: Aug. 20, 2019
 *      Author: oscar
 */

#ifndef SYMKEYGEN_H_
#define SYMKEYGEN_H_

#include <stdint.h>

#define SYM_KEY_GEN_ITERCNT 32767

void symTripleKeyGen(const char *password, const uint8_t *salt, int saltlen, uint8_t *keyout1, uint8_t *keyout2, uint8_t *keyout3);


#endif /* SYMKEYGEN_H_ */
