#pragma once
#include <memory>
#include <vector>
#include <optional>
#include <string>
#include <unordered_map>
#include "../models/Wallet.h"
#include "../models/Transaction.h"
#include "../models/CoinExchange.h"
#include "../models/ExchangeTransaction.h"
#include "../modules/json/json.hpp"

using namespace std;
using json = nlohmann::json;

class WalletService {
private:
    unordered_map<string, shared_ptr<Wallet>> wallets;
    vector<shared_ptr<Transaction>> transactions;
    vector<shared_ptr<ExchangeTransaction>> exchangeTransactions;
    shared_ptr<CoinExchange> coinExchange;
    const string WALLETS_FILE = "data/wallets.json";
    const string TRANSACTIONS_FILE = "data/transactions.json";
    const string EXCHANGE_TRANSACTIONS_FILE = "data/exchange_transactions.json";
    const string COIN_EXCHANGE_FILE = "data/coin_exchange.json";

    // Helper functions
    string generateWalletId();
    string generateTransactionId();
    string generateExchangeTransactionId();
    optional<shared_ptr<Wallet>> findWalletById(const string& walletId) const;
    vector<shared_ptr<Wallet>> getWalletsByOwner(const string& username) const;
    shared_ptr<Wallet> getSourceWallet();
    bool initializeSourceWallet();
    
    // File operations
    bool loadWallets();
    bool saveWallets() const;
    bool loadTransactions();
    bool saveTransactions() const;
    bool loadExchangeTransactions();
    bool saveExchangeTransactions() const;
    bool loadCoinExchange();
    bool saveCoinExchange() const;
    
    // JSON conversion
    json walletToJson(const Wallet& wallet) const;
    shared_ptr<Wallet> jsonToWallet(const json& j) const;
    json transactionToJson(const Transaction& transaction) const;
    shared_ptr<Transaction> jsonToTransaction(const json& j) const;
    json exchangeTransactionToJson(const ExchangeTransaction& transaction) const;
    shared_ptr<ExchangeTransaction> jsonToExchangeTransaction(const json& j) const;
    json coinExchangeToJson(const CoinExchange& exchange) const;
    shared_ptr<CoinExchange> jsonToCoinExchange(const json& j) const;

public:
    WalletService();
    
    // Wallet management
    bool createWallet(const string& ownerUsername, const string& walletName);
    bool deleteWallet(const string& walletId, const string& ownerUsername);
    vector<shared_ptr<Wallet>> getUserWallets(const string& username) const;
    optional<shared_ptr<Wallet>> getWallet(const string& walletId) const;
    
    // Balance operations
    bool addFunds(const string& walletId, double amount);
    bool deductFunds(const string& walletId, double amount);
    
    // Transfer operations
    bool transferCoins(const string& fromWalletId, const string& toWalletId, double amount, const string& fromUsername);
    vector<shared_ptr<Transaction>> getUserTransactions(const string& username) const;
    vector<shared_ptr<Transaction>> getWalletTransactions(const string& walletId) const;
    
    // Coin exchange operations
    bool exchangeUSDForCoins(const string& username, const string& walletId, double usdAmount);
    double getCurrentCoinPrice() const;
    double getAvailableCoins() const;
    double getTotalCoinsInCirculation() const;
    double getMaxCoinSupply() const;
    vector<shared_ptr<ExchangeTransaction>> getUserExchangeTransactions(const string& username) const;
    
    // Admin functions
    vector<shared_ptr<Wallet>> getAllWallets() const;
    vector<shared_ptr<Transaction>> getAllTransactions() const;
    vector<shared_ptr<ExchangeTransaction>> getAllExchangeTransactions() const;
    bool setCoinPrice(double newPrice);
    bool setMaxCoinSupply(double newMaxSupply);
    shared_ptr<CoinExchange> getCoinExchange() const;
}; 