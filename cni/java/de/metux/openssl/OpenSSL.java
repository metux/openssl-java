package de.metux.openssl;

public final class OpenSSL {
	public static native byte[] AES256_encrypt(byte[] key, byte[] content);
	public static native byte[] AES256_decrypt(byte[] key, byte[] content);
}
