#include "interface/user_service.h"
#include <iostream>
#include <algorithm>

UserService::UserService() {
    // Initialize with some default users
    users.push_back("admin");
    users.push_back("guest");
}

Common::ServiceResult UserService::createUser(const std::string& username) {
    auto it = std::find(users.begin(), users.end(), username);
    if (it != users.end()) {
        return Common::ServiceResult(false, "User already exists");
    }
    
    users.push_back(username);
    return Common::ServiceResult(true, "User created successfully");
}

Common::ServiceResult UserService::deleteUser(const std::string& username) {
    auto it = std::find(users.begin(), users.end(), username);
    if (it == users.end()) {
        return Common::ServiceResult(false, "User not found");
    }
    
    users.erase(it);
    return Common::ServiceResult(true, "User deleted successfully");
}

std::vector<std::string> UserService::listUsers() const {
    return users;
}

void UserService::processUserCommands() {
    std::cout << "1. Create User\n2. Delete User\n3. List Users\n4. Back to main menu" << std::endl;
    std::cout << "Choice: ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1: {
            std::string username;
            std::cout << "Enter username: ";
            std::cin >> username;
            auto result = createUser(username);
            std::cout << result.message << std::endl;
            break;
        }
        case 2: {
            std::string username;
            std::cout << "Enter username to delete: ";
            std::cin >> username;
            auto result = deleteUser(username);
            std::cout << result.message << std::endl;
            break;
        }
        case 3: {
            std::cout << "Users:" << std::endl;
            for (const auto& user : listUsers()) {
                std::cout << "- " << user << std::endl;
            }
            break;
        }
        case 4:
            return;
        default:
            std::cout << "Invalid choice" << std::endl;
    }
}