#pragma once
#include <string>

class Wallet {
private:
    std::string walletId;
    std::string ownerUsername;
    std::string walletName;
    double balance;

public:
    Wallet(const std::string& walletId, const std::string& ownerUsername, const std::string& walletName, double balance = 0.0);
    
    // Getters
    std::string getWalletId() const;
    std::string getOwnerUsername() const;
    std::string getWalletName() const;
    double getBalance() const;
    
    // Setters
    void setWalletName(const std::string& newName);
    void setBalance(double newBalance);
    
    // Wallet operations
    bool addFunds(double amount);
    bool deductFunds(double amount);
    bool hasSufficientFunds(double amount) const;
}; 