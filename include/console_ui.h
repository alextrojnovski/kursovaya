#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include "password_manager.h"

class ConsoleUI {
private:
    PasswordManager manager;
    
    // Вспомогательные методы
    void displayMenu() const;
    void addPasswordInteraction();
    void findPasswordInteraction();
    void deletePasswordInteraction();
    void listAllPasswordsInteraction();
    void saveToFileInteraction();
    void loadFromFileInteraction();
    std::string getInput(const std::string& prompt);

public:
    void run();  // Главный метод - запуск интерфейса
};

#endif