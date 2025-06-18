#include "Transaction.h"
#include <sstream>
#include <iomanip>

Transaction::Transaction(const std::string& transactionId, const std::string& fromWalletId, const std::string& toWalletId,
                         const std::string& fromUsername, const std::string& toUsername, double amount)
    : transactionId(transactionId), fromWalletId(fromWalletId), toWalletId(toWalletId),
      fromUsername(fromUsername), toUsername(toUsername), amount(amount), 
      timestamp(std::chrono::system_clock::now()), status("pending") {
}

std::string Transaction::getTransactionId() const {
    return transactionId;
}

std::string Transaction::getFromWalletId() const {
    return fromWalletId;
}

std::string Transaction::getToWalletId() const {
    return toWalletId;
}

std::string Transaction::getFromUsername() const {
    return fromUsername;
}

std::string Transaction::getToUsername() const {
    return toUsername;
}

double Transaction::getAmount() const {
    return amount;
}

std::chrono::system_clock::time_point Transaction::getTimestamp() const {
    return timestamp;
}

std::string Transaction::getStatus() const {
    return status;
}

void Transaction::setStatus(const std::string& newStatus) {
    status = newStatus;
}

std::string Transaction::getFormattedTimestamp() const {
    auto time_t = std::chrono::system_clock::to_time_t(timestamp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
} 