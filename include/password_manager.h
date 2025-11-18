#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <vector>
#include <string>
#include "password_entry.h"
#include "crypto.h"

class PasswordManager {
private:
    std::vector<PasswordEntry> entries;
    Crypto crypto;
    std::string masterPassword;
    
    // Вспомогательные методы для работы с зашифрованными данными
    std::string serializeEntries() const;
    void deserializeEntries(const std::string& data);
    
public:
    // Устанавливаем мастер-пароль
    void setMasterPassword(const std::string& password);
    
    // Основные методы управления паролями
    void addEntry(const PasswordEntry& entry);
    bool deleteEntry(const std::string& service_name);
    PasswordEntry* findEntry(const std::string& service_name);
    void listAllEntries() const;
    
    // Работа с файлом (теперь с шифрованием)
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    
    // Вспомогательные методы
    size_t getEntryCount() const;
    void clearAllEntries();
    bool isMasterPasswordSet() const;
};

#endif