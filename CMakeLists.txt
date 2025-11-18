cmake_minimum_required(VERSION 3.15)
project(PasswordManager VERSION 1.0.0 LANGUAGES CXX)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем debug информацию
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
endif()

# Подключаем подпапки
include_directories(include)

# Добавляем исполняемый файл
add_executable(PasswordManager
    src/main.cpp
    src/password_entry.cpp
    src/password_manager.cpp  # Добавляем новый файл
)
# Для будущих зависимостей (шифрование, тестирование)
# find_package() добавим позже
