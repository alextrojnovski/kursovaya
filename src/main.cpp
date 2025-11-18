#include <iostream>
#include "password_manager.h"

int main() {
    std::cout << "=== Password Manager Test ===" << std::endl;
    
    // Создаем менеджер паролей
    PasswordManager manager;
    
    // Тестируем добавление записей
    PasswordEntry entry1("github", "user1@example.com", "pass123");
    PasswordEntry entry2("gmail", "user2@gmail.com", "qwerty");
    PasswordEntry entry3("yandex", "user3@yandex.ru", "secret");
    
    manager.addEntry(entry1);
    manager.addEntry(entry2);
    manager.addEntry(entry3);
    
    // Тестируем вывод всех записей
    std::cout << "\n=== All Entries ===" << std::endl;
    manager.listAllEntries();
    
    // Тестируем поиск
    std::cout << "\n=== Search Test ===" << std::endl;
    PasswordEntry* found = manager.findEntry("github");
    if (found) {
        std::cout << "Found: ";
        found->print();
    }
    
    // Тестируем удаление
    std::cout << "\n=== Delete Test ===" << std::endl;
    manager.deleteEntry("gmail");
    
    // Показываем результат после удаления
    std::cout << "\n=== After Deletion ===" << std::endl;
    manager.listAllEntries();
    
    // Тестируем сохранение в файл
    std::cout << "\n=== File Operations ===" << std::endl;
    manager.saveToFile("test_passwords.csv");
    
    // Тестируем загрузку из файла
    PasswordManager manager2;
    manager2.loadFromFile("test_passwords.csv");
    std::cout << "Loaded entries: " << manager2.getEntryCount() << std::endl;
    
    std::cout << "\n=== Test Completed ===" << std::endl;
    return 0;
}