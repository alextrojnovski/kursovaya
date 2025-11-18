#include "password_manager.h"
#include <iostream>
#include <fstream>
#include <algorithm>

void PasswordManager::addEntry(const PasswordEntry& entry) {
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
        entry.print();
    }
    std::cout << "Total: " << entries.size() << " entries" << std::endl;
}

bool PasswordManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for writing." << std::endl;
        return false;
    }
    
    for (const auto& entry : entries) {
        file << entry.getServiceName() << ","
             << entry.getLogin() << ","
             << entry.getPassword() << "\n";
    }
    
    file.close();
    std::cout << "Data saved to '" << filename << "' successfully." << std::endl;
    return true;
}

bool PasswordManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for reading." << std::endl;
        return false;
    }
    
    entries.clear(); // Очищаем текущие записи
    std::string line;
    while (std::getline(file, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        
        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            std::string service = line.substr(0, pos1);
            std::string login = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string password = line.substr(pos2 + 1);
            
            entries.emplace_back(service, login, password);
        }
    }
    
    file.close();
    std::cout << "Data loaded from '" << filename << "' successfully." << std::endl;
    return true;
}

size_t PasswordManager::getEntryCount() const {
    return entries.size();
}

void PasswordManager::clearAllEntries() {
    entries.clear();
    std::cout << "All entries cleared." << std::endl;
}