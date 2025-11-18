#include "password_entry.h"
#include <iostream>

PasswordEntry::PasswordEntry(const std::string& service, const std::string& login, const std::string& pwd)
    : service_name(service), login(login), password(pwd) {}

std::string PasswordEntry::getServiceName() const {
    return service_name;
}

std::string PasswordEntry::getLogin() const {
    return login;
}

std::string PasswordEntry::getPassword() const {
    return password;
}

void PasswordEntry::setServiceName(const std::string& service) {
    service_name = service;
}

void PasswordEntry::setLogin(const std::string& login) {
    this->login = login;
}

void PasswordEntry::setPassword(const std::string& pwd) {
    password = pwd;
}

void PasswordEntry::print() const {
    std::cout << "Service: " << service_name 
              << ", Login: " << login 
              << ", Password: " << password << std::endl;
}