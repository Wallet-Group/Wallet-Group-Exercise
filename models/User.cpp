#include "User.h"

User::User(const std::string& username, const std::string& password, UserRole role)
    : username(username), password(password), role(role) {}

std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

UserRole User::getRole() const {
    return role;
}

void User::setPassword(const std::string& newPassword) {
    password = newPassword;
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