#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <vector>
#include "secure_string.h"

class Crypto {
private:
    std::string deriveKeyFromPassword(const SecureString& password, const std::string& salt) const;
    
public:
    // Шифрование строки
    std::string encrypt(const std::string& plaintext, const SecureString& password) const;
    
    // Дешифрование строки
    std::string decrypt(const std::string& ciphertext, const SecureString& password) const;
    
    // Генерация соли
    std::string generateSalt() const;
    
    // Хеширование пароля
    std::string hashPassword(const SecureString& password, const std::string& salt) const;
    
    // Простая проверка работы шифрования (для тестов)
    bool selfTest() const;
};

#endif