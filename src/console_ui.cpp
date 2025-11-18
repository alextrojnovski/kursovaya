
#include "console_ui.h"
#include <iostream>
#include <limits>

void ConsoleUI::displayMenu() const {
    std::cout << "\n=== Password Manager ===" << std::endl;
    std::cout << "1. Add new password" << std::endl;
    std::cout << "2. Find password" << std::endl;
    std::cout << "3. Delete password" << std::endl;
    std::cout << "4. List all passwords" << std::endl;
    std::cout << "5. Save to file" << std::endl;
    std::cout << "6. Load from file" << std::endl;
    std::cout << "7. Exit" << std::endl;
    std::cout << "Choose option (1-7): ";
}

std::string ConsoleUI::getInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

bool ConsoleUI::setupMasterPassword() {
    std::cout << "\n=== Master Password Setup ===" << std::endl;
    std::cout << "Set a master password to encrypt your data: ";
    
    std::string password;
    std::getline(std::cin, password);
    
    if (password.empty()) {
        std::cout << "Error: Master password cannot be empty!" << std::endl;
        return false;
    }
    
    SecureString securePassword(password);  // Создаем SecureString из std::string
    manager.setMasterPassword(securePassword);
    return true;
}
void ConsoleUI::addPasswordInteraction() {
    std::cout << "\n--- Add New Password ---" << std::endl;
    
    std::string service = getInput("Service name: ");
    std::string login = getInput("Login: ");
    std::string password = getInput("Password: ");
    
    if (!service.empty() && !login.empty() && !password.empty()) {
        PasswordEntry newEntry(service, login, password);
        manager.addEntry(newEntry);
    } else {
        std::cout << "Error: All fields must be filled!" << std::endl;
    }
}

void ConsoleUI::findPasswordInteraction() {
    std::cout << "\n--- Find Password ---" << std::endl;
    
    std::string service = getInput("Enter service name to find: ");
    PasswordEntry* found = manager.findEntry(service);
    
    if (found) {
        std::cout << "Found entry: ";
        std::cout << "Service: " << found->getServiceName() 
                  << ", Login: " << found->getLogin() 
                  << ", Password: " << found->getPassword() << std::endl;
    } else {
        std::cout << "Entry not found for service: " << service << std::endl;
    }
}

void ConsoleUI::deletePasswordInteraction() {
    std::cout << "\n--- Delete Password ---" << std::endl;
    
    std::string service = getInput("Enter service name to delete: ");
    manager.deleteEntry(service);
}

void ConsoleUI::listAllPasswordsInteraction() {
    std::cout << "\n--- All Passwords ---" << std::endl;
    manager.listAllEntries();
}

void ConsoleUI::saveToFileInteraction() {
    std::cout << "\n--- Save to File ---" << std::endl;
    
    std::string filename = getInput("Enter filename (default: passwords.dat): ");
    if (filename.empty()) {
        filename = "passwords.dat";
    }
    
    // Проверяем доступ перед сохранением
    if (!manager.canAccessFile(filename)) {
        std::cout << "❌ Access denied! This file belongs to another master password." << std::endl;
        std::cout << "💡 Tip: Use a different filename or switch to the correct master password." << std::endl;
        return;
    }
    
    if (manager.saveToFile(filename)) {
        std::cout << "✅ Data saved successfully!" << std::endl;
    }
}

void ConsoleUI::loadFromFileInteraction() {
    std::cout << "\n--- Load from File ---" << std::endl;
    
    std::string filename = getInput("Enter filename (default: passwords.dat): ");
    if (filename.empty()) {
        filename = "passwords.dat";
    }
    
    // Сначала проверяем доступ
    if (!manager.canAccessFile(filename)) {
        std::cout << "❌ ACCESS DENIED! This file belongs to another master password." << std::endl;
        std::cout << "💡 Use the correct master password or a different filename." << std::endl;
        return;
    }
    
    // Затем пытаемся загрузить
    if (manager.loadFromFile(filename)) {
        std::cout << "✅ Data loaded successfully!" << std::endl;
    } else {
        std::cout << "❌ Failed to load data from file." << std::endl;
    }
}
void ConsoleUI::run() {
    std::cout << "Welcome to Password Manager!" << std::endl;
    
    // Запрашиваем мастер-пароль при запуске
    if (!setupMasterPassword()) {
        std::cout << "Failed to set master password. Exiting." << std::endl;
        return;
    }
    
    bool running = true;
    while (running) {
        displayMenu();
        
        std::string choice;
        std::getline(std::cin, choice);
        
        switch (choice[0]) {
            case '1':
                addPasswordInteraction();
                break;
            case '2':
                findPasswordInteraction();
                break;
            case '3':
                deletePasswordInteraction();
                break;
            case '4':
                listAllPasswordsInteraction();
                break;
            case '5':
                saveToFileInteraction();
                break;
            case '6':
                loadFromFileInteraction();
                break;
            case '7':
                std::cout << "Goodbye!" << std::endl;
                running = false;
                break;
            default:
                std::cout << "Invalid option! Please choose 1-7." << std::endl;
                break;
        }
    }
}