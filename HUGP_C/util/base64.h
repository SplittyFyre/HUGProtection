/*
 * base64.h
 *
 *  Created on: Aug. 15, 2019
 *      Author: oscar
 */

#ifndef BASE64_H_
#define BASE64_H_

#include <stdlib.h>
#include <stdint.h>


char *base64_encode(const unsigned char *data, size_t len);
uint8_t *base64_decode(const char *strin, size_t *lenp);


#endif /* BASE64_H_ */
