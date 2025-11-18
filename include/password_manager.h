#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <vector>
#include <string>
#include "password_entry.h"

class PasswordManager {
private:
    std::vector<PasswordEntry> entries;

public:
    // Основные методы управления паролями
    void addEntry(const PasswordEntry& entry);
    bool deleteEntry(const std::string& service_name);
    PasswordEntry* findEntry(const std::string& service_name);
    void listAllEntries() const;
    
    // Работа с файлом (пока без шифрования)
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    
    // Вспомогательные методы
    size_t getEntryCount() const;
    void clearAllEntries();
};

#endif