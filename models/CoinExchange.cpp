#include "CoinExchange.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

CoinExchange::CoinExchange(double initialPrice, double maxSupply)
    : coinPrice(initialPrice), totalCoinsInCirculation(0.0), maxCoinSupply(maxSupply),
      lastPriceUpdate(std::chrono::system_clock::now()) {
    if (initialPrice <= 0) {
        throw std::invalid_argument("Coin price must be positive");
    }
    if (maxSupply <= 0) {
        throw std::invalid_argument("Max coin supply must be positive");
    }
}

double CoinExchange::getCoinPrice() const {
    return coinPrice;
}

double CoinExchange::getTotalCoinsInCirculation() const {
    return totalCoinsInCirculation;
}

double CoinExchange::getMaxCoinSupply() const {
    return maxCoinSupply;
}

double CoinExchange::getAvailableCoins() const {
    return maxCoinSupply - totalCoinsInCirculation;
}

std::chrono::system_clock::time_point CoinExchange::getLastPriceUpdate() const {
    return lastPriceUpdate;
}

void CoinExchange::setCoinPrice(double newPrice) {
    if (newPrice <= 0) {
        throw std::invalid_argument("Coin price must be positive");
    }
    coinPrice = newPrice;
    lastPriceUpdate = std::chrono::system_clock::now();
}

void CoinExchange::setMaxCoinSupply(double newMaxSupply) {
    if (newMaxSupply <= 0) {
        throw std::invalid_argument("Max coin supply must be positive");
    }
    if (newMaxSupply < totalCoinsInCirculation) {
        throw std::invalid_argument("New max supply cannot be less than current circulation");
    }
    maxCoinSupply = newMaxSupply;
}

bool CoinExchange::canExchangeForCoins(double usdAmount) const {
    if (usdAmount <= 0) return false;
    double coinsToReceive = calculateCoinsForUSD(usdAmount);
    return coinsToReceive > 0 && coinsToReceive <= getAvailableCoins();
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
    if (totalCoinsInCirculation + amount > maxCoinSupply) return false;
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