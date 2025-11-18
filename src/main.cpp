#include <iostream>
#include "console_ui.h"
#include "crypto.h"

int main() {
    // Тест шифрования
    Crypto crypto;
    if (crypto.selfTest()) {
        std::cout << "Crypto system ready!" << std::endl;
    } else {
        std::cout << "Crypto system has issues!" << std::endl;
        return 1;
    }
    
    // Запускаем основной интерфейс
    ConsoleUI ui;
    ui.run();
    return 0;
}