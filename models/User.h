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
    std::string name;
    int yearOfBirth;
    UserRole role;

public:
    User(const std::string& username, const std::string& password, const std::string& name, int yearOfBirth, UserRole role);
    
    // Getters
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getName() const;
    int getYearOfBirth() const;
    UserRole getRole() const;
    
    // Setters
    void setPassword(const std::string& newPassword);
    void setName(const std::string& newName);
    void setYearOfBirth(int newYearOfBirth);
    void setRole(UserRole newRole);
    
    // Edit methods
    bool updatePassword(const std::string& oldPassword, const std::string& newPassword);
    bool updateUsername(const std::string& newUsername);
}; 