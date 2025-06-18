#include "../interface/WalletService.h"
#include "../helper/File/JsonFileManager.h"
#include "../defined/constant.h"
#include <filesystem>
#include <random>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace nlohmann;

WalletService::WalletService() {
    // Create data directory if it doesn't exist
    filesystem::create_directories("data");
    // Load existing data
    loadWallets();
    loadTransactions();
    loadExchangeTransactions();
    loadCoinExchange();
    initializeSourceWallet();
}

string WalletService::generateWalletId() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(100000, 999999);
    return "W" + to_string(dis(gen));
}

string WalletService::generateTransactionId() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(100000, 999999);
    return "T" + to_string(dis(gen));
}

string WalletService::generateExchangeTransactionId() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(100000, 999999);
    return "E" + to_string(dis(gen));
}

optional<shared_ptr<Wallet>> WalletService::findWalletById(const string& walletId) const {
    auto it = wallets.find(walletId);
    if (it != wallets.end()) {
        return it->second;
    }
    return nullopt;
}

vector<shared_ptr<Wallet>> WalletService::getWalletsByOwner(const string& username) const {
    vector<shared_ptr<Wallet>> userWallets;
    for (const auto& pair : wallets) {
        if (pair.second->getOwnerUsername() == username) {
            userWallets.push_back(pair.second);
        }
    }
    return userWallets;
}

shared_ptr<Wallet> WalletService::getSourceWallet() {
    auto sourceWallet = findWalletById(constants::SOURCE_WALLET_ID);
    if (sourceWallet) {
        return sourceWallet.value();
    }
    return nullptr;
}

bool WalletService::initializeSourceWallet() {
    auto sourceWallet = getSourceWallet();
    if (!sourceWallet) {
        // Create source wallet if it doesn't exist
        auto newSourceWallet = make_shared<Wallet>(
            constants::SOURCE_WALLET_ID,
            constants::SOURCE_WALLET_OWNER,
            constants::SOURCE_WALLET_NAME,
            1000000.0 // Default large balance for source wallet
        );
        wallets[constants::SOURCE_WALLET_ID] = newSourceWallet;
        return saveWallets();
    }
    return true;
}

bool WalletService::loadWallets() {
    json j;
    if (!JsonFileManager::loadFromFile(WALLETS_FILE, j)) {
        return true; // File doesn't exist yet
    }
    
    try {
        wallets.clear();
        for (const auto& walletJson : j["wallets"]) {
            auto walletPtr = jsonToWallet(walletJson);
            wallets[walletPtr->getWalletId()] = walletPtr;
        }
        return true;
    } catch (const exception& e) {
        cout << "Error loading wallets: " << e.what() << endl;
        return false;
    }
}

bool WalletService::saveWallets() const {
    try {
        json j;
        j["wallets"] = json::array();
        for (const auto& pair : wallets) {
            j["wallets"].push_back(walletToJson(*pair.second));
        }
        return JsonFileManager::saveToFile(WALLETS_FILE, j);
    } catch (const exception& e) {
        cout << "Error saving wallets: " << e.what() << endl;
        return false;
    }
}

bool WalletService::loadTransactions() {
    json j;
    if (!JsonFileManager::loadFromFile(TRANSACTIONS_FILE, j)) {
        return true; // File doesn't exist yet
    }
    
    try {
        transactions.clear();
        for (const auto& transactionJson : j["transactions"]) {
            auto transactionPtr = jsonToTransaction(transactionJson);
            transactions.push_back(transactionPtr);
        }
        return true;
    } catch (const exception& e) {
        cout << "Error loading transactions: " << e.what() << endl;
        return false;
    }
}

bool WalletService::saveTransactions() const {
    try {
        json j;
        j["transactions"] = json::array();
        for (const auto& transaction : transactions) {
            j["transactions"].push_back(transactionToJson(*transaction));
        }
        return JsonFileManager::saveToFile(TRANSACTIONS_FILE, j);
    } catch (const exception& e) {
        cout << "Error saving transactions: " << e.what() << endl;
        return false;
    }
}

bool WalletService::loadExchangeTransactions() {
    json j;
    if (!JsonFileManager::loadFromFile(EXCHANGE_TRANSACTIONS_FILE, j)) {
        return true;
    }
    
    try {
        exchangeTransactions.clear();
        for (const auto& transactionJson : j["exchange_transactions"]) {
            auto transactionPtr = jsonToExchangeTransaction(transactionJson);
            exchangeTransactions.push_back(transactionPtr);
        }
        return true;
    } catch (const exception& e) {
        cout << "Error loading exchange transactions: " << e.what() << endl;
        return false;
    }
}

bool WalletService::saveExchangeTransactions() const {
    try {
        json j;
        j["exchange_transactions"] = json::array();
        for (const auto& transaction : exchangeTransactions) {
            j["exchange_transactions"].push_back(exchangeTransactionToJson(*transaction));
        }
        return JsonFileManager::saveToFile(EXCHANGE_TRANSACTIONS_FILE, j);
    } catch (const exception& e) {
        cout << "Error saving exchange transactions: " << e.what() << endl;
        return false;
    }
}

bool WalletService::loadCoinExchange() {
    json j;
    if (!JsonFileManager::loadFromFile(COIN_EXCHANGE_FILE, j)) {
        // Initialize with default values
        coinExchange = make_shared<CoinExchange>(constants::DEFAULT_COIN_PRICE);
        return saveCoinExchange();
    }
    
    try {
        coinExchange = jsonToCoinExchange(j);
        return true;
    } catch (const exception& e) {
        cout << "Error loading coin exchange: " << e.what() << endl;
        coinExchange = make_shared<CoinExchange>(constants::DEFAULT_COIN_PRICE);
        return false;
    }
}

bool WalletService::saveCoinExchange() const {
    try {
        json j = coinExchangeToJson(*coinExchange);
        return JsonFileManager::saveToFile(COIN_EXCHANGE_FILE, j);
    } catch (const exception& e) {
        cout << "Error saving coin exchange: " << e.what() << endl;
        return false;
    }
}

json WalletService::walletToJson(const Wallet& wallet) const {
    json j;
    j["walletId"] = wallet.getWalletId();
    j["ownerUsername"] = wallet.getOwnerUsername();
    j["walletName"] = wallet.getWalletName();
    j["balance"] = wallet.getBalance();
    return j;
}

shared_ptr<Wallet> WalletService::jsonToWallet(const json& j) const {
    return make_shared<Wallet>(
        j["walletId"].get<string>(),
        j["ownerUsername"].get<string>(),
        j["walletName"].get<string>(),
        j["balance"].get<double>()
    );
}

json WalletService::transactionToJson(const Transaction& transaction) const {
    json j;
    j["transactionId"] = transaction.getTransactionId();
    j["fromWalletId"] = transaction.getFromWalletId();
    j["toWalletId"] = transaction.getToWalletId();
    j["fromUsername"] = transaction.getFromUsername();
    j["toUsername"] = transaction.getToUsername();
    j["amount"] = transaction.getAmount();
    j["timestamp"] = transaction.getFormattedTimestamp();
    j["status"] = transaction.getStatus();
    return j;
}

shared_ptr<Transaction> WalletService::jsonToTransaction(const json& j) const {
    // Note: This is a simplified version. In a real implementation,
    // you'd need to parse the timestamp string back to time_point
    return make_shared<Transaction>(
        j["transactionId"].get<string>(),
        j["fromWalletId"].get<string>(),
        j["toWalletId"].get<string>(),
        j["fromUsername"].get<string>(),
        j["toUsername"].get<string>(),
        j["amount"].get<double>()
    );
}

json WalletService::exchangeTransactionToJson(const ExchangeTransaction& transaction) const {
    json j;
    j["transactionId"] = transaction.getTransactionId();
    j["username"] = transaction.getUsername();
    j["walletId"] = transaction.getWalletId();
    j["usdAmount"] = transaction.getUsdAmount();
    j["coinAmount"] = transaction.getCoinAmount();
    j["coinPriceAtTime"] = transaction.getCoinPriceAtTime();
    j["timestamp"] = transaction.getFormattedTimestamp();
    j["status"] = transaction.getStatus();
    return j;
}

shared_ptr<ExchangeTransaction> WalletService::jsonToExchangeTransaction(const json& j) const {
    return make_shared<ExchangeTransaction>(
        j["transactionId"].get<string>(),
        j["username"].get<string>(),
        j["walletId"].get<string>(),
        j["usdAmount"].get<double>(),
        j["coinAmount"].get<double>(),
        j["coinPriceAtTime"].get<double>()
    );
}

json WalletService::coinExchangeToJson(const CoinExchange& exchange) const {
    json j;
    j["coinPrice"] = exchange.getCoinPrice();
    j["totalCoinsInCirculation"] = exchange.getTotalCoinsInCirculation();
    j["lastPriceUpdate"] = exchange.getFormattedLastUpdate();
    j["priceConfigured"] = exchange.isPriceConfigured();
    return j;
}

shared_ptr<CoinExchange> WalletService::jsonToCoinExchange(const json& j) const {
    auto exchange = make_shared<CoinExchange>(j["coinPrice"].get<double>());
    
    // Restore the circulation amount
    if (j.contains("totalCoinsInCirculation")) {
        double circulation = j["totalCoinsInCirculation"].get<double>();
        // Add coins to circulation to restore the state
        for (int i = 0; i < static_cast<int>(circulation); i++) {
            exchange->addCoinsToCirculation(1.0);
        }
        // Add remaining fractional coins
        double fractional = circulation - static_cast<int>(circulation);
        if (fractional > 0) {
            exchange->addCoinsToCirculation(fractional);
        }
    }
    
    // Restore price configured status
    if (j.contains("priceConfigured") && j["priceConfigured"].get<bool>()) {
        // If it was configured, we need to mark it as such
        // Since we can't directly set the flag, we'll call setCoinPrice again
        // but only if the price is different from default
        if (j["coinPrice"].get<double>() != 1.0) {
            exchange->setCoinPrice(j["coinPrice"].get<double>());
        }
    }
    
    return exchange;
}

bool WalletService::createWallet(const string& ownerUsername, const string& walletName) {
    if (walletName.empty()) {
        cout << "Error: Wallet name cannot be empty." << endl;
        return false;
    }
    
    string walletId = generateWalletId();
    while (findWalletById(walletId).has_value()) {
        walletId = generateWalletId(); // Ensure unique ID
    }
    
    auto wallet = make_shared<Wallet>(walletId, ownerUsername, walletName);
    wallets[walletId] = wallet;
    
    if (saveWallets()) {
        cout << "Wallet created successfully! ID: " << walletId << endl;
        return true;
    } else {
        wallets.erase(walletId);
        cout << "Error: Failed to save wallet." << endl;
        return false;
    }
}

bool WalletService::deleteWallet(const string& walletId, const string& ownerUsername) {
    auto wallet = findWalletById(walletId);
    if (!wallet) {
        cout << "Error: Wallet not found." << endl;
        return false;
    }
    
    if (wallet.value()->getOwnerUsername() != ownerUsername) {
        cout << "Error: You can only delete your own wallets." << endl;
        return false;
    }
    
    if (wallet.value()->getBalance() > 0) {
        cout << "Error: Cannot delete wallet with remaining balance." << endl;
        return false;
    }
    
    wallets.erase(walletId);
    return saveWallets();
}

vector<shared_ptr<Wallet>> WalletService::getUserWallets(const string& username) const {
    return getWalletsByOwner(username);
}

optional<shared_ptr<Wallet>> WalletService::getWallet(const string& walletId) const {
    return findWalletById(walletId);
}

bool WalletService::addFunds(const string& walletId, double amount) {
    auto wallet = findWalletById(walletId);
    if (!wallet) {
        cout << "Error: Wallet not found." << endl;
        return false;
    }
    
    if (wallet.value()->addFunds(amount)) {
        return saveWallets();
    } else {
        cout << "Error: Invalid amount." << endl;
        return false;
    }
}

bool WalletService::deductFunds(const string& walletId, double amount) {
    auto wallet = findWalletById(walletId);
    if (!wallet) {
        cout << "Error: Wallet not found." << endl;
        return false;
    }
    
    if (wallet.value()->deductFunds(amount)) {
        return saveWallets();
    } else {
        cout << "Error: Insufficient funds or invalid amount." << endl;
        return false;
    }
}

bool WalletService::transferCoins(const string& fromWalletId, const string& toWalletId, double amount, const string& fromUsername) {
    if (amount <= 0) {
        cout << "Error: Transfer amount must be positive." << endl;
        return false;
    }
    
    auto fromWallet = findWalletById(fromWalletId);
    auto toWallet = findWalletById(toWalletId);
    
    if (!fromWallet) {
        cout << "Error: Source wallet not found." << endl;
        return false;
    }
    
    if (!toWallet) {
        cout << "Error: Destination wallet not found." << endl;
        return false;
    }
    
    if (fromWallet.value()->getOwnerUsername() != fromUsername) {
        cout << "Error: You can only transfer from your own wallets." << endl;
        return false;
    }
    
    if (fromWalletId == toWalletId) {
        cout << "Error: Cannot transfer to the same wallet." << endl;
        return false;
    }
    
    if (!fromWallet.value()->hasSufficientFunds(amount)) {
        cout << "Error: Insufficient funds for transfer." << endl;
        return false;
    }
    
    // Create transaction record
    string transactionId = generateTransactionId();
    auto transaction = make_shared<Transaction>(
        transactionId, fromWalletId, toWalletId,
        fromWallet.value()->getOwnerUsername(),
        toWallet.value()->getOwnerUsername(),
        amount
    );
    
    // Perform transfer
    if (fromWallet.value()->deductFunds(amount) && toWallet.value()->addFunds(amount)) {
        transaction->setStatus("completed");
        transactions.push_back(transaction);
        
        if (saveWallets() && saveTransactions()) {
            cout << "Transfer completed successfully! Transaction ID: " << transactionId << endl;
            return true;
        } else {
            // Rollback on save failure
            fromWallet.value()->addFunds(amount);
            toWallet.value()->deductFunds(amount);
            cout << "Error: Failed to save transaction." << endl;
            return false;
        }
    } else {
        transaction->setStatus("failed");
        transactions.push_back(transaction);
        saveTransactions();
        cout << "Error: Transfer failed." << endl;
        return false;
    }
}

vector<shared_ptr<Transaction>> WalletService::getUserTransactions(const string& username) const {
    vector<shared_ptr<Transaction>> userTransactions;
    for (const auto& transaction : transactions) {
        if (transaction->getFromUsername() == username || transaction->getToUsername() == username) {
            userTransactions.push_back(transaction);
        }
    }
    return userTransactions;
}

vector<shared_ptr<Transaction>> WalletService::getWalletTransactions(const string& walletId) const {
    vector<shared_ptr<Transaction>> walletTransactions;
    for (const auto& transaction : transactions) {
        if (transaction->getFromWalletId() == walletId || transaction->getToWalletId() == walletId) {
            walletTransactions.push_back(transaction);
        }
    }
    return walletTransactions;
}

bool WalletService::exchangeUSDForCoins(const string& username, const string& walletId, double usdAmount) {
    if (usdAmount <= 0) {
        cout << "Error: USD amount must be positive." << endl;
        return false;
    }
    
    // Check if exchange is enabled (price configured by admin)
    if (!isExchangeEnabled()) {
        cout << "Error: Coin exchange is not available. Admin must configure the coin price first." << endl;
        return false;
    }
    
    auto wallet = findWalletById(walletId);
    if (!wallet) {
        cout << "Error: Wallet not found." << endl;
        return false;
    }
    
    if (wallet.value()->getOwnerUsername() != username) {
        cout << "Error: You can only exchange coins to your own wallets." << endl;
        return false;
    }
    
    auto sourceWallet = getSourceWallet();
    if (!sourceWallet) {
        cout << "Error: Source wallet not available." << endl;
        return false;
    }
    
    if (!coinExchange->canExchangeForCoins(usdAmount)) {
        cout << "Error: Insufficient coins available for exchange." << endl;
        return false;
    }
    
    double coinAmount = coinExchange->calculateCoinsForUSD(usdAmount);
    double currentPrice = coinExchange->getCoinPrice();
    
    // Create exchange transaction record
    string exchangeId = generateExchangeTransactionId();
    auto exchangeTransaction = make_shared<ExchangeTransaction>(
        exchangeId, username, walletId, usdAmount, coinAmount, currentPrice
    );
    
    // Perform the exchange
    if (sourceWallet->deductFunds(coinAmount) && wallet.value()->addFunds(coinAmount)) {
        coinExchange->addCoinsToCirculation(coinAmount);
        exchangeTransaction->setStatus("completed");
        exchangeTransactions.push_back(exchangeTransaction);
        
        if (saveWallets() && saveExchangeTransactions() && saveCoinExchange()) {
            cout << "Exchange completed successfully!" << endl;
            cout << "You received " << fixed << setprecision(2) << coinAmount << " coins for $" << usdAmount << endl;
            cout << "Exchange ID: " << exchangeId << endl;
            cout << "Exchange rate: $" << fixed << setprecision(2) << currentPrice << " per coin" << endl;
            return true;
        } else {
            // Rollback on save failure
            sourceWallet->addFunds(coinAmount);
            wallet.value()->deductFunds(coinAmount);
            coinExchange->removeCoinsFromCirculation(coinAmount);
            cout << "Error: Failed to save exchange transaction." << endl;
            return false;
        }
    } else {
        exchangeTransaction->setStatus("failed");
        exchangeTransactions.push_back(exchangeTransaction);
        saveExchangeTransactions();
        cout << "Error: Exchange failed." << endl;
        return false;
    }
}

double WalletService::getCurrentCoinPrice() const {
    return coinExchange->getCoinPrice();
}

double WalletService::getAvailableCoins() const {
    return coinExchange->getAvailableCoins();
}

double WalletService::getTotalCoinsInCirculation() const {
    return coinExchange->getTotalCoinsInCirculation();
}

bool WalletService::isExchangeEnabled() const {
    return coinExchange->isExchangeEnabled();
}

vector<shared_ptr<ExchangeTransaction>> WalletService::getUserExchangeTransactions(const string& username) const {
    vector<shared_ptr<ExchangeTransaction>> userExchangeTransactions;
    for (const auto& transaction : exchangeTransactions) {
        if (transaction->getUsername() == username) {
            userExchangeTransactions.push_back(transaction);
        }
    }
    return userExchangeTransactions;
}

vector<shared_ptr<Wallet>> WalletService::getAllWallets() const {
    vector<shared_ptr<Wallet>> allWallets;
    for (const auto& pair : wallets) {
        allWallets.push_back(pair.second);
    }
    return allWallets;
}

vector<shared_ptr<Transaction>> WalletService::getAllTransactions() const {
    return transactions;
}

vector<shared_ptr<ExchangeTransaction>> WalletService::getAllExchangeTransactions() const {
    return exchangeTransactions;
}

bool WalletService::setCoinPrice(double newPrice) {
    try {
        coinExchange->setCoinPrice(newPrice);
        return saveCoinExchange();
    } catch (const exception& e) {
        cout << "Error setting coin price: " << e.what() << endl;
        return false;
    }
}

shared_ptr<CoinExchange> WalletService::getCoinExchange() const {
    return coinExchange;
}

bool WalletService::addCoinAvailable(double availableCoins) {
    try {
        coinExchange->addCoinAvailable(availableCoins);
        return saveCoinExchange();
    } catch (const exception& e) {
        cout << "Error adding available coins: " << e.what() << endl;
        return false;
    }
} 