/*
 * polynomial.h
 *
 *  Created on: Aug. 14, 2019
 *      Author: oscar
 */

#ifndef POLYNOMIAL_H_
#define POLYNOMIAL_H_

#include <stdint.h>

typedef int16_t pci_t;


typedef struct {

	pci_t *coef;
	pci_t coeflen;

} Polynomial;

Polynomial *Poly_new(pci_t coefficient, int exponent);
Polynomial *Poly_withcoeflen(int coeflen);
Polynomial *Poly_from_other(pci_t *coefficients, int tocopy, int coeflen);
Polynomial *Poly_scalarAs(pci_t scalar, int coeflen);
void Poly_delete(Polynomial *poly);


int Poly_calcDegree(Polynomial *this);
Polynomial *Poly_plus(Polynomial *this, Polynomial *that);
Polynomial *Poly_minus(Polynomial *this, Polynomial *that);

void Poly_add_inplace(Polynomial *dest, Polynomial *src);
void Poly_addScalar(Polynomial *this, pci_t scalar);
void Poly_minus_inplace(Polynomial *dest, Polynomial *src);

Polynomial *Poly_times(Polynomial *this, Polynomial *that);
void Poly_times_to_dest(Polynomial *this, Polynomial *that, Polynomial *dest);
void Poly_timesScalar(Polynomial *this, pci_t scalar);

void Poly_modby(Polynomial *this, int modulus);


Polynomial *Poly_inverseMod2(Polynomial *this);
Polynomial *Poly_mod2TomodQ(Polynomial *this, Polynomial *f2, int q);
Polynomial *Poly_inverseModQ(Polynomial *this, int q);


void Poly_ensurePositive(Polynomial *this, int modulus);
void Poly_center(Polynomial *this, int q);

Polynomial *Poly_makeRandom(int N, int numOnes, int numNegOnes);
void Poly_scramble(Polynomial *this);
void Poly_print(Polynomial *this);



#endif /* POLYNOMIAL_H_ */
