/*
 * hugdataio.h
 *
 *  Created on: Aug. 16, 2019
 *      Author: oscar
 */

#ifndef HUGDATAIO_H_
#define HUGDATAIO_H_

#include <stdint.h>

void hugdataio_writedata(FILE *fout, const char * const start, const char * const end, uint8_t *data, size_t datalen);
uint8_t *hugdataio_extractdata(FILE *fin, const char * const start, const char * const end, size_t *datalenp);

#endif /* HUGDATAIO_H_ */
