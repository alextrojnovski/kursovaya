#ifndef SECURE_STRING_H
#define SECURE_STRING_H

#include <vector>
#include <string>
#include <algorithm>

class SecureString {
private:
    std::vector<char> data;

public:
    // Конструкторы
    SecureString();
    SecureString(const std::string& str);
    SecureString(const char* str);
    SecureString(const SecureString& other);
    SecureString(SecureString&& other) noexcept;
    
    // Деструктор - безопасно затираем память
    ~SecureString();
    
    // Операторы присваивания
    SecureString& operator=(const SecureString& other);
    SecureString& operator=(SecureString&& other) noexcept;
    
    // Методы доступа
    const char* c_str() const;
    size_t length() const;
    bool empty() const;
    
    // Безопасные операции
    void clear();
    void assign(const std::string& str);
    void assign(const char* str, size_t len);
    
    // Сравнение
    bool equals(const SecureString& other) const;
    bool equals(const std::string& other) const;
    
    // Для использования в Crypto
    std::string toString() const; // Осторожно! Создает временную копию
};

#endif