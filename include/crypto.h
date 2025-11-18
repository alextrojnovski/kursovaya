#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <vector>

class Crypto {
private:
    std::string deriveKeyFromPassword(const std::string& password, const std::string& salt);
    
public:
    // Шифрование строки
    std::string encrypt(const std::string& plaintext, const std::string& password);
    
    // Дешифрование строки
    std::string decrypt(const std::string& ciphertext, const std::string& password);
    
    // Генерация соли
    std::string generateSalt();
    
    // Простая проверка работы шифрования (для тестов)
    bool selfTest();
};

#endif