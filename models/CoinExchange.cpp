#include "CoinExchange.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

CoinExchange::CoinExchange(double initialPrice)
    : coinPrice(initialPrice), totalCoinsInCirculation(0.0),
      lastPriceUpdate(std::chrono::system_clock::now()), priceConfigured(false) {
    if (initialPrice <= 0) {
        throw std::invalid_argument("Coin price must be positive");
    }
}

double CoinExchange::getCoinPrice() const {
    return coinPrice;
}

double CoinExchange::getTotalCoinsInCirculation() const {
    return totalCoinsInCirculation;
}

double CoinExchange::getAvailableCoins() const {
    // Available coins are managed by admin through setAvailableCoins
    // This is a simplified approach - in a real system you'd have a separate field
    // For now, we'll assume a large available amount when not explicitly set
    return 1000000.0 - totalCoinsInCirculation; // Default large supply
}

std::chrono::system_clock::time_point CoinExchange::getLastPriceUpdate() const {
    return lastPriceUpdate;
}

bool CoinExchange::isPriceConfigured() const {
    return priceConfigured;
}

void CoinExchange::setCoinPrice(double newPrice) {
    if (newPrice <= 0) {
        throw std::invalid_argument("Coin price must be positive");
    }
    coinPrice = newPrice;
    priceConfigured = true; // Mark as configured when admin sets price
    lastPriceUpdate = std::chrono::system_clock::now();
}

void CoinExchange::addCoinAvailable(double availableCoins) {
    if (availableCoins < 0) {
        throw std::invalid_argument("Available coins to add cannot be negative");
    }
    totalCoinsInCirculation += availableCoins; // Decrease circulation to increase available coins
    if (totalCoinsInCirculation < 0) totalCoinsInCirculation = 0;
}

bool CoinExchange::canExchangeForCoins(double usdAmount) const {
    if (usdAmount <= 0) return false;
    if (!isExchangeEnabled()) return false; // Check if exchange is enabled
    double coinsToReceive = calculateCoinsForUSD(usdAmount);
    return coinsToReceive > 0 && coinsToReceive <= getAvailableCoins();
}

bool CoinExchange::isExchangeEnabled() const {
    return priceConfigured; // Exchange is only enabled if admin has configured the price
}

double CoinExchange::calculateCoinsForUSD(double usdAmount) const {
    if (usdAmount <= 0 || coinPrice <= 0) return 0.0;
    return usdAmount / coinPrice;
}

double CoinExchange::calculateUSDForCoins(double coinAmount) const {
    if (coinAmount <= 0 || coinPrice <= 0) return 0.0;
    return coinAmount * coinPrice;
}

bool CoinExchange::addCoinsToCirculation(double amount) {
    if (amount <= 0) return false;
    totalCoinsInCirculation += amount;
    return true;
}

bool CoinExchange::removeCoinsFromCirculation(double amount) {
    if (amount <= 0) return false;
    if (totalCoinsInCirculation < amount) return false;
    totalCoinsInCirculation -= amount;
    return true;
}

std::string CoinExchange::getFormattedLastUpdate() const {
    auto time_t = std::chrono::system_clock::to_time_t(lastPriceUpdate);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
} 