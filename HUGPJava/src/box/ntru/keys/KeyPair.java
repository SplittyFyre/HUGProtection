package box.ntru.keys;

public class KeyPair {

	public PubKey pub;
	public PrivKey priv;
	
	public KeyPair(PubKey pub, PrivKey priv) {
		this.pub = pub;
		this.priv = priv;
	}
	
}
