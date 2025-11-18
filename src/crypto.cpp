#include "crypto.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

std::string Crypto::encrypt(const std::string& plaintext, const SecureString& password) const {
    if (password.empty()) {
        throw std::invalid_argument("Password cannot be empty");
    }
    
    std::string ciphertext = plaintext;
    const char* password_data = password.c_str();
    size_t password_len = password.length();
    
    for (size_t i = 0; i < plaintext.length(); ++i) {
        ciphertext[i] = plaintext[i] ^ password_data[i % password_len];
    }
    
    return ciphertext;
}

std::string Crypto::decrypt(const std::string& ciphertext, const SecureString& password) const {
    return encrypt(ciphertext, password);
}

std::string Crypto::generateSalt() const {
    std::srand(std::time(nullptr));
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string salt;
    
    for (int i = 0; i < 16; ++i) {
        salt += charset[std::rand() % charset.length()];
    }
    
    return salt;
}

std::string Crypto::deriveKeyFromPassword(const SecureString& password, const std::string& salt) const {
    // Пока простая реализация - позже добавим PBKDF2
    return password.toString() + salt;
}

bool Crypto::selfTest() const {
    try {
        std::string test_text = "Hello, World! This is a test message.";
        SecureString test_password("my_secret_password");
        
        std::string encrypted = encrypt(test_text, test_password);
        std::string decrypted = decrypt(encrypted, test_password);
        
        if (test_text == decrypted) {
            std::cout << "Crypto self-test: PASSED" << std::endl;
            return true;
        } else {
            std::cout << "Crypto self-test: FAILED - decryption mismatch" << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cout << "Crypto self-test: FAILED - " << e.what() << std::endl;
        return false;
    }
}