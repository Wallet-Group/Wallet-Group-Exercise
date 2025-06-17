#pragma once
#include <string>

enum class UserRole {
    ADMIN,
    USER
};

class User {
private:
    std::string username;
    std::string password;
    UserRole role;

public:
    User(const std::string& username, const std::string& password, UserRole role);
    
    // Getters
    std::string getUsername() const;
    std::string getPassword() const;
    UserRole getRole() const;
    
    // Setters
    void setPassword(const std::string& newPassword);
    void setRole(UserRole newRole);
    
    // Edit methods
    bool updatePassword(const std::string& oldPassword, const std::string& newPassword);
    bool updateUsername(const std::string& newUsername);
}; 