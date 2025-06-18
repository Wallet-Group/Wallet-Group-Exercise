#include "userScreen.h"
#include "../helper/OTPHelper.h"
#include <iostream>

using namespace std;

void showUserScreen(shared_ptr<User> user, UserService& userService, WalletService& walletService) {
    int choice;
    bool running = true;
    while (running) {
        cout << "\n--- User Menu ---" << endl;
        cout << "1. Change Username" << endl;
        cout << "2. Change Password with OTP" << endl;
        cout << "3. Wallet Management" << endl;
        cout << "4. Logout" << endl;
        cout << "Select an option: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                string newUsername;
                cout << "Enter new username: ";
                getline(cin, newUsername);
                if (userService.updateUsername(user->getUsername(), newUsername)) {
                    cout << "Username updated successfully!" << endl;
                    user->updateUsername(newUsername);
                } else {
                    cout << "Failed to update username. It may already be taken." << endl;
                }
                break;
            }
            case 2: {
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
                OTPHelper::generateOTP(user->getUsername());
                cout << "Please check your OTP and enter it below." << endl;
                
                cout << "Enter OTP: ";
                getline(cin, otp);
                
                if (userService.changePasswordWithOTP(user->getUsername(), oldPassword, newPassword, otp)) {
                    cout << "Password changed successfully with OTP verification!" << endl;
                } else {
                    cout << "Failed to change password. Please check your inputs and OTP." << endl;
                }
                break;
            }
            case 3:
                showWalletScreen(user, walletService);
                break;
            case 4:
                cout << "Logging out..." << endl;
                running = false;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
} 