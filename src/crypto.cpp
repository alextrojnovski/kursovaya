#include "crypto.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>

// Простая реализация XOR шифрования для начала

std::string Crypto::encrypt(const std::string& plaintext, const SecureString& password) const {
    if (password.empty()) {
        throw std::invalid_argument("Password cannot be empty");
    }
    
    // Используем хешированный пароль вместо прямого использования
    std::string salt = generateSalt();
    std::string hashedPassword = hashPassword(password, salt);
    
    std::string ciphertext = plaintext;
    size_t password_len = hashedPassword.length();
    
    for (size_t i = 0; i < plaintext.length(); ++i) {
        ciphertext[i] = plaintext[i] ^ hashedPassword[i % password_len];
    }
    
    // Добавляем соль в начало зашифрованных данных
    return salt + ":" + ciphertext;
}

std::string Crypto::decrypt(const std::string& ciphertext, const SecureString& password) const {
    if (password.empty()) {
        throw std::invalid_argument("Password cannot be empty");
    }
    
    // Извлекаем соль и зашифрованные данные
    size_t separator_pos = ciphertext.find(':');
    if (separator_pos == std::string::npos) {
        throw std::invalid_argument("Invalid ciphertext format");
    }
    
    std::string salt = ciphertext.substr(0, separator_pos);
    std::string encrypted_data = ciphertext.substr(separator_pos + 1);
    
    // Хешируем пароль с той же солью
    std::string hashedPassword = hashPassword(password, salt);
    size_t password_len = hashedPassword.length();
    
    std::string plaintext = encrypted_data;
    for (size_t i = 0; i < encrypted_data.length(); ++i) {
        plaintext[i] = encrypted_data[i] ^ hashedPassword[i % password_len];
    }
    
    return plaintext;
}

std::string Crypto::generateSalt() const {
    std::srand(std::time(nullptr));
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string salt;
    
    for (int i = 0; i < 32; ++i) {  // Увеличиваем длину соли
        salt += charset[std::rand() % charset.length()];
    }
    
    return salt;
}

std::string Crypto::hashPassword(const SecureString& password, const std::string& salt) const {
    if (password.empty() || salt.empty()) {
        throw std::invalid_argument("Password and salt cannot be empty");
    }
    
    // Упрощенная реализация PBKDF2-like хеширования
    std::string input = password.toString() + salt;
    std::string hashed = input;
    
    // Многократное "хеширование" (1000 итераций)
    for (int iteration = 0; iteration < 1000; ++iteration) {
        std::string new_hashed = hashed;
        
        // Применяем XOR с различными комбинациями
        for (size_t i = 0; i < hashed.length(); ++i) {
            char c = hashed[i];
            // Создаем сложную трансформацию
            c = c ^ input[(i + iteration) % input.length()];
            c = c ^ salt[i % salt.length()];
            c = c ^ (char)(iteration % 256);
            c = c ^ (char)(i % 256);
            new_hashed[i] = c;
        }
        
        hashed = new_hashed;
        
        // Добавляем "соль" в каждой итерации
        if (iteration % 10 == 0) {
            hashed = hashed + salt;
        }
    }
    
    return hashed;
}

std::string Crypto::deriveKeyFromPassword(const SecureString& password, const std::string& salt) const {
    return hashPassword(password, salt);
}

bool Crypto::selfTest() const {
    try {
        std::string test_text = "Hello, World! This is a test message.";
        SecureString test_password("my_secret_password");
        
        std::string encrypted = encrypt(test_text, test_password);
        std::string decrypted = decrypt(encrypted, test_password);
        
        if (test_text == decrypted) {
            std::cout << "Crypto self-test: PASSED" << std::endl;
            
            // Дополнительный тест хеширования
            std::string salt = generateSalt();
            std::string hash1 = hashPassword(test_password, salt);
            std::string hash2 = hashPassword(test_password, salt);
            
            if (hash1 == hash2) {
                std::cout << "Hash consistency test: PASSED" << std::endl;
            } else {
                std::cout << "Hash consistency test: FAILED" << std::endl;
                return false;
            }
            
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