#pragma once
#include <string>
#include <chrono>

class ExchangeTransaction {
private:
    std::string transactionId;
    std::string username;
    std::string walletId;
    double usdAmount;
    double coinAmount;
    double coinPriceAtTime;
    std::chrono::system_clock::time_point timestamp;
    std::string status; // "pending", "completed", "failed"

public:
    ExchangeTransaction(const std::string& transactionId, const std::string& username, 
                       const std::string& walletId, double usdAmount, double coinAmount, 
                       double coinPriceAtTime);
    
    // Getters
    std::string getTransactionId() const;
    std::string getUsername() const;
    std::string getWalletId() const;
    double getUsdAmount() const;
    double getCoinAmount() const;
    double getCoinPriceAtTime() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    std::string getStatus() const;
    
    // Setters
    void setStatus(const std::string& newStatus);
    
    // Utility
    std::string getFormattedTimestamp() const;
}; 