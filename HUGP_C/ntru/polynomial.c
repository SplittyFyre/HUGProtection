/*
 * polynomial.c
 *
 *  Created on: Aug. 13, 2019
 *      Author: oscar
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include "polynomial.h"
#include "../util/csprng.h"

#define MAX(a, b) ( ( (a) > (b) ) ? (a) : (b) )


// private

bool Poly_equalsZero(Polynomial *this) {
	for (int i = 0; i < this->coeflen; i++)
		if (this->coef[i] != 0)
			return false;
	return true;
}

bool Poly_equalsScalar(Polynomial *this, int scalar) {
	if (this->coef[0] != scalar)
		false;
	for (int i = 1; i < this->coeflen; i++)
		if (this->coef[i] != 0)
			return false;
	return true;
}

// private


Polynomial *Poly_new(int coefficient, int exponent) {
	Polynomial *poly = malloc(sizeof(Polynomial));
	poly->coef = calloc(exponent + 1, sizeof(int));
	poly->coeflen = (exponent + 1);
	poly->coef[exponent] = coefficient;
	return poly;
}

Polynomial *Poly_withcoeflen(int coeflen) {
	Polynomial *poly = malloc(sizeof(Polynomial));
	poly->coef = calloc(coeflen, sizeof(int));
	poly->coeflen = coeflen;
	return poly;
}

Polynomial *Poly_from_other(int *coefficients, int tocopy, int coeflen) {
	Polynomial *poly = Poly_withcoeflen(coeflen);
	memcpy(poly->coef, coefficients, tocopy * sizeof(int));
	poly->coeflen = coeflen;
	return poly;
}

Polynomial *Poly_scalarAs(int scalar, int coeflen) {
	Polynomial *poly = Poly_withcoeflen(coeflen);
	poly->coef[0] = scalar;
	return poly;
}


void Poly_delete(Polynomial *poly) {
	free(poly->coef);
	free(poly);
}


int Poly_calcDegree(Polynomial *this) {
	for (int i = this->coeflen - 1; i >= 0; i--) {
		if (this->coef[i] != 0) {
			return i;
		}
	}
	return -1;
}


Polynomial *Poly_plus(Polynomial *this, Polynomial *that) {
	int thisdeg = Poly_calcDegree(this);
	int thatdeg = Poly_calcDegree(that);
	Polynomial *poly = Poly_withcoeflen(MAX(thisdeg, thatdeg) + 1);
	for (int i = 0; i < this->coeflen; i++) poly->coef[i] += this->coef[i];
	for (int i = 0; i < that->coeflen; i++) poly->coef[i] += that->coef[i];
	return poly;
}

Polynomial *Poly_minus(Polynomial *this, Polynomial *that) {
	int thisdeg = Poly_calcDegree(this);
	int thatdeg = Poly_calcDegree(that);
	Polynomial *poly = Poly_withcoeflen(MAX(thisdeg, thatdeg) + 1);
	for (int i = 0; i < this->coeflen; i++) poly->coef[i] += this->coef[i];
	for (int i = 0; i < that->coeflen; i++) poly->coef[i] -= that->coef[i];
	return poly;
}


void Poly_add_inplace(Polynomial *dest, Polynomial *src) {
	for (int i = 0; i < dest->coeflen; i++) {
		dest->coef[i] += src->coef[i];
	}
}
void Poly_addScalar(Polynomial *this, int scalar) {
	this->coef[0] += scalar;
}
void Poly_minus_inplace(Polynomial *dest, Polynomial *src) {
	for (int i = 0; i < dest->coeflen; i++) {
		dest->coef[i] -= src->coef[i];
	}
}


Polynomial *Poly_times(Polynomial *this, Polynomial *that) {
	int thisdeg = Poly_calcDegree(this);
	int thatdeg = Poly_calcDegree(that);
	Polynomial *poly = Poly_withcoeflen(thisdeg + thatdeg + 1);
	for (int i = 0; i <= thisdeg; i++)
		for (int j = 0; j <= thatdeg; j++)
			poly->coef[i + j] += (this->coef[i] * that->coef[j]);

	// below code takes indices mod this->coeflen

	if (poly->coeflen > this->coeflen) {
		for (int k = this->coeflen; k < poly->coeflen; k++)
			poly->coef[k - this->coeflen] += poly->coef[k];
		poly->coef = realloc(poly->coef, this->coeflen * sizeof(int));
		poly->coeflen = this->coeflen;
	}
	return poly;
}

void Poly_times_to_dest(Polynomial *this, Polynomial *that, Polynomial *dest) {
	int thisdeg = Poly_calcDegree(this);
	int thatdeg = Poly_calcDegree(that);

	if (dest->coeflen != thisdeg + thatdeg + 1) {
		fprintf(stderr, "error in Poly_times_to_dest(), dest must have coef size %d, not %d\n", thisdeg + thatdeg + 1, dest->coeflen);
		exit(1);
	}

	for (int i = 0; i <= thisdeg; i++)
		for (int j = 0; j <= thatdeg; j++)
			dest->coef[i + j] += (this->coef[i] * that->coef[j]);

	// below code takes indices mod this->coeflen

	if (dest->coeflen > this->coeflen) {
		for (int k = this->coeflen; k < dest->coeflen; k++)
			dest->coef[k - this->coeflen] += dest->coef[k];
		dest->coef = realloc(dest->coef, this->coeflen * sizeof(int));
		dest->coeflen = this->coeflen;
	}
}


void Poly_timesScalar(Polynomial *this, int scalar) {
	for (int i = 0; i < this->coeflen; i++) {
		this->coef[i] *= scalar;
	}
}

void Poly_modby(Polynomial *this, int modulus) {
	for (int i = 0; i < this->coeflen; i++) {
		this->coef[i] %= modulus;
	}
}






Polynomial *Poly_inverseMod2(Polynomial *this) {

	int k = 0;
	int N = this->coeflen;

	// b(X) := 1
	Polynomial *b = Poly_scalarAs(1, N + 1);
	// c(X) := 0
	Polynomial *c = Poly_scalarAs(0, N + 1);
	// f(X) := a(X) = input = this
	Polynomial *f = Poly_from_other(this->coef, N, N + 1);
	Poly_modby(f, 2);
	// g(X) := X^N - 1
	Polynomial *g = Poly_withcoeflen(N + 1);
	g->coef[0] = -1;
	g->coef[N] = 1;


	while (1) {

		while (f->coef[0] == 0) {
			for (int i = 1; i <= N; i++) {
				// divide f(X) by X
				// shift over to left
				f->coef[i - 1] = f->coef[i];
				// mul c(X) by X
				// shift to right
				c->coef[N - i + 1] = c->coef[N - i];
			}
			// finalize
			f->coef[N] = 0;
			c->coef[0] = 0;

			k++;

			if (Poly_equalsZero(f)) {
				// not invertible
				return NULL;
			}
		}

		if (Poly_equalsScalar(f, 1)) {
			// break loop, return after more calcs
			break;
		}

		if (Poly_calcDegree(f) < Poly_calcDegree(g)) {
			// swap f and g
			Polynomial *temp = f;
			f = g;
			g = temp;
			// swap b and c
			temp = b;
			b = c;
			c = temp;
		}

		// inplace adding
		Poly_add_inplace(f, g);
		Poly_modby(f, 2);
		Poly_add_inplace(b, c);
		Poly_modby(b, 2);
	}

	if (b->coef[N] != 0)
		return NULL;

	// Fq(x) = x^(N-k) * b(x)
	Polynomial *f2 = Poly_withcoeflen(N);

	k %= N;
	f2->coef[N - k] = 1;
	Polynomial *result = Poly_times(f2, b);

	// clean up
	Poly_delete(b);
	Poly_delete(c);
	Poly_delete(f);
	Poly_delete(g);
	Poly_delete(f2);
	// clean up

	return result;
}



Polynomial *Poly_mod2TomodQ(Polynomial *this, Polynomial *f2, int q) {
	int v = 2;
	Polynomial *temp;
	while (v < q) {
		v *= 2;

		temp = Poly_from_other(f2->coef, f2->coeflen, f2->coeflen);

		Poly_timesScalar(temp, 2);
		Poly_modby(temp, v);

		Polynomial *step1 = Poly_times(this, f2);
		Poly_modby(step1, v);
		Polynomial *step2 = Poly_times(step1, f2);
		Poly_modby(step2, v);

		f2 = step2;

		/*Poly_delete(step1);
		Poly_delete(f2);*/

		Poly_minus_inplace(temp, f2);
		Poly_modby(temp, v);

		//Poly_delete(step2);
		f2 = temp;
	}
	return f2;
}



Polynomial *Poly_inverseModQ(Polynomial *this, int q) {
	Polynomial *fq = Poly_inverseMod2(this);

	if (fq == NULL)
		return NULL;
	Polynomial *retval = Poly_mod2TomodQ(this, fq, q);

	Poly_delete(fq);
	return retval;
}



void Poly_ensurePositive(Polynomial *this, int modulus) {
	for (int i = 0; i < this->coeflen; i++) {
		while (this->coef[i] < 0) {
			this->coef[i] += modulus;
		}
	}
}

void Poly_center(Polynomial *this, int q) {
	for (int i = 0; i < this->coeflen; i++) {
        while (this->coef[i] < (-q / 2))
        	this->coef[i] += q;
        while (this->coef[i] > (q / 2))
        	this->coef[i] -= q;
    }
}




Polynomial *Poly_makeRandom(int N, int numOnes, int numNegOnes) {

	if (numOnes + numNegOnes > N) {
		fprintf(stderr, "error in Poly_random(), 1s + -1s > N\n");
		exit(1);
	}

	Polynomial *poly = Poly_withcoeflen(N);
	int i = 0;
	for (int j = 0; j < numOnes; j++) {
		poly->coef[i++] = 1;
	}
	for (int j = 0; j < numNegOnes; j++) {
		poly->coef[i++] = -1;
	}

	for (int i = N - 1; i > 0; i--) {
		int j = CSPRNGrandUInt() % (i + 1);

		int tmp = poly->coef[i];
		poly->coef[i] = poly->coef[j];
		poly->coef[j] = tmp;
	}

	return poly;
}


void Poly_scramble(Polynomial *this) {

	for (int i = this->coeflen - 1; i > 0; i--) {
		int j = CSPRNGrandUInt() % (i + 1);

		int tmp = this->coef[i];
		this->coef[i] = this->coef[j];
		this->coef[j] = tmp;
	}

}


void Poly_print(Polynomial *this) {
	int degree = Poly_calcDegree(this);
	if (degree == -1) {
		puts("0");
		return;
	}
	else if (degree == 0) {
		printf("%d\n", this->coef[0]);
		return;
	}
	else if (degree == 1) {
		printf("%dx + %d\n", this->coef[1], this->coef[0]);
		return;
	}

	printf("%d", this->coef[0]);
	for (int i = 1; i < this->coeflen; i++) {
		int val = this->coef[i];
		if (val == 0) {
			continue;
		}
		else if (val > 0) {
			printf(" + %d", val);
		}
		else if (val < 0) {
			printf(" - %d", -val);
		}
		if (i == 1) {
			printf("x");
		}
		else if (i > 1) {
			printf("x^%d", i);
		}
	}
	// new line
	puts("");
}
