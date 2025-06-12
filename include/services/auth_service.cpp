#include "interface/auth_service.h"
#include <iostream>

AuthService::AuthService() {
    // Initialize with default admin account
    credentials["admin"] = "password123";
    currentUser = "";
}

Common::ServiceResult AuthService::login(const std::string& username, const std::string& password) {
    auto it = credentials.find(username);
    if (it == credentials.end() || it->second != password) {
        return Common::ServiceResult(false, "Invalid credentials");
    }
    
    currentUser = username;
    return Common::ServiceResult(true, "Login successful");
}

Common::ServiceResult AuthService::logout() {
    if (currentUser.empty()) {
        return Common::ServiceResult(false, "No user logged in");
    }
    
    currentUser = "";
    return Common::ServiceResult(true, "Logout successful");
}

Common::ServiceResult AuthService::registerUser(const std::string& username, const std::string& password) {
    if (credentials.find(username) != credentials.end()) {
        return Common::ServiceResult(false, "User already exists");
    }
    
    credentials[username] = password;
    return Common::ServiceResult(true, "User registered successfully");
}

bool AuthService::isAuthenticated() const {
    return !currentUser.empty();
}

std::string AuthService::getCurrentUser() const {
    return currentUser;
}

void AuthService::processAuthCommands() {
    std::cout << "1. Login\n2. Logout\n3. Register\n4. Check Status\n5. Back to main menu" << std::endl;
    std::cout << "Choice: ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1: {
            std::string username, password;
            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;
            auto result = login(username, password);
            std::cout << result.message << std::endl;
            break;
        }
        case 2: {
            auto result = logout();
            std::cout << result.message << std::endl;
            break;
        }
        case 3: {
            std::string username, password;
            std::cout << "New username: ";
            std::cin >> username;
            std::cout << "New password: ";
            std::cin >> password;
            auto result = registerUser(username, password);
            std::cout << result.message << std::endl;
            break;
        }
        case 4: {
            if (isAuthenticated()) {
                std::cout << "Logged in as: " << getCurrentUser() << std::endl;
            } else {
                std::cout << "Not logged in" << std::endl;
            }
            break;
        }
        case 5:
            return;
        default:
            std::cout << "Invalid choice" << std::endl;
    }
}