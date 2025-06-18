#include <iostream>
#include <string>
#include "interface/UserService.h"
#include "interface/WalletService.h"
#include "screen/userScreen.h"
#include "screen/adminScreen.h"

void printMainMenu() {
    std::cout << "\n=== Authentication System ===\n";
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. Exit\n";
    std::cout << "Choose an option: ";
}

UserRole getRoleFromInput() {
    std::cout << "Select role (1 for ADMIN, 2 for USER): ";
    int roleChoice;
    std::cin >> roleChoice;
    std::cin.ignore(); // Clear the newline character
    
    return (roleChoice == 1) ? UserRole::ADMIN : UserRole::USER;
}

int main() {
    UserService UserService;
    WalletService WalletService;
    int choice;
    std::string username, password, name;
    int yearOfBirth;
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
                    std::cout << "Enter full name: ";
                    std::getline(std::cin, name);
                    std::cout << "Enter year of birth: ";
                    std::cin >> yearOfBirth;
                    std::cin.ignore(); // Clear the newline character
                    
                    if (UserService.registerUser(username, password, name, yearOfBirth, UserRole::USER)) {
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
            // Route to appropriate screen based on user role
            if (currentUser->getRole() == UserRole::ADMIN) {
                showAdminScreen(currentUser, UserService, WalletService);
            } else {
                showUserScreen(currentUser, UserService, WalletService);
            }
            currentUser.reset(); // Logout
        }
    }

    return 0;
}