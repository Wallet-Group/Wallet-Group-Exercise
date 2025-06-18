#include "ExchangeTransaction.h"
#include <sstream>
#include <iomanip>

ExchangeTransaction::ExchangeTransaction(const std::string& transactionId, const std::string& username, 
                                       const std::string& walletId, double usdAmount, double coinAmount, 
                                       double coinPriceAtTime)
    : transactionId(transactionId), username(username), walletId(walletId), 
      usdAmount(usdAmount), coinAmount(coinAmount), coinPriceAtTime(coinPriceAtTime),
      timestamp(std::chrono::system_clock::now()), status("pending") {
}

std::string ExchangeTransaction::getTransactionId() const {
    return transactionId;
}

std::string ExchangeTransaction::getUsername() const {
    return username;
}

std::string ExchangeTransaction::getWalletId() const {
    return walletId;
}

double ExchangeTransaction::getUsdAmount() const {
    return usdAmount;
}

double ExchangeTransaction::getCoinAmount() const {
    return coinAmount;
}

double ExchangeTransaction::getCoinPriceAtTime() const {
    return coinPriceAtTime;
}

std::chrono::system_clock::time_point ExchangeTransaction::getTimestamp() const {
    return timestamp;
}

std::string ExchangeTransaction::getStatus() const {
    return status;
}

void ExchangeTransaction::setStatus(const std::string& newStatus) {
    status = newStatus;
}

std::string ExchangeTransaction::getFormattedTimestamp() const {
    auto time_t = std::chrono::system_clock::to_time_t(timestamp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
} 