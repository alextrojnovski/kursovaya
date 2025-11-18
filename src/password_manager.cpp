#include "password_manager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

void PasswordManager::setMasterPassword(const SecureString& password) {
    if (!password.empty()) {
        masterPassword = password;
        std::cout << "Master password set successfully." << std::endl;
    } else {
        std::cout << "Error: Master password cannot be empty!" << std::endl;
    }
}

bool PasswordManager::isMasterPasswordSet() const {
    return !masterPassword.empty();
}

std::string PasswordManager::serializeEntries() const {
    std::stringstream ss;
    for (const auto& entry : entries) {
        ss << entry.getServiceName() << "\t"
           << entry.getLogin() << "\t"
           << entry.getPassword() << "\n";
    }
    return ss.str();
}

void PasswordManager::deserializeEntries(const std::string& data) {
    entries.clear();
    std::stringstream ss(data);
    std::string line;
    
    while (std::getline(ss, line)) {
        if (line.empty()) continue;
        
        size_t pos1 = line.find('\t');
        size_t pos2 = line.find('\t', pos1 + 1);
        
        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            std::string service = line.substr(0, pos1);
            std::string login = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string password = line.substr(pos2 + 1);
            
            entries.emplace_back(service, login, password);
        }
    }
}

void PasswordManager::addEntry(const PasswordEntry& entry) {
    // Проверяем нет ли уже записи с таким сервисом
    if (findEntry(entry.getServiceName())) {
        std::cout << "Error: Entry for service '" << entry.getServiceName() << "' already exists!" << std::endl;
        return;
    }
    
    entries.push_back(entry);
    std::cout << "Entry for service '" << entry.getServiceName() << "' added successfully." << std::endl;
}

bool PasswordManager::deleteEntry(const std::string& service_name) {
    auto it = std::remove_if(entries.begin(), entries.end(),
        [&service_name](const PasswordEntry& entry) {
            return entry.getServiceName() == service_name;
        });
    
    if (it != entries.end()) {
        entries.erase(it, entries.end());
        std::cout << "Entry for service '" << service_name << "' deleted successfully." << std::endl;
        return true;
    }
    
    std::cout << "Entry for service '" << service_name << "' not found." << std::endl;
    return false;
}

PasswordEntry* PasswordManager::findEntry(const std::string& service_name) {
    for (auto& entry : entries) {
        if (entry.getServiceName() == service_name) {
            return &entry;
        }
    }
    return nullptr;
}

void PasswordManager::listAllEntries() const {
    if (entries.empty()) {
        std::cout << "No entries stored." << std::endl;
        return;
    }
    
    std::cout << "\n=== Stored Password Entries ===" << std::endl;
    for (const auto& entry : entries) {
        std::cout << "Service: " << entry.getServiceName() 
                  << ", Login: " << entry.getLogin() << std::endl;
        // Пароль не показываем для безопасности
    }
    std::cout << "Total: " << entries.size() << " entries" << std::endl;
}

bool PasswordManager::saveToFile(const std::string& filename) const {
    if (!isMasterPasswordSet()) {
        std::cerr << "Error: Master password not set! Cannot encrypt data." << std::endl;
        return false;
    }
    
    // ПРОВЕРЯЕМ: если файл существует и НЕ принадлежит нам - ЗАПРЕЩАЕМ!
    std::ifstream test_file(filename, std::ios::binary);
    if (test_file.is_open()) {
        test_file.close();
        if (!canAccessFile(filename)) {
            std::cerr << "Error: File '" << filename << "' belongs to another master password!" << std::endl;
            std::cerr << "Cannot overwrite. Use a different filename or the correct master password." << std::endl;
            return false;
        }
    }
    
    std::string data = serializeEntries();
    std::string encryptedData = crypto.encrypt(data, masterPassword);
    
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for writing." << std::endl;
        return false;
    }
    
    file << encryptedData;
    file.close();
    std::cout << "Encrypted data saved to '" << filename << "' successfully." << std::endl;
    return true;
}
bool PasswordManager::loadFromFile(const std::string& filename) {
    if (!isMasterPasswordSet()) {
        std::cerr << "Error: Master password not set! Cannot decrypt data." << std::endl;
        return false;
    }
    
    std::cout << "DEBUG: Checking file access for: " << filename << std::endl;
    if (!canAccessFile(filename)) {
        std::cerr << "Error: File '" << filename << "' belongs to another master password!" << std::endl;
        return false;
    }
    
    std::cout << "DEBUG: Access granted, loading file..." << std::endl;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for reading." << std::endl;
        return false;
    }
    
    std::string encryptedData((std::istreambuf_iterator<char>(file)), 
                             std::istreambuf_iterator<char>());
    file.close();
    
    try {
        std::string decryptedData = crypto.decrypt(encryptedData, masterPassword);
        deserializeEntries(decryptedData);
        std::cout << "Encrypted data loaded from '" << filename << "' successfully." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to decrypt file. Wrong master password?" << std::endl;
        return false;
    }
}

size_t PasswordManager::getEntryCount() const {
    return entries.size();
}

bool PasswordManager::canAccessFile(const std::string& filename) const {
    if (!isMasterPasswordSet()) {
        return false;
    }
    
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return true; // Файл не существует - можно создать
    }
    
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    if (file_size == 0) {
        return true; // Файл пустой - можно использовать
    }
    
    std::string encryptedData((std::istreambuf_iterator<char>(file)), 
                             std::istreambuf_iterator<char>());
    file.close();
    
    try {
        std::string decryptedData = crypto.decrypt(encryptedData, masterPassword);
        
        // ДОПОЛНИТЕЛЬНАЯ ПРОВЕРКА: пробуем десериализовать данные
        // Если данные не содержат ожидаемого формата - считаем файл чужим
        if (decryptedData.empty()) {
            return false;
        }
        
        // Проверяем что расшифрованные данные содержат табы (наш формат)
        return decryptedData.find('\t') != std::string::npos;
        
    } catch (const std::exception& e) {
        // Любая ошибка при дешифровке - файл не наш
        return false;
    }
}