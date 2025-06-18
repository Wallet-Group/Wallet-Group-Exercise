#include "Wallet.h"
#include <stdexcept>

Wallet::Wallet(const std::string& walletId, const std::string& ownerUsername, const std::string& walletName, double balance)
    : walletId(walletId), ownerUsername(ownerUsername), walletName(walletName), balance(balance) {
    if (balance < 0) {
        throw std::invalid_argument("Balance cannot be negative");
    }
}

std::string Wallet::getWalletId() const {
    return walletId;
}

std::string Wallet::getOwnerUsername() const {
    return ownerUsername;
}

std::string Wallet::getWalletName() const {
    return walletName;
}

double Wallet::getBalance() const {
    return balance;
}

void Wallet::setWalletName(const std::string& newName) {
    walletName = newName;
}

void Wallet::setBalance(double newBalance) {
    if (newBalance < 0) {
        throw std::invalid_argument("Balance cannot be negative");
    }
    balance = newBalance;
}

bool Wallet::addFunds(double amount) {
    if (amount <= 0) {
        return false;
    }
    balance += amount;
    return true;
}

bool Wallet::deductFunds(double amount) {
    if (amount <= 0 || !hasSufficientFunds(amount)) {
        return false;
    }
    balance -= amount;
    return true;
}

bool Wallet::hasSufficientFunds(double amount) const {
    return balance >= amount;
} 