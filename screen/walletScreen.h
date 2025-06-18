#pragma once
#include <memory>
#include <string>
#include "../models/User.h"
#include "../interface/WalletService.h"

void showWalletScreen(std::shared_ptr<User> user, WalletService& walletService); 