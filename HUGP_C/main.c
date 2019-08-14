/*
 * main.c
 *
 *  Created on: Aug. 14, 2019
 *      Author: oscar
 */

#include "ntru/polynomial.h"
#include "util/csprng.h"

int main(void) {

	Polynomial *poly = Poly_new(420, 69);

	Poly_print(poly);

	Polynomial *second = Poly_new(20, 69);

	Poly_timesScalar(second, 3);

	Poly_print(second);

	CSPRNGinit();

	unsigned int result = CSPRNGrandUInt();

	printf("%d\n", result);


	Poly_print(Poly_random(10, 3, 7));

	CSPRNGdestroy();


	Poly_delete(poly);

	return 0;
}

