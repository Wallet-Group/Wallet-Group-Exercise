#include "walletScreen.h"
#include <iostream>
#include <iomanip>

using namespace std;

void showWalletScreen(shared_ptr<User> user, WalletService& walletService) {
    int choice;
    bool running = true;
    while (running) {
        cout << "\n--- Wallet Management ---" << endl;
        cout << "1. View My Wallets" << endl;
        cout << "2. Create New Wallet" << endl;
        cout << "3. Transfer Coins" << endl;
        cout << "4. Add Funds" << endl;
        
        // Check if exchange is enabled before showing the option
        if (walletService.isExchangeEnabled()) {
            cout << "5. Exchange USD for Coins" << endl;
        } else {
            cout << "5. Exchange USD for Coins (Disabled - Admin must configure price)" << endl;
        }
        
        cout << "6. View Transaction History" << endl;
        cout << "7. View Exchange History" << endl;
        cout << "8. Delete Wallet" << endl;
        cout << "9. Back to Main Menu" << endl;
        cout << "Select an option: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                cout << "\n--- My Wallets ---" << endl;
                auto userWallets = walletService.getUserWallets(user->getUsername());
                if (userWallets.empty()) {
                    cout << "No wallets found. Create your first wallet!" << endl;
                } else {
                    cout << "Wallet ID\t\tName\t\t\tBalance" << endl;
                    cout << "---------\t\t----\t\t\t-------" << endl;
                    for (const auto& wallet : userWallets) {
                        cout << wallet->getWalletId() << "\t\t" 
                             << wallet->getWalletName() << "\t\t\t" 
                             << fixed << setprecision(2) << wallet->getBalance() << endl;
                    }
                }
                break;
            }
            case 2: {
                string walletName;
                cout << "Enter wallet name: ";
                getline(cin, walletName);
                
                if (walletName.empty()) {
                    cout << "Error: Wallet name cannot be empty." << endl;
                    break;
                }
                
                if (walletService.createWallet(user->getUsername(), walletName)) {
                    cout << "Wallet created successfully!" << endl;
                } else {
                    cout << "Failed to create wallet." << endl;
                }
                break;
            }
            case 3: {
                string fromWalletId, toWalletId;
                double amount;
                
                // Show user's wallets first
                auto userWallets = walletService.getUserWallets(user->getUsername());
                if (userWallets.empty()) {
                    cout << "You need to create a wallet first." << endl;
                    break;
                }
                
                cout << "\nYour wallets:" << endl;
                for (const auto& wallet : userWallets) {
                    cout << wallet->getWalletId() << " - " << wallet->getWalletName() 
                         << " (Balance: " << fixed << setprecision(2) << wallet->getBalance() << ")" << endl;
                }
                
                cout << "\nEnter source wallet ID: ";
                getline(cin, fromWalletId);
                
                cout << "Enter destination wallet ID: ";
                getline(cin, toWalletId);
                
                cout << "Enter amount to transfer: ";
                cin >> amount;
                cin.ignore();
                
                if (amount <= 0) {
                    cout << "Error: Amount must be positive." << endl;
                    break;
                }
                
                if (walletService.transferCoins(fromWalletId, toWalletId, amount, user->getUsername())) {
                    cout << "Transfer completed successfully!" << endl;
                } else {
                    cout << "Transfer failed. Please check your inputs." << endl;
                }
                break;
            }
            case 4: {
                string walletId;
                double amount;
                
                // Show user's wallets
                auto userWallets = walletService.getUserWallets(user->getUsername());
                if (userWallets.empty()) {
                    cout << "You need to create a wallet first." << endl;
                    break;
                }
                
                cout << "\nYour wallets:" << endl;
                for (const auto& wallet : userWallets) {
                    cout << wallet->getWalletId() << " - " << wallet->getWalletName() 
                         << " (Balance: " << fixed << setprecision(2) << wallet->getBalance() << ")" << endl;
                }
                
                cout << "\nEnter wallet ID: ";
                getline(cin, walletId);
                
                cout << "Enter amount to add: ";
                cin >> amount;
                cin.ignore();
                
                if (amount <= 0) {
                    cout << "Error: Amount must be positive." << endl;
                    break;
                }
                
                if (walletService.addFunds(walletId, amount)) {
                    cout << "Funds added successfully!" << endl;
                } else {
                    cout << "Failed to add funds." << endl;
                }
                break;
            }
            case 5: {
                // Check if exchange is enabled
                if (!walletService.isExchangeEnabled()) {
                    cout << "\n--- Coin Exchange Status ---" << endl;
                    cout << "Coin exchange is currently disabled." << endl;
                    cout << "Admin must configure the coin price before exchanges can be made." << endl;
                    cout << "Please contact an administrator to enable coin exchange." << endl;
                    break;
                }
                
                string walletId;
                double usdAmount;
                
                // Show current coin price and available coins
                cout << "\n--- Coin Exchange ---" << endl;
                cout << "Current coin price: $" << fixed << setprecision(2) << walletService.getCurrentCoinPrice() << " per coin" << endl;
                cout << "Available coins: " << fixed << setprecision(2) << walletService.getAvailableCoins() << endl;
                cout << "Total coins in circulation: " << fixed << setprecision(2) << walletService.getTotalCoinsInCirculation() << endl;
                
                // Show user's wallets
                auto userWallets = walletService.getUserWallets(user->getUsername());
                if (userWallets.empty()) {
                    cout << "You need to create a wallet first." << endl;
                    break;
                }
                
                cout << "\nYour wallets:" << endl;
                for (const auto& wallet : userWallets) {
                    cout << wallet->getWalletId() << " - " << wallet->getWalletName() 
                         << " (Balance: " << fixed << setprecision(2) << wallet->getBalance() << ")" << endl;
                }
                
                cout << "\nEnter wallet ID to receive coins: ";
                getline(cin, walletId);
                
                cout << "Enter USD amount to exchange: $";
                cin >> usdAmount;
                cin.ignore();
                
                if (usdAmount <= 0) {
                    cout << "Error: USD amount must be positive." << endl;
                    break;
                }
                
                // Calculate and show preview
                double coinAmount = usdAmount / walletService.getCurrentCoinPrice();
                cout << "\nExchange Preview:" << endl;
                cout << "USD Amount: $" << fixed << setprecision(2) << usdAmount << endl;
                cout << "Coins to receive: " << fixed << setprecision(2) << coinAmount << endl;
                cout << "Rate: $" << fixed << setprecision(2) << walletService.getCurrentCoinPrice() << " per coin" << endl;
                
                cout << "\nConfirm exchange? (y/n): ";
                char confirm;
                cin >> confirm;
                cin.ignore();
                
                if (confirm == 'y' || confirm == 'Y') {
                    if (walletService.exchangeUSDForCoins(user->getUsername(), walletId, usdAmount)) {
                        cout << "Exchange completed successfully!" << endl;
                    } else {
                        cout << "Exchange failed. Please check your inputs." << endl;
                    }
                } else {
                    cout << "Exchange cancelled." << endl;
                }
                break;
            }
            case 6: {
                cout << "\n--- Transaction History ---" << endl;
                auto transactions = walletService.getUserTransactions(user->getUsername());
                if (transactions.empty()) {
                    cout << "No transactions found." << endl;
                } else {
                    cout << "Transaction ID\tFrom\t\tTo\t\tAmount\tStatus\t\tDate" << endl;
                    cout << "--------------\t----\t\t--\t\t------\t------\t\t----" << endl;
                    for (const auto& transaction : transactions) {
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
            case 7: {
                cout << "\n--- Exchange History ---" << endl;
                auto exchangeTransactions = walletService.getUserExchangeTransactions(user->getUsername());
                if (exchangeTransactions.empty()) {
                    cout << "No exchange transactions found." << endl;
                } else {
                    cout << "Exchange ID\tUSD Amount\tCoins\tPrice\tStatus\t\tDate" << endl;
                    cout << "-----------\t----------\t-----\t-----\t------\t\t----" << endl;
                    for (const auto& transaction : exchangeTransactions) {
                        cout << transaction->getTransactionId() << "\t"
                             << "$" << fixed << setprecision(2) << transaction->getUsdAmount() << "\t\t"
                             << fixed << setprecision(2) << transaction->getCoinAmount() << "\t"
                             << "$" << fixed << setprecision(2) << transaction->getCoinPriceAtTime() << "\t"
                             << transaction->getStatus() << "\t\t"
                             << transaction->getFormattedTimestamp() << endl;
                    }
                }
                break;
            }
            case 8: {
                string walletId;
                
                // Show user's wallets
                auto userWallets = walletService.getUserWallets(user->getUsername());
                if (userWallets.empty()) {
                    cout << "No wallets to delete." << endl;
                    break;
                }
                
                cout << "\nYour wallets:" << endl;
                for (const auto& wallet : userWallets) {
                    cout << wallet->getWalletId() << " - " << wallet->getWalletName() 
                         << " (Balance: " << fixed << setprecision(2) << wallet->getBalance() << ")" << endl;
                }
                
                cout << "\nEnter wallet ID to delete: ";
                getline(cin, walletId);
                
                if (walletService.deleteWallet(walletId, user->getUsername())) {
                    cout << "Wallet deleted successfully!" << endl;
                } else {
                    cout << "Failed to delete wallet." << endl;
                }
                break;
            }
            case 9:
                cout << "Returning to main menu..." << endl;
                running = false;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
} 