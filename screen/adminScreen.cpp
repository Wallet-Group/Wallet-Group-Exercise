#include "adminScreen.h"
#include "../helper/OTPHelper.h"
#include <iostream>
#include <iomanip>

using namespace std;

void showAdminScreen(shared_ptr<User> admin, UserService& userService, WalletService& walletService) {
    int choice;
    bool running = true;
    while (running) {
        cout << "\n--- Admin Menu ---" << endl;
        cout << "1. View User List" << endl;
        cout << "2. Add New User" << endl;
        cout << "3. Change Username" << endl;
        cout << "4. Change Password with OTP" << endl;
        cout << "5. Wallet Management" << endl;
        cout << "6. View All Wallets" << endl;
        cout << "7. View All Transactions" << endl;
        cout << "8. Coin Exchange Management" << endl;
        cout << "9. View All Exchange Transactions" << endl;
        cout << "10. Logout" << endl;
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
                showWalletScreen(admin, walletService);
                break;
            case 6: {
                cout << "\n--- All Wallets ---" << endl;
                auto allWallets = walletService.getAllWallets();
                if (allWallets.empty()) {
                    cout << "No wallets found." << endl;
                } else {
                    cout << "Wallet ID\tOwner\t\tName\t\t\tBalance" << endl;
                    cout << "---------\t-----\t\t----\t\t\t-------" << endl;
                    for (const auto& wallet : allWallets) {
                        cout << wallet->getWalletId() << "\t\t" 
                             << wallet->getOwnerUsername() << "\t\t"
                             << wallet->getWalletName() << "\t\t\t" 
                             << fixed << setprecision(2) << wallet->getBalance() << endl;
                    }
                }
                break;
            }
            case 7: {
                cout << "\n--- All Transactions ---" << endl;
                auto allTransactions = walletService.getAllTransactions();
                if (allTransactions.empty()) {
                    cout << "No transactions found." << endl;
                } else {
                    cout << "Transaction ID\tFrom\t\tTo\t\tAmount\tStatus\t\tDate" << endl;
                    cout << "--------------\t----\t\t--\t\t------\t------\t\t----" << endl;
                    for (const auto& transaction : allTransactions) {
                        cout << transaction->getTransactionId() << "\t"
                             << transaction->getFromUsername() << "\t\t"
                             << transaction->getToUsername() << "\t\t"
                             << fixed << setprecision(2) << transaction->getAmount() << "\t"
                             << transaction->getStatus() << "\t\t"
                             << transaction->getFormattedTimestamp() << endl;
                    }
                }
                break;
            }
            case 8: {
                cout << "\n--- Coin Exchange Management ---" << endl;
                cout << "Current coin price: $" << fixed << setprecision(2) << walletService.getCurrentCoinPrice() << " per coin" << endl;
                cout << "Total coins in circulation: " << fixed << setprecision(2) << walletService.getTotalCoinsInCirculation() << endl;
                cout << "Available coins: " << fixed << setprecision(2) << walletService.getAvailableCoins() << endl;
                
                // Show exchange status
                if (walletService.isExchangeEnabled()) {
                    cout << "Exchange Status: ENABLED (Users can exchange USD for coins)" << endl;
                } else {
                    cout << "Exchange Status: DISABLED (Users cannot exchange until price is configured)" << endl;
                }
                
                cout << "\n1. Set Coin Price (Enables exchange for users)" << endl;
                cout << "2. Add Available Coins" << endl;
                cout << "3. Back to Admin Menu" << endl;
                cout << "Select an option: ";
                
                int exchangeChoice;
                cin >> exchangeChoice;
                cin.ignore();
                
                switch (exchangeChoice) {
                    case 1: {
                        double newPrice;
                        cout << "Enter new coin price (USD per coin): $";
                        cin >> newPrice;
                        cin.ignore();
                        
                        if (newPrice <= 0) {
                            cout << "Error: Coin price must be positive." << endl;
                            break;
                        }
                        
                        if (walletService.setCoinPrice(newPrice)) {
                            cout << "Coin price updated successfully!" << endl;
                            cout << "Exchange is now ENABLED for users." << endl;
                        } else {
                            cout << "Failed to update coin price." << endl;
                        }
                        break;
                    }
                    case 2: {
                        double availableCoins;
                        cout << "Enter number of coins to add to available supply: ";
                        cin >> availableCoins;
                        cin.ignore();
                        
                        if (availableCoins < 0) {
                            cout << "Error: Available coins to add cannot be negative." << endl;
                            break;
                        }
                        
                        if (walletService.addCoinAvailable(availableCoins)) {
                            cout << "Available coins increased successfully!" << endl;
                        } else {
                            cout << "Failed to add available coins." << endl;
                        }
                        break;
                    }
                    case 3:
                        break;
                    default:
                        cout << "Invalid option." << endl;
                }
                break;
            }
            case 9: {
                cout << "\n--- All Exchange Transactions ---" << endl;
                auto allExchangeTransactions = walletService.getAllExchangeTransactions();
                if (allExchangeTransactions.empty()) {
                    cout << "No exchange transactions found." << endl;
                } else {
                    cout << "Exchange ID\tUser\t\tUSD Amount\tCoins\tPrice\tStatus\t\tDate" << endl;
                    cout << "-----------\t----\t\t----------\t-----\t-----\t------\t\t----" << endl;
                    for (const auto& transaction : allExchangeTransactions) {
                        cout << transaction->getTransactionId() << "\t"
                             << transaction->getUsername() << "\t\t"
                             << "$" << fixed << setprecision(2) << transaction->getUsdAmount() << "\t\t"
                             << fixed << setprecision(2) << transaction->getCoinAmount() << "\t"
                             << "$" << fixed << setprecision(2) << transaction->getCoinPriceAtTime() << "\t"
                             << transaction->getStatus() << "\t\t"
                             << transaction->getFormattedTimestamp() << endl;
                    }
                }
                break;
            }
            case 10:
                cout << "Logging out..." << endl;
                running = false;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
} 