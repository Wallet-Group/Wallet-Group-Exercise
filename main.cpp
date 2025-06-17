#include <iostream>
#include <string>
#include "interface/UserService.h"

void printMainMenu() {
    std::cout << "\n=== Authentication System ===\n";
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. Exit\n";
    std::cout << "Choose an option: ";
}

void printUserMenu(const std::string& username) {
    std::cout << "\n=== User Menu (" << username << ") ===\n";
    std::cout << "1. Edit Username\n";
    std::cout << "2. Change Password\n";
    std::cout << "3. Logout\n";
    std::cout << "Choose an option: ";
}

UserRole getRoleFromInput() {
    std::cout << "Select role (1 for ADMIN, 2 for USER): ";
    int roleChoice;
    std::cin >> roleChoice;
    std::cin.ignore(); // Clear the newline character
    
    return (roleChoice == 1) ? UserRole::ADMIN : UserRole::USER;
}

void handleUserEdit(UserService& UserService, const std::shared_ptr<User>& currentUser) {
    int choice;
    std::string input;
    
    while (true) {
        printUserMenu(currentUser->getUsername());
        std::cin >> choice;
        std::cin.ignore(); // Clear the newline character

        switch (choice) {
            case 1: { // Edit Username
                std::cout << "Enter new username: ";
                std::getline(std::cin, input);
                
                if (UserService.updateUsername(currentUser->getUsername(), input)) {
                    std::cout << "Username updated successfully!\n";
                    // Update the current user's username in memory
                    currentUser->updateUsername(input);
                } else {
                    std::cout << "Failed to update username. It might be taken.\n";
                }
                break;
            }
            case 2: { // Change Password
                std::string oldPassword, newPassword;
                std::cout << "Enter current password: ";
                std::getline(std::cin, oldPassword);
                std::cout << "Enter new password: ";
                std::getline(std::cin, newPassword);
                
                if (UserService.updateUserPassword(currentUser->getUsername(), oldPassword, newPassword)) {
                    std::cout << "Password updated successfully!\n";
                } else {
                    std::cout << "Failed to update password. Check your current password.\n";
                }
                break;
            }
            case 3: // Logout
                return;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
}

int main() {
    UserService UserService;
    int choice;
    std::string username, password;
    std::shared_ptr<User> currentUser;

    while (true) {
        if (!currentUser) {
            printMainMenu();
            std::cin >> choice;
            std::cin.ignore(); // Clear the newline character

            switch (choice) {
                case 1: { // Register
                    std::cout << "Enter username: ";
                    std::getline(std::cin, username);
                    std::cout << "Enter password: ";
                    std::getline(std::cin, password);
                    
                    // UserRole role = getRoleFromInput();
                    
                    if (UserService.registerUser(username, password, UserRole::USER)) {
                        std::cout << "Registration successful!\n";
                    } else {
                        std::cout << "Registration failed. Username might be taken.\n";
                    }
                    break;
                }
                case 2: { // Login
                    std::cout << "Enter username: ";
                    std::getline(std::cin, username);
                    std::cout << "Enter password: ";
                    std::getline(std::cin, password);
                    
                    auto user = UserService.login(username, password);
                    if (user) {
                        std::cout << "Login successful!\n";
                        std::cout << "Welcome, " << user.value()->getUsername() << "!\n";
                        std::cout << "Your role is: " << 
                            (user.value()->getRole() == UserRole::ADMIN ? "ADMIN" : "USER") << "\n";
                        currentUser = user.value();
                    } else {
                        std::cout << "Login failed. Invalid username or password.\n";
                    }
                    break;
                }
                case 3: // Exit
                    std::cout << "Goodbye!\n";
                    return 0;
                default:
                    std::cout << "Invalid option. Please try again.\n";
            }
        } else {
            handleUserEdit(UserService, currentUser);
            currentUser.reset(); // Logout
        }
    }

    return 0;
}