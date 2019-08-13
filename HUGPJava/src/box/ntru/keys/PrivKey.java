package box.ntru.keys;

import box.ntru.Polynomial;

public class PrivKey {
	
	public Polynomial f, g, fp;

	public PrivKey(Polynomial f, Polynomial g, Polynomial fp) {
		this.f = f;
		this.g = g;
		this.fp = fp;
	}
	
}
