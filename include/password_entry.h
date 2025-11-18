#ifndef PASSWORD_ENTRY_H
#define PASSWORD_ENTRY_H

#include <string>

class PasswordEntry {
private:
    std::string service_name;
    std::string login;
    std::string password;

public:
    // Конструктор
    PasswordEntry(const std::string& service, const std::string& login, const std::string& pwd);
    
    // Геттеры
    std::string getServiceName() const;
    std::string getLogin() const;
    std::string getPassword() const;
    
    // Сеттеры
    void setServiceName(const std::string& service);
    void setLogin(const std::string& login);
    void setPassword(const std::string& pwd);
    
    // Метод для отладки
    void print() const;
};

#endif