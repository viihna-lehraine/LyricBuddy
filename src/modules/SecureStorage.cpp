// File: src/modules/SecureStorage.cpp

#include "../headers/SecureStorage.h"

namespace SecureStorage
{
	bool encryptKey(const std::string &key, const std::string &password, const std::string &filePath)
	{
		unsigned char salt[8];

		if (!RAND_bytes(salt, sizeof(salt)))
		{
			std::cerr << "Failed to generate salt.\n";

			return false;
		}

		unsigned char keyBytes[32], iv[16];

		if (!EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), salt,
							(unsigned char *)password.c_str(), password.size(), 1, keyBytes, iv))
		{
			std::cerr << "Failed to derive key and IV.\n";

			return false;
		}

		EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

		if (!ctx)
		{
			std::cerr << "Failed to create cipher context.\n";

			return false;
		}

		if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, keyBytes, iv))
		{
			EVP_CIPHER_CTX_free(ctx);

			std::cerr << "Failed to initialize encryption.\n";

			return false;
		}

		std::vector<unsigned char> encrypted(key.size() + AES_BLOCK_SIZE);

		int len, ciphertext_len;

		if (!EVP_EncryptUpdate(ctx, encrypted.data(), &len, (unsigned char *)key.c_str(), key.size()))
		{
			EVP_CIPHER_CTX_free(ctx);

			std::cerr << "Failed to encrypt data.\n";

			return false;
		}
		ciphertext_len = len;

		if (!EVP_EncryptFinal_ex(ctx, encrypted.data() + len, &len))
		{
			EVP_CIPHER_CTX_free(ctx);

			std::cerr << "Failed to finalize encryption.\n";

			return false;
		}
		ciphertext_len += len;

		EVP_CIPHER_CTX_free(ctx);

		// write salt + encrypted data to file
		std::ofstream outFile(filePath, std::ios::binary);

		if (!outFile)
		{
			std::cerr << "Failed to open file for writing.\n";

			return false;
		}

		outFile.write((char *)salt, sizeof(salt));
		outFile.write((char *)encrypted.data(), ciphertext_len);
		outFile.close();

		return true;
	}

	bool decryptKey(const std::string &password, const std::string &filePath, std::string &key)
	{
		std::ifstream inFile(filePath, std::ios::binary);
		if (!inFile)
		{
			std::cerr << "Failed to open file for reading.\n";

			return false;
		}

		unsigned char salt[8];

		inFile.read((char *)salt, sizeof(salt));

		std::vector<unsigned char> encrypted((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

		inFile.close();

		unsigned char keyBytes[32], iv[16];

		if (!EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), salt,
							(unsigned char *)password.c_str(), password.size(), 1, keyBytes, iv))
		{
			std::cerr << "Failed to derive key and IV.\n";

			return false;
		}

		EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

		if (!ctx)
		{
			std::cerr << "Failed to create cipher context.\n";

			return false;
		}

		if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, keyBytes, iv))
		{
			EVP_CIPHER_CTX_free(ctx);

			std::cerr << "Failed to initialize decryption.\n";

			return false;
		}

		std::vector<unsigned char> decrypted(encrypted.size());

		int len, plaintext_len;

		if (!EVP_DecryptUpdate(ctx, decrypted.data(), &len, encrypted.data(), encrypted.size()))
		{
			EVP_CIPHER_CTX_free(ctx);

			std::cerr << "Failed to decrypt data.\n";

			return false;
		}

		plaintext_len = len;

		if (!EVP_DecryptFinal_ex(ctx, decrypted.data() + len, &len))
		{
			EVP_CIPHER_CTX_free(ctx);

			std::cerr << "Failed to finalize decryption.\n";

			return false;
		}

		plaintext_len += len;

		EVP_CIPHER_CTX_free(ctx);

		key.assign(decrypted.begin(), decrypted.begin() + plaintext_len);

		return true;
	}
}
