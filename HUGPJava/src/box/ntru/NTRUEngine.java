package box.ntru;

import java.util.Arrays;

import box.ntru.keys.KeyPair;
import box.ntru.keys.PrivKey;
import box.ntru.keys.PubKey;

public class NTRUEngine {

	// P is always 3
	private static final int Q = 2048, N = 743;
			
	public static KeyPair generateKeyPair() {
		
		// generate g polynomial
		Polynomial g = null;
		
		while (true) {
			g = Polynomial.makeRandom(N, 247, 247 - 1);
			if (g.inverseMod2() != null) {
				break;
			}
		}
		
		Polynomial f = null, fq = null, t = null;
				
		// generate f
		while (true) {
			t = Polynomial.makeRandom(N, 22, 22);
			f = new Polynomial(Arrays.copyOf(t.coef, t.coef.length));
			f.times(3);
			f.addScalar(1);
			
			fq = f.inverseModQ(Q);
			if (fq != null) {
				break;
			}
		}
		
		
		// g * fq mod q
		Polynomial h = g.times(fq).mod(Q);
		h.times(3); // p is always 3
		h.mod(Q); // mod Q many
		h.ensurePositive(Q);
		
		PubKey pub = new PubKey(h);
		PrivKey priv = new PrivKey(t);

		return new KeyPair(pub, priv);
	}
	
	
	/*private static Polynomial generateBlinding() {
		
	}*/
	
	
	public static Polynomial encrypt(Polynomial msg, PubKey pub) {
		
		Polynomial r = Polynomial.makeRandom(N, 5, 5);
 
		Polynomial e = (r.times(pub.h).mod(Q));
		e = e.plus(msg).mod(Q);
		e.ensurePositive(Q);
		
		return e;
	}
	
	public static Polynomial decrypt(Polynomial cip, PrivKey priv) {
		Polynomial a = priv.ft.times(cip).mod(Q);
		a.times(3);
		a.add(cip);
		a.center(Q);
		a.mod(3);
		
		a.center(3);
		
		return a;
	}
	
	

	public static void main(String[] args) {
		
		
		KeyPair keys = generateKeyPair();
		

		//for (int i = 0; i < 100; i++) {
		Polynomial msg = Polynomial.makeRandom(N, 512, 0);
		
		Polynomial e = encrypt(msg, keys.pub);
		Polynomial d = decrypt(e, keys.priv);
		
		System.out.println(msg);
		System.out.println(e);
		System.out.println(d);
		System.out.println(msg.equals(d));
		//}
	}

	
}
