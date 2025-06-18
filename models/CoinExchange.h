#pragma once
#include <string>
#include <chrono>

class CoinExchange {
private:
    double coinPrice; // Price per coin in USD
    double totalCoinsInCirculation;
    std::chrono::system_clock::time_point lastPriceUpdate;
    bool priceConfigured; // Flag to track if admin has configured the price

public:
    CoinExchange(double initialPrice = 1.0);
    
    // Getters
    double getCoinPrice() const;
    double getTotalCoinsInCirculation() const;
    double getAvailableCoins() const;
    std::chrono::system_clock::time_point getLastPriceUpdate() const;
    bool isPriceConfigured() const; // Check if admin has configured the price
    
    // Setters
    void setCoinPrice(double newPrice);
    void addCoinAvailable(double availableCoins); // Admin can add available coins
    
    // Exchange operations
    bool canExchangeForCoins(double usdAmount) const;
    bool isExchangeEnabled() const; // Check if exchange is enabled (price configured)
    double calculateCoinsForUSD(double usdAmount) const;
    double calculateUSDForCoins(double coinAmount) const;
    bool addCoinsToCirculation(double amount);
    bool removeCoinsFromCirculation(double amount);
    
    // Utility
    std::string getFormattedLastUpdate() const;
}; 