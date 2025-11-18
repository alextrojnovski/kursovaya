#include "crypto.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

// Простая реализация XOR шифрования для начала
// Позже заменим на более надежное шифрование

std::string Crypto::encrypt(const std::string& plaintext, const std::string& password) {
    if (password.empty()) {
        throw std::invalid_argument("Password cannot be empty");
    }
    
    std::string ciphertext = plaintext;
    size_t password_len = password.length();
    
    for (size_t i = 0; i < plaintext.length(); ++i) {
        ciphertext[i] = plaintext[i] ^ password[i % password_len];
    }
    
    return ciphertext;
}

std::string Crypto::decrypt(const std::string& ciphertext, const std::string& password) {
    // XOR шифрование симметрично - шифрование и дешифрование одинаковы
    return encrypt(ciphertext, password);
}

std::string Crypto::generateSalt() {
    std::srand(std::time(nullptr));
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string salt;
    
    for (int i = 0; i < 16; ++i) {
        salt += charset[std::rand() % charset.length()];
    }
    
    return salt;
}

std::string Crypto::deriveKeyFromPassword(const std::string& password, const std::string& salt) {
    // Простая производная ключа - соединяем пароль и соль
    // В реальном приложении здесь должен быть PBKDF2 или аналогичный алгоритм
    return password + salt;
}

bool Crypto::selfTest() {
    try {
        std::string test_text = "Hello, World! This is a test message.";
        std::string test_password = "my_secret_password";
        
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