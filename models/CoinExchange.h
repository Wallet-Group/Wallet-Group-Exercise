#pragma once
#include <string>
#include <chrono>

class CoinExchange {
private:
    double coinPrice; // Price per coin in USD
    double totalCoinsInCirculation;
    double maxCoinSupply;
    std::chrono::system_clock::time_point lastPriceUpdate;

public:
    CoinExchange(double initialPrice = 1.0, double maxSupply = 1000000.0);
    
    // Getters
    double getCoinPrice() const;
    double getTotalCoinsInCirculation() const;
    double getMaxCoinSupply() const;
    double getAvailableCoins() const;
    std::chrono::system_clock::time_point getLastPriceUpdate() const;
    
    // Setters
    void setCoinPrice(double newPrice);
    void setMaxCoinSupply(double newMaxSupply);
    
    // Exchange operations
    bool canExchangeForCoins(double usdAmount) const;
    double calculateCoinsForUSD(double usdAmount) const;
    double calculateUSDForCoins(double coinAmount) const;
    bool addCoinsToCirculation(double amount);
    bool removeCoinsFromCirculation(double amount);
    
    // Utility
    std::string getFormattedLastUpdate() const;
}; 