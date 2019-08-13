package box.symmetric;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.SecretKeySpec;

import box.SRng;

public class TripleAES {
	
	public static void main(String args[]) throws Exception {
		
		byte[] bk1 = new byte[32];
		SRng.srng.nextBytes(bk1);
		byte[] bk2 = new byte[32];
		SRng.srng.nextBytes(bk2);
		byte[] bk3 = new byte[32];
		SRng.srng.nextBytes(bk3);
		
		byte[] iv = new byte[16];
		SRng.srng.nextBytes(iv);
		
		encryptFile(new FileInputStream(new File("plain.txt")), bk1, bk2, bk3, iv);
		decryptFile(new FileInputStream(new File("enced")), bk1, bk2, bk3, iv);
		
		System.out.println("doneit");
		
	}
	
	public static byte[] encryptFile(InputStream input, byte[] kb1, byte[] kb2, byte[] kb3, byte[] iv) throws NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException, IOException {
		
		if (kb1.length != 32) {
			throw new RuntimeException("kb1 length not 32 bytes / 256 bits");
		}
		if (kb2.length != 32) {
			throw new RuntimeException("kb2 length not 32 bytes / 256 bits");
		}
		if (kb3.length != 32) {
			throw new RuntimeException("kb3 length not 32 bytes / 256 bits");
		}
		if (iv.length != 16) {
			throw new RuntimeException("iv not 16 bytes / 128 bits");
		}
		
		
		// my own cbc and my own padding (PKCS7)
		Cipher cipher1 = Cipher.getInstance("AES/ECB/NoPadding");
		cipher1.init(Cipher.ENCRYPT_MODE, new SecretKeySpec(kb1, "AES"));
		Cipher cipher2 = Cipher.getInstance("AES/ECB/NoPadding");
		cipher2.init(Cipher.DECRYPT_MODE, new SecretKeySpec(kb2, "AES"));
		Cipher cipher3 = Cipher.getInstance("AES/ECB/NoPadding");
		cipher3.init(Cipher.ENCRYPT_MODE, new SecretKeySpec(kb3, "AES"));
		
		FileOutputStream outstream = new FileOutputStream(new File("enced"));
		
		byte[] inbuf = new byte[16];
		byte[] prevblock = iv;
		
		boolean padded = false;
		
		int len;
		while ((len = input.read(inbuf)) != -1) {
			
			if (len != 16) {
				padded = true;
				int padsneeded = 16 - len;
				for (int i = len; i < 16; i++) {
					inbuf[i] = (byte) padsneeded;
				}
			}
			
			for (int i = 0; i < 16; i++) {
				inbuf[i] ^= prevblock[i];
			}
			
			byte[] out = cipher1.update(inbuf, 0, 16);
			out = cipher2.update(out, 0, 16);
			out = cipher3.update(out, 0, 16);
			outstream.write(out);
			prevblock = Arrays.copyOf(out, 16);
		}
		
		if (!padded) {
			for (int i = 0; i < 16; i++) {
				inbuf[i] = 16;
				byte[] out = cipher1.update(inbuf, 0, 16);
				out = cipher2.update(out, 0, 16);
				out = cipher3.update(out, 0, 16);
				outstream.write(out);
			}
			
		}
		
		outstream.close();
		
		return null;
		
	}
	
	
	
	
	
	
	
	
	public static byte[] decryptFile(InputStream input, byte[] kb1, byte[] kb2, byte[] kb3, byte[] iv) throws NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException, IOException {
		
		if (kb1.length != 32) {
			throw new RuntimeException("kb1 length not 32 bytes / 256 bits");
		}
		if (kb2.length != 32) {
			throw new RuntimeException("kb2 length not 32 bytes / 256 bits");
		}
		if (kb3.length != 32) {
			throw new RuntimeException("kb3 length not 32 bytes / 256 bits");
		}
		if (iv.length != 16) {
			throw new RuntimeException("iv not 16 bytes / 128 bits");
		}
		
		
		// my own cbc and my own padding (PKCS7)
		Cipher cipher1 = Cipher.getInstance("AES/ECB/NoPadding");
		cipher1.init(Cipher.DECRYPT_MODE, new SecretKeySpec(kb1, "AES"));
		Cipher cipher2 = Cipher.getInstance("AES/ECB/NoPadding");
		cipher2.init(Cipher.ENCRYPT_MODE, new SecretKeySpec(kb2, "AES"));
		Cipher cipher3 = Cipher.getInstance("AES/ECB/NoPadding");
		cipher3.init(Cipher.DECRYPT_MODE, new SecretKeySpec(kb3, "AES"));
		
		FileOutputStream outstream = new FileOutputStream(new File("mydeced"));
		
		byte[] inbuf = new byte[16];
		byte[] prevblock = iv;
		byte[] tmp = new byte[16];
		
		
		int len;
		while ((len = input.read(inbuf)) != -1) {
			
			tmp = Arrays.copyOf(inbuf, 16);
			
			byte[] out = cipher3.update(inbuf, 0, 16);
			out = cipher2.update(out, 0, 16);
			out = cipher1.update(out, 0, 16);
			
			for (int i = 0; i < 16; i++) {
				out[i] ^= prevblock[i];
			}
			prevblock = Arrays.copyOf(tmp, 16);
			
			outstream.write(out);
			
		}
		
		
		outstream.close();
		
		return null;
		
	}

}
