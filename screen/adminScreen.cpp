#include "adminScreen.h"
#include "../helper/OTPHelper.h"
#include <iostream>

using namespace std;

void showAdminScreen(shared_ptr<User> admin, UserService& userService) {
    int choice;
    bool running = true;
    while (running) {
        cout << "\n--- Admin Menu ---" << endl;
        cout << "1. View User List" << endl;
        cout << "2. Add New User" << endl;
        cout << "3. Change Username" << endl;
        cout << "4. Change Password with OTP" << endl;
        cout << "5. Logout" << endl;
        cout << "Select an option: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                cout << "\n--- User List ---" << endl;
                const auto& users = userService.getAllUsers();
                if (users.empty()) {
                    cout << "No users found." << endl;
                } else {
                    cout << "Username\t\tName\t\t\tYear of Birth\tRole" << endl;
                    cout << "--------\t\t----\t\t\t-------------\t----" << endl;
                    for (const auto& pair : users) {
                        const auto& user = pair.second;
                        string roleStr = (user->getRole() == UserRole::ADMIN) ? "ADMIN" : "USER";
                        cout << user->getUsername() << "\t\t" 
                             << user->getName() << "\t\t\t" 
                             << user->getYearOfBirth() << "\t\t" 
                             << roleStr << endl;
                    }
                }
                break;
            }
            case 2: {
                string username, password, name;
                int yearOfBirth, roleChoice;
                
                cout << "Enter new username: ";
                getline(cin, username);
                
                if (username.empty()) {
                    cout << "Error: Username cannot be empty." << endl;
                    break;
                }
                
                if (userService.isUsernameTaken(username)) {
                    cout << "Error: Username already exists." << endl;
                    break;
                }
                
                cout << "Enter password: ";
                getline(cin, password);
                
                if (password.empty()) {
                    cout << "Error: Password cannot be empty." << endl;
                    break;
                }
                
                cout << "Enter full name: ";
                getline(cin, name);
                
                if (name.empty()) {
                    cout << "Error: Name cannot be empty." << endl;
                    break;
                }
                
                cout << "Enter year of birth: ";
                cin >> yearOfBirth;
                cin.ignore();
                
                if (yearOfBirth < 1900 || yearOfBirth > 2025) {
                    cout << "Error: Please enter a valid year of birth (1900-2025)." << endl;
                    break;
                }
                
                cout << "Select role (1 for USER, 2 for ADMIN): ";
                cin >> roleChoice;
                cin.ignore();
                
                UserRole role = (roleChoice == 2) ? UserRole::ADMIN : UserRole::USER;
                
                if (userService.registerUser(username, password, name, yearOfBirth, role)) {
                    cout << "User created successfully!" << endl;
                } else {
                    cout << "Failed to create user." << endl;
                }
                break;
            }
            case 3: {
                string newUsername;
                cout << "Enter new username: ";
                getline(cin, newUsername);
                if (userService.updateUsername(admin->getUsername(), newUsername)) {
                    cout << "Username updated successfully!" << endl;
                    admin->updateUsername(newUsername);
                } else {
                    cout << "Failed to update username. It may already be taken." << endl;
                }
                break;
            }
            case 4: {
                string oldPassword, newPassword, otp;
                
                // Validate new password first
                cout << "Enter old password: ";
                getline(cin, oldPassword);
                cout << "Enter new password: ";
                getline(cin, newPassword);
                
                // Check if new password is empty or same as old
                if (newPassword.empty()) {
                    cout << "Error: New password cannot be empty." << endl;
                    break;
                }
                
                if (newPassword == oldPassword) {
                    cout << "Error: New password must be different from old password." << endl;
                    break;
                }
                
                // Generate OTP
                cout << "Generating OTP..." << endl;
                OTPHelper::generateOTP(admin->getUsername());
                cout << "Please check your OTP and enter it below." << endl;
                
                cout << "Enter OTP: ";
                getline(cin, otp);
                
                if (userService.changePasswordWithOTP(admin->getUsername(), oldPassword, newPassword, otp)) {
                    cout << "Password changed successfully with OTP verification!" << endl;
                } else {
                    cout << "Failed to change password. Please check your inputs and OTP." << endl;
                }
                break;
            }
            case 5:
                cout << "Logging out..." << endl;
                running = false;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
} 