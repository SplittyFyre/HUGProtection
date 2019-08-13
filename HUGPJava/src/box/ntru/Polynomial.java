package box.ntru;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import box.SRng;

public class Polynomial {
	
    public int[] coef;   // coefficients p(x) = sum { coef[i] * x^i }

    /**
     * Initializes a new polynomial a x^b
     * @param a the leading coefficient
     * @param b the exponent
     * @throws IllegalArgumentException if {@code b} is negative
     */
    public Polynomial(int a, int b) {
        if (b < 0) {
            throw new IllegalArgumentException("exponent cannot be negative: " + b);
        }
        coef = new int[b + 1];
        coef[b] = a;
    }
    
    public Polynomial(int[] coefficients) {
    	this.coef = coefficients;
    }
    
    
    public Polynomial(int coeflen) {
    	this.coef = new int[coeflen];
    }
    
    
    public static Polynomial scalarAsPoly(int scalar, int coeflen) {
    	Polynomial retval = new Polynomial(coeflen);
    	retval.coef[0] = scalar;
    	return retval;
    }
    
    
    public static Polynomial makeRandom(int N, int numOnes, int numNegOnes) {
    	
    	if (numOnes + numNegOnes > N) {
    		throw new RuntimeException("numOnes + numNegOnes > N, and N - 1 is the max degree");
    	}
    	
    	List<Integer> coes = new ArrayList<Integer>();
    	
    	for (int i = 0; i < numOnes; i++) {
    		coes.add(1);
    	}
    	for (int i = 0; i < numNegOnes; i++) {
    		coes.add(-1);
    	}
    	
    	// max degree is N - 1
    	// coes will end up with size N, index 0 is for term with degree 0
    	while (coes.size() < N) {
    		coes.add(0);
    	}
    	
    	Collections.shuffle(coes, SRng.srng);
    	    	
    	int[] array = new int[coes.size()];
    	for (int i = 0; i < coes.size(); i++) {
    		array[i] = coes.get(i);
    	}
    	
    	return new Polynomial(array);
    }


    public int calculateDegree() {
        for (int i = coef.length - 1; i >= 0; i--) {
            if (coef[i] != 0) {
                return i;
            }
        }
        return -1;
    }

    

    public Polynomial plus(Polynomial that) {
    	int thisdeg = this.calculateDegree();
    	int thatdeg = that.calculateDegree();
        Polynomial poly = new Polynomial(0, Math.max(thisdeg, thatdeg));
        for (int i = 0; i < this.coef.length; i++) poly.coef[i] += this.coef[i];
        for (int i = 0; i < that.coef.length; i++) poly.coef[i] += that.coef[i];
        return poly;
    }
    
    public void add(Polynomial that) {
    	for (int i = 0; i < that.coef.length; i++)
            this.coef[i] += that.coef[i];
    }
    
    public void addScalar(int scalar) {
    	this.coef[0] += scalar;
    }
    
    public void center(int q) {
    	for (int i=0; i<coef.length; i++) {
            while (coef[i] < -q/2)
                coef[i] += q;
            while (coef[i] > q/2)
                coef[i] -= q;
        }
    }

    
    public Polynomial minus(Polynomial that) {
        Polynomial poly = new Polynomial(0, Math.max(this.calculateDegree(), that.calculateDegree()));
        for (int i = 0; i < this.coef.length; i++) poly.coef[i] += this.coef[i];
        for (int i = 0; i < that.coef.length; i++) poly.coef[i] -= that.coef[i];
        return poly;
    }
    
    public void minus_inplace(Polynomial that) {
    	for (int i = 0; i < this.coef.length; i++) {
    		this.coef[i] -= that.coef[i];
    	}
    }

    

    public Polynomial times(Polynomial that) {
    	int thisdeg = this.calculateDegree();
    	int thatdeg = that.calculateDegree();
        Polynomial poly = new Polynomial(0, thisdeg + thatdeg);
        for (int i = 0; i <= thisdeg; i++)
            for (int j = 0; j <= thatdeg; j++)
                poly.coef[i+j] += (this.coef[i] * that.coef[j]);

        
        if (poly.coef.length > this.coef.length) {
            for (int k = this.coef.length; k < poly.coef.length; k++)
                poly.coef[k - this.coef.length] += poly.coef[k];
            poly.coef = Arrays.copyOf(poly.coef, this.coef.length);
        }
        return poly;
    }
    
    public void times(int x) {
    	for (int i = 0; i < this.coef.length; i++) {
    		this.coef[i] *= x;
    	}
    }


    
    
    public Polynomial inverseMod2() {
    	    	
    	int k = 0;
    	
    	// b(X) := 1
    	Polynomial b = Polynomial.scalarAsPoly(1, this.coef.length + 1);
    	// c(X) := 0
    	Polynomial c = Polynomial.scalarAsPoly(0, this.coef.length + 1);
    	// f(X) := a(X) = input = this
    	Polynomial f = new Polynomial(Arrays.copyOf(this.coef, this.coef.length + 1));
    	f.mod(2);
    	// g(X) := X^N - 1
    	Polynomial g = new Polynomial(this.coef.length + 1);
    	g.coef[0] = -1;
    	g.coef[this.coef.length] = 1;
    	
    	
    	// loop
    	while (true) {
    		    		
    		while (f.coef[0] == 0) {
    			for (int i = 1; i <= this.coef.length; i++) {
    				// divide f(X) by X
    				// shift over to left
                    f.coef[i - 1] = f.coef[i]; 
                    // mul c(X) by X
                    // shift to right
                    c.coef[this.coef.length + 1 - i] = c.coef[this.coef.length - i];
                }
    			// finalize
    			f.coef[this.coef.length] = 0;
                c.coef[0] = 0;
                
                k++;
                
                if (f.equalsZero()) {
                	// not invertible
                    return null;
                }
    		}
    		
    		if (f.equalsScalar(1)) {
    			// break loop, and return after more stuff
    			break;
    		}
    		
    		if (f.calculateDegree() < g.calculateDegree()) {
    			// swap f and g
                Polynomial temp = f;
                f = g;
                g = temp;
                // swap b and c
                temp = b;
                b = c;
                c = temp;
    		}
    		
    		// inplace adding
    		f.add(g);
            f.mod(2);
            b.add(c);
            b.mod(2);
            
            // goto loop
    		
    	}
    	

        if (b.coef[this.coef.length] != 0)
            return null; // not invertible
        
        // Fq(x) = x^(N-k) * b(x)
        Polynomial f2 = new Polynomial(this.coef.length);
        
        /*int j = 0;
        k %= this.coef.length;
        for (int i = this.coef.length - 1; i >= 0; i--) {
            j = i - k;
            if (j < 0)
                j += this.coef.length;
            f2.coef[j] = b.coef[i];
        }*/
        
        k %= this.coef.length;
        f2.coef[this.coef.length - k ] = 1;
        f2 = f2.times(b);
        
        return f2;
    	
    }

	public Polynomial inverseModQ(int q) {
    	Polynomial fq = inverseMod2();
    	if (fq == null) {
    		return null;
    	}
    	return mod2TomodQ(fq, q);
    }
    
    
    public Polynomial mod2TomodQ(Polynomial f2, int q) {
    	int v = 2;
    	while (v < q) {
    		v *= 2;
    		Polynomial temp = new Polynomial(f2.coef.clone());
    		temp.times(2);
    		temp.mod(v);
    		f2 = this.times(f2).mod(v).times(f2).mod(v);
    		temp.minus_inplace(f2);
    		temp.mod(v);
    		f2 = temp;
    	}
    	return f2;
    }
    
    
    public Polynomial mod(int modulus) {
    	for (int i = 0; i < this.coef.length; i++) {
    		this.coef[i] %= modulus;
    	}
    	return this;
    }
    
    private boolean equalsZero() {
        for (int i : this.coef)
            if (i != 0)
                return false;
        return true;
    }
    
    private boolean equalsScalar(int scalar) {
    	for (int i = 1; i < coef.length; i++)
            if (coef[i] != 0)
                return false;
    	return coef[0] == scalar;
    }
    
    public void ensurePositive(int modulus) {
    	for (int i = 0; i < this.coef.length; i++) {
    		while (this.coef[i] < 0) {
    			this.coef[i] += modulus;
    		}
    	}
    }
    

    @Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Polynomial other = (Polynomial) obj;
		return Arrays.equals(coef, other.coef);
	}

    
    
    @Override
    public String toString() {
    	int degree = this.calculateDegree();
        if      (degree == -1) return "0";
        else if (degree ==  0) return "" + coef[0];
        else if (degree ==  1) return coef[1] + "x + " + coef[0];
        String s = coef[degree] + "x^" + degree;
        for (int i = degree - 1; i >= 0; i--) {
            if      (coef[i] == 0) continue;
            else if (coef[i]  > 0) s = s + " + " + (coef[i]);
            else if (coef[i]  < 0) s = s + " - " + (-coef[i]);
            if      (i == 1) s = s + "x";
            else if (i >  1) s = s + "x^" + i;
        }
        return s;
    }

}
