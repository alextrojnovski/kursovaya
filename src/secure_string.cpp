#include "secure_string.h"
#include <cstring>
#include <stdexcept>

SecureString::SecureString() = default;

SecureString::SecureString(const std::string& str) {
    assign(str);
}

SecureString::SecureString(const char* str) {
    if (str) {
        assign(str, std::strlen(str));
    }
}

SecureString::SecureString(const SecureString& other) 
    : data(other.data) {}

SecureString::SecureString(SecureString&& other) noexcept 
    : data(std::move(other.data)) {}

SecureString::~SecureString() {
    clear();
}

SecureString& SecureString::operator=(const SecureString& other) {
    if (this != &other) {
        clear();
        data = other.data;
    }
    return *this;
}

SecureString& SecureString::operator=(SecureString&& other) noexcept {
    if (this != &other) {
        clear();
        data = std::move(other.data);
    }
    return *this;
}

const char* SecureString::c_str() const {
    return data.empty() ? "" : data.data();
}

size_t SecureString::length() const {
    return data.size();
}

bool SecureString::empty() const {
    return data.empty();
}

void SecureString::clear() {
    if (!data.empty()) {
        // Безопасно затираем память
        std::fill(data.begin(), data.end(), '\0');
        data.clear();
        data.shrink_to_fit();
    }
}

void SecureString::assign(const std::string& str) {
    clear();
    if (!str.empty()) {
        data.assign(str.begin(), str.end());
        data.push_back('\0'); // null-terminator
    }
}

void SecureString::assign(const char* str, size_t len) {
    clear();
    if (str && len > 0) {
        data.reserve(len + 1);
        data.assign(str, str + len);
        data.push_back('\0');
    }
}

bool SecureString::equals(const SecureString& other) const {
    if (data.size() != other.data.size()) return false;
    return std::equal(data.begin(), data.end(), other.data.begin());
}

bool SecureString::equals(const std::string& other) const {
    if (data.size() != other.length() + 1) return false; // +1 for null terminator
    return std::equal(data.begin(), data.end() - 1, other.begin());
}

std::string SecureString::toString() const {
    if (data.empty()) return "";
    return std::string(data.begin(), data.end() - 1); // Без null-terminator
}