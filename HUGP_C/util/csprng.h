/*
 * csprng.h
 *
 *  Created on: Aug. 14, 2019
 *      Author: oscar
 */

#ifndef CSPRNG_H_
#define CSPRNG_H_

#ifdef WIN32
#error "your operating system is windows, HUGP CSPRNG depends on /dev/urandom on nix-ish systems"
#endif

void CSPRNGinit();
void CSPRNGrandBytes(char *dest, int length);
unsigned int CSPRNGrandUInt();
void CSPRNGdestroy();


#endif /* CSPRNG_H_ */
