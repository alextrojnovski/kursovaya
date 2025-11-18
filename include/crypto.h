#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <vector>

class Crypto {
private:
    std::string deriveKeyFromPassword(const std::string& password, const std::string& salt) const;
    
public:
    // Шифрование строки
    std::string encrypt(const std::string& plaintext, const std::string& password) const;
    
    // Дешифрование строки
    std::string decrypt(const std::string& ciphertext, const std::string& password) const;
    
    // Генерация соли
    std::string generateSalt() const;
    
    // Простая проверка работы шифрования (для тестов)
    bool selfTest() const;
};

#endif