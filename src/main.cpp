#include <iostream>
#include "password_entry.h"

int main() {
    std::cout << "Password Manager started!" << std::endl;
    
    // Простой тест
    PasswordEntry test("github", "user@example.com", "password123");
    test.print();
    
    return 0;
}