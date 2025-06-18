#pragma once
#include <string>
#include <chrono>

class Transaction {
private:
    std::string transactionId;
    std::string fromWalletId;
    std::string toWalletId;
    std::string fromUsername;
    std::string toUsername;
    double amount;
    std::chrono::system_clock::time_point timestamp;
    std::string status; // "pending", "completed", "failed"

public:
    Transaction(const std::string& transactionId, const std::string& fromWalletId, const std::string& toWalletId,
                const std::string& fromUsername, const std::string& toUsername, double amount);
    
    // Getters
    std::string getTransactionId() const;
    std::string getFromWalletId() const;
    std::string getToWalletId() const;
    std::string getFromUsername() const;
    std::string getToUsername() const;
    double getAmount() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    std::string getStatus() const;
    
    // Setters
    void setStatus(const std::string& newStatus);
    
    // Utility
    std::string getFormattedTimestamp() const;
}; 