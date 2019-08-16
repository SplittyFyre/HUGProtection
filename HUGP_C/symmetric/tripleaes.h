/*
 * tripleaes.h
 *
 *  Created on: Aug. 15, 2019
 *      Author: oscar
 */

#ifndef TRIPLEAES_H_
#define TRIPLEAES_H_


#include <stdint.h>


size_t tripleAESEncrypt(uint8_t *data, size_t datalen,
		uint8_t *kb1, uint8_t *kb2, uint8_t *kb3, uint8_t *iv);

size_t tripleAESDecrypt(uint8_t *data, size_t datalen,
		uint8_t *kb1, uint8_t *kb2, uint8_t *kb3, uint8_t *iv);

#endif /* TRIPLEAES_H_ */
