#include "User.h"

User::User(const std::string& username, const std::string& password, const std::string& name, int yearOfBirth, UserRole role)
    : username(username), password(password), name(name), yearOfBirth(yearOfBirth), role(role) {}

std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

std::string User::getName() const {
    return name;
}

int User::getYearOfBirth() const {
    return yearOfBirth;
}

UserRole User::getRole() const {
    return role;
}

void User::setPassword(const std::string& newPassword) {
    password = newPassword;
}

void User::setName(const std::string& newName) {
    name = newName;
}

void User::setYearOfBirth(int newYearOfBirth) {
    yearOfBirth = newYearOfBirth;
}

void User::setRole(UserRole newRole) {
    role = newRole;
}

bool User::updatePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (password != oldPassword) {
        return false;
    }
    password = newPassword;
    return true;
}

bool User::updateUsername(const std::string& newUsername) {
    if (newUsername.empty()) {
        return false;
    }
    username = newUsername;
    return true;
}