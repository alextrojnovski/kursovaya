#include "crypto.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>

// Простая но эффективная хэш-функция
std::string simpleHash(const std::string& input) {
    // Константы для хэширования (простые числа)
    const unsigned int prime1 = 16777619;
    const unsigned int prime2 = 2166136261U;
    
    unsigned int hash = prime2;
    
    for (char c : input) {
        hash ^= static_cast<unsigned char>(c);
        hash *= prime1;
    }
    
    // Конвертируем хэш в строку
    std::string result;
    for (int i = 0; i < 16; ++i) {
        unsigned char byte = (hash >> (i * 2)) & 0xFF;
        result += "0123456789ABCDEF"[byte / 16];
        result += "0123456789ABCDEF"[byte % 16];
    }
    
    return result;
}

std::string Crypto::encrypt(const std::string& plaintext, const SecureString& password) const {
    if (password.empty()) {
        throw std::invalid_argument("Password cannot be empty");
    }
    
    // Генерируем соль и создаем настоящий ключ
    std::string salt = generateSalt();
    std::string key = hashPassword(password, salt);
    
    std::string ciphertext = plaintext;
    size_t key_len = key.length();
    
    for (size_t i = 0; i < plaintext.length(); ++i) {
        ciphertext[i] = plaintext[i] ^ key[i % key_len];
    }
    
    return salt + ":" + ciphertext;
}

std::string Crypto::decrypt(const std::string& ciphertext, const SecureString& password) const {
    if (password.empty()) {
        throw std::invalid_argument("Password cannot be empty");
    }
    
    size_t separator_pos = ciphertext.find(':');
    if (separator_pos == std::string::npos) {
        throw std::invalid_argument("Invalid ciphertext format: no salt separator");
    }
    
    std::string salt = ciphertext.substr(0, separator_pos);
    std::string encrypted_data = ciphertext.substr(separator_pos + 1);
    
    if (encrypted_data.empty()) {
        throw std::invalid_argument("Invalid ciphertext: no encrypted data");
    }
    
    // Создаем ключ для дешифровки
    std::string key = hashPassword(password, salt);
    size_t key_len = key.length();
    
    std::string plaintext = encrypted_data;
    for (size_t i = 0; i < encrypted_data.length(); ++i) {
        plaintext[i] = encrypted_data[i] ^ key[i % key_len];
    }
    
    return plaintext;
}

std::string Crypto::generateSalt() const {
    std::srand(std::time(nullptr));
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string salt;
    
    for (int i = 0; i < 32; ++i) {
        salt += charset[std::rand() % charset.length()];
    }
    
    return salt;
}

std::string Crypto::hashPassword(const SecureString& password, const std::string& salt) const {
    if (password.empty() || salt.empty()) {
        throw std::invalid_argument("Password and salt cannot be empty");
    }
    
    // Используем настоящую хэш-функцию
    std::string input = password.toString() + salt;
    
    // Многократное хэширование для усиления безопасности
    std::string hashed = simpleHash(input);
    for (int i = 0; i < 100; ++i) {
        hashed = simpleHash(hashed + salt);
    }
    
    return hashed;
}

bool Crypto::selfTest() const {
    try {
        std::string test_text = "github\tuser@example.com\tpassword123\n";
        SecureString test_password("my_secret_password");
        
        std::string encrypted = encrypt(test_text, test_password);
        std::string decrypted = decrypt(encrypted, test_password);
        
        if (test_text == decrypted) {
            std::cout << "Crypto self-test: PASSED" << std::endl;
            
            // Тестируем что разные пароли дают РАЗНЫЕ хэши
            std::string salt = generateSalt();
            std::string hash1 = hashPassword(SecureString("password1"), salt);
            std::string hash2 = hashPassword(SecureString("password2"), salt);
            
            if (hash1 != hash2) {
                std::cout << "Hash uniqueness test: PASSED" << std::endl;
                return true;
            } else {
                std::cout << "Hash uniqueness test: FAILED - same hash for different passwords!" << std::endl;
                return false;
            }
        } else {
            std::cout << "Crypto self-test: FAILED - decryption mismatch" << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cout << "Crypto self-test: FAILED - " << e.what() << std::endl;
        return false;
    }
}